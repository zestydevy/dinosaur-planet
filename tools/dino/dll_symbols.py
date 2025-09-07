import bisect

from .dll import DLL

class DLLGlobalSymbol:
    def __init__(self,
                 value: int,
                 name: str):
        self.value = value
        self.name = name

class DLLLocalSymbol:
    def __init__(self,
                 section: int,
                 offset: int,
                 name: str):
        self.section = section
        self.offset = offset
        self.name = name

DLL_SECTION_NAMES = {
    0: ".text",
    1: ".rodata",
    2: ".data",
    3: ".bss"
}

class DLLSymbols:
    def __init__(self, dll: DLL, dll_number: str | int, known_symbols: dict[int, str] = {}):
        self.dll = dll
        self.dll_number = dll_number
        self.known_symbols = known_symbols
        
        global_syms: dict[int, DLLGlobalSymbol] = {} # value -> sym
        local_syms: dict[int, dict[int, DLLLocalSymbol]] = {} # section -> offset -> sym
        local_syms[0] = {} # .text
        local_syms[1] = {} # .rodata
        local_syms[2] = {} # .data
        local_syms[3] = {} # .bss
        local_syms_sorted: dict[int, list[DLLLocalSymbol]] = {} # section -> syms
        local_syms_sorted[0] = [] # .text
        local_syms_sorted[1] = [] # .rodata
        local_syms_sorted[2] = [] # .data
        local_syms_sorted[3] = [] # .bss

        self.rodata_voffset = dll.header.rodata_offset + dll.reloc_table.get_size() - dll.header.size
        self.data_voffset = dll.header.data_offset - dll.header.size
        self.bss_voffset = dll.get_bss_offset() - dll.header.size

        exports = set(dll.header.export_offsets)

        for item in known_symbols.items():
            address = item[0]
            name = item[1]

            if address & 0x80000000 or address in exports:
                global_syms[address] = DLLGlobalSymbol(address, name)
            else:
                if address >= self.bss_voffset:
                    section = 3
                    offset = address - self.bss_voffset
                elif address >= self.data_voffset:
                    section = 2
                    offset = address - self.data_voffset
                elif address >= self.rodata_voffset:
                    section = 1
                    offset = address - self.rodata_voffset
                else:
                    section = 0
                    offset = address
                
                sym = DLLLocalSymbol(section, offset, name)
                local_syms[section][offset] = sym
                local_syms_sorted[section].append(sym)
        
        local_syms_sorted[0].sort(key=lambda s: s.offset) # .text
        local_syms_sorted[1].sort(key=lambda s: s.offset) # .rodata
        local_syms_sorted[2].sort(key=lambda s: s.offset) # .data
        local_syms_sorted[3].sort(key=lambda s: s.offset) # .bss
        
        self.global_syms = global_syms
        self.local_syms = local_syms
        self.local_syms_sorted = local_syms_sorted
    
    def convert_absolute_to_relative_address(self, address: int) -> tuple[int, int]:
        if address >= self.bss_voffset:
            return (3, address - self.bss_voffset)
        elif address >= self.data_voffset:
            return (2, address - self.data_voffset)
        elif address >= self.rodata_voffset:
            return (1, address - self.rodata_voffset)
        else:
            return (0, address)

    def get_func_name_or_default(self, offset: int, default: str | None=None):
        # Note: We dont know if the func is local or global, but since .text is always the
        # first section, we can cheat and just use the known symbols map to find the name
        name = self.known_symbols.get(offset, default)
        if name == None:
            name = self.__make_func_name(offset)
        
        return name

    def get_local_name_or_default(self, section: int, offset: int, default: str | None=None):
        sym = self.local_syms[section].get(offset, None)
        if sym != None:
            return sym.name
        
        if default == None:
            match section:
                case 0:
                    default = self.__make_func_name(offset)
                case 1:
                    return "_rodata_{:X}".format(offset)
                case 2:
                    return "_data_{:X}".format(offset)
                case 3:
                    return "_bss_{:X}".format(offset)
                case _:
                    raise ValueError(f"Invalid section idx: {section}")

        return default
    
    def get_local_or_encapsulating(self, section: int, offset: int) -> tuple[str, int]:
        sym = self.local_syms[section].get(offset, None)
        if sym != None:
            return (sym.name, 0)
        
        sym_list = self.local_syms_sorted[section]
        if len(sym_list) == 0:
            # We don't have any local symbols defined, so we can't find an encapsulating symbol.
            # Fallback to generating a symbol for this address.
            return (self.get_local_name_or_default(section, offset), 0)
        
        # Find the symbol that contains this offset
        idx = bisect.bisect(sym_list, offset, key=lambda s: s.offset) - 1
        if idx < 0:
            return (DLL_SECTION_NAMES[section], offset)
        
        sym = sym_list[idx]
        return (sym.name, offset - sym.offset)
    
    def get_jtable_name_or_default(self, offset: int, default: str | None=None):
        sym = self.local_syms[1].get(offset, None)
        if sym != None:
            return sym.name

        if default == None:
            default = "jtbl_{:X}".format(offset)
            
        return default

    def get_global_name_or_default(self, address: int, default: str | None=None):
        sym = self.global_syms.get(address, None)
        if sym != None:
            return sym.name
        
        if default == None:
            if address in self.dll.header.export_offsets:
                default = self.__make_func_name(address)
            elif (address & 0x8000_0000) != 0:
                default = "IMPORT_{:X}".format(address)
            else:
                (section, local_address) = self.convert_absolute_to_relative_address(address)
                sym = self.local_syms[section].get(local_address, None)
                if sym != None:
                    return sym.name
                default = "GLOBAL_{:X}".format(address)
        
        return default
    
    def __make_func_name(self, offset: int):
        if offset == self.dll.header.ctor_offset:
            return "dll_{}_ctor".format(self.dll_number)
        elif offset == self.dll.header.dtor_offset:
            return "dll_{}_dtor".format(self.dll_number)
        else:
            return "dll_{}_func_{:X}".format(self.dll_number, offset)
