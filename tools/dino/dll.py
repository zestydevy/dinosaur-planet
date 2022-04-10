from collections import OrderedDict
import struct
from capstone import CS_ARCH_MIPS, CS_MODE_BIG_ENDIAN, CS_MODE_MIPS64, Cs, CsInsn

class DLL:
    """A Dinosaur Planet DLL"""
    def __init__(self,
                 number: str,
                 header: "DLLHeader",
                 reloc_table: "DLLRelocationTable",
                 functions: "list[DLLFunction] | None") -> None:
        self.number = number
        self.header = header
        self.reloc_table = reloc_table
        self.functions = functions

    @staticmethod
    def parse(data: bytearray, 
              number: str, 
              include_funcs=True,
              vram: int=0x8000_0000):
        header = DLLHeader.parse(data)
        reloc_table = DLLRelocationTable.parse(data, header)
        if include_funcs:
            functions = parse_functions(data, header, reloc_table, vram)
        else:
            functions = None

        return DLL(number, header, reloc_table, functions)

class DLLHeader:
    """The header section (including exports)"""
    def __init__(self, 
                 header_size: int,
                 data_offset: int,
                 rodata_offset: int,
                 export_count: int,
                 ctor_offset: int,
                 dtor_offset: int,
                 export_offsets: "list[int]") -> None:
        self.header_size = header_size
        """Header size in bytes"""
        self.data_offset = data_offset
        """DATA offset (relative to start of header or 0xFFFFFFFF if section is not present)"""
        self.rodata_offset = rodata_offset
        """RODATA offset (relative to start of header or 0xFFFFFFFF if section is not present)"""
        self.export_count = export_count
        """Number of exports"""
        self.ctor_offset = ctor_offset
        """Constructor offset (relative to end of header)"""
        self.dtor_offset = dtor_offset
        """Destructor offset (relative to end of header)"""
        self.export_offsets = export_offsets
        """List of exports (the offsets they specify)"""
    
    @staticmethod
    def parse(data: bytearray) -> "DLLHeader":
        """Given a DLL file, parses and returns the header"""
        header_size = struct.unpack_from(">I", data, offset=0x0)[0]
        data_offsets = struct.unpack_from(">II", data, offset=0x4)
        export_count = struct.unpack_from(">H", data, offset=0xC)[0]
        ctor_dtor = struct.unpack_from(">II", data, offset=0x10)
        export_offsets = struct.unpack_from(">" + ("I" * export_count), data, offset=0x1C)
        
        return DLLHeader(
            header_size=header_size,
            data_offset=data_offsets[0],
            rodata_offset=data_offsets[1],
            export_count=export_count,
            ctor_offset=ctor_dtor[0],
            dtor_offset=ctor_dtor[1],
            export_offsets=list(export_offsets)
        )

class DLLRelocationTable:
    """The relocation table (including global offset table)"""
    def __init__(self,
                 global_offset_table: "list[int]",
                 gp_relocations: "list[int]",
                 data_relocations: "list[int]") -> None:
        self.global_offset_table = global_offset_table
        self.gp_relocations = gp_relocations
        self.data_relocations = data_relocations

    def size(self) -> int:
        """Calculates the size of the relocation table in bytes"""
        # +4 to include table section end markers
        return len(self.global_offset_table) * 4 + 4 \
            + len(self.gp_relocations) * 4 + 4 \
            + len(self.data_relocations) * 4 + 4

    @staticmethod
    def parse(data: bytearray, header: DLLHeader) -> "DLLRelocationTable":
        """Given a DLL file, parses and returns the relocation section"""
        if header.rodata_offset == 0xFFFF_FFFF:
            # No relocation table
            return DLLRelocationTable([], [], [])
        
        offset = header.rodata_offset
        global_offset_table: "list[int]" = []
        while offset < len(data) and (value := struct.unpack_from(">I", data, offset)[0]) != 0xFFFF_FFFE:
            global_offset_table.append(value)
            offset += 0x4
        
        offset += 0x4
        gp_relocations: "list[int]" = []
        while offset < len(data) and (value := struct.unpack_from(">I", data, offset)[0]) != 0xFFFF_FFFD:
            gp_relocations.append(value)
            offset += 0x4
        
        offset += 0x4
        data_relocations: "list[int]" = []
        while offset < len(data) and (value := struct.unpack_from(">I", data, offset)[0]) != 0xFFFF_FFFF:
            data_relocations.append(value)
            offset += 0x4
        
        return DLLRelocationTable(global_offset_table, gp_relocations, data_relocations)

class DLLInst:
    def __init__(self,
                 original: CsInsn,
                 address: int,
                 mnemonic: str,
                 op_str: str,
                 is_branch_delay_slot: bool,
                 label: "str | None") -> None:
        self.original = original
        self.address = address
        self.mnemonic = mnemonic
        self.op_str = op_str
        self.is_branch_delay_slot = is_branch_delay_slot
        self.label = label

    def is_op_modified(self):
        """Whether the operand string was modified during parsing.
        Use original.op_str to get the real value."""
        return self.op_str != self.original.op_str

class DLLFunction:
    def __init__(self,
                 insts: "list[DLLInst]",
                 symbol: str,
                 is_static: bool,
                 auto_symbols: "OrderedDict[str, int]") -> None:
        self.insts = insts
        self.symbol = symbol
        self.is_static = is_static
        self.auto_symbols = auto_symbols
        """A map of symbols (to their address) automatically generated 
        while parsing the function."""

def __mnemonic_has_delay_slot(mnemonic: str) -> bool:
    return (mnemonic.startswith("b") or mnemonic.startswith("j")) and mnemonic != "break"

def __mnemonic_is_branch(mnemonic: str) -> bool:
    return (mnemonic.startswith("b") or mnemonic == "j") and mnemonic != "break"

def parse_functions(data: bytearray, 
                    header: DLLHeader,
                    reloc_table: DLLRelocationTable,
                    vram: int=0x8000_0000) -> "list[DLLFunction]":
    """Parses and returns all functions in the given Dinosaur Planet DLL."""
    # Convert exported function addresses to VRAM
    ctor_vram = header.ctor_offset + vram
    dtor_vram = header.dtor_offset + vram
    exports_vram = [ctor_vram, dtor_vram]
    exports_vram.extend([o + vram for o in header.export_offsets])

    # Determine where in the file the .text section ends
    text_end = min(header.rodata_offset, header.data_offset)

    # Vars
    new_func = True
    last_mnemonic: "str | None" = None

    # Disassemble
    md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS64 + CS_MODE_BIG_ENDIAN)
    insts = [i for i in md.disasm(data[header.header_size:text_end], vram)]

    # Extract all branches
    branches: "list[tuple[int, int]]" = []
    for i in insts:
        if __mnemonic_is_branch(i.mnemonic):
            branch_target = int(i.op_str.split(" ")[-1], 0)
            branches.append((i.address, branch_target))

    # Extract functions
    funcs: "list[DLLFunction]" = []
    cur_func_insts: "list[DLLInst]" = []
    cur_func_name = ""
    cur_func_is_static = False
    cur_func_auto_syms: "OrderedDict[str, int]" = OrderedDict()
    cur_func_inst_index = 0
    for i in insts:
        # Check if this instruction is a branch delay slot of the previous instruction
        is_delay_slot = last_mnemonic != None and __mnemonic_has_delay_slot(last_mnemonic)
        
        if new_func and i.mnemonic != "nop" and not is_delay_slot:
            # Add previous function
            if cur_func_name != "":
                funcs.append(DLLFunction(
                    insts=cur_func_insts,
                    symbol=cur_func_name,
                    is_static=cur_func_is_static,
                    auto_symbols=cur_func_auto_syms
                ))
            
            # New function, determine name and type
            if i.address == ctor_vram:
                cur_func_name = "ctor"
            elif i.address == dtor_vram:
                cur_func_name = "dtor"
            else:
                cur_func_name = ("func_%x" %(i.address))
                cur_func_is_static = not i.address in exports_vram
            
            cur_func_insts = []
            cur_func_auto_syms = OrderedDict()
            new_func = False
            cur_func_inst_index = 0
        
        # Pre-process operand string
        op_str: str = i.op_str
        operands = [op.strip() for op in op_str.split(",")]
        num_operands = len(operands)

        if __mnemonic_is_branch(i.mnemonic):
            # Replace branch address with label
            branch_target = int(operands[-1], 0)
            op_label = (".L%x" %(branch_target))
            op_str = ", ".join(operands[:-1] + [op_label])
        elif cur_func_inst_index < 2 and num_operands > 0 and operands[0] == "$gp":
            # Add _gp_disp to $gp initializer stub
            if cur_func_inst_index == 0:
                assert num_operands == 2
                op_str = ", ".join(operands[:-1] + [r"%hi(_gp_disp)"])
            elif cur_func_inst_index == 1:
                assert num_operands == 3
                op_str = ", ".join(operands[:-1] + [r"%lo(_gp_disp)"])
        elif num_operands > 0 and operands[-1].endswith("($gp)"):
            # Replace $gp references with %got (if not referencing a section)
            gp_mem_op = operands[-1]
            offset = 0 if gp_mem_op == "($gp)" else int(gp_mem_op[:-5], 0)
            if offset >= 16:
                symbol_addr = reloc_table.global_offset_table[offset // 4]
                symbol = ("D_%X" %(symbol_addr))
                op_str = ", ".join(operands[:-1] + [rf"%got({symbol})"])
                cur_func_auto_syms[symbol] = symbol_addr

        # Determine whether this instruction address is branched to
        label: "str | None" = None
        for branch in branches:
            if branch[1] == i.address:
                label = (".L%x" %(i.address))
                break

        # Add instruction
        cur_func_insts.append(DLLInst(
            original=i,
            address=i.address,
            mnemonic=i.mnemonic,
            op_str=op_str,
            is_branch_delay_slot=is_delay_slot,
            label=label
        ))

        # Check for function end
        if i.mnemonic == "jr" and i.op_str == "$ra":
            new_func = True
            for branch in branches:
                if (branch[0] > i.address and branch[1] <= i.address) or (branch[0] <= i.address and branch[1] > i.address):
                    # jr falls within a known branch, so there's more to this function
                    new_func = False
                    break

        # Track last instruction
        last_mnemonic = i.mnemonic
        cur_func_inst_index += 1
    
    # Add final function
    if cur_func_name != "":
        funcs.append(DLLFunction(
            insts=cur_func_insts,
            symbol=cur_func_name,
            is_static=cur_func_is_static,
            auto_symbols=cur_func_auto_syms
        ))

    return funcs
