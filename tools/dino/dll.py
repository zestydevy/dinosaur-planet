from collections import OrderedDict
import struct
from capstone import CS_ARCH_MIPS, CS_MODE_BIG_ENDIAN, CS_MODE_MIPS64, Cs, CsInsn

class DLL:
    functions: "list[DLLFunction] | None"

    """A Dinosaur Planet DLL"""
    def __init__(self,
                 number: str,
                 size_aligned: int,
                 header: "DLLHeader",
                 reloc_table: "DLLRelocationTable") -> None:
        self.number = number
        self.size_aligned = size_aligned
        """The total size of this DLL (in bytes), 16-byte aligned"""
        self.header = header
        self.reloc_table = reloc_table

    def has_data(self) -> bool:
        """Returns whether this DLL has a .data section"""
        return self.header.data_offset != 0xFFFF_FFFF
    
    def has_rodata(self) -> bool:
        """Returns whether this DLL has a .rodata section"""
        return self.header.rodata_offset != 0xFFFF_FFFF

    def get_text_size(self) -> int:
        """Calculates the size (in bytes) of this DLL's .text section"""
        start = self.header.size

        if self.has_rodata():
            end = self.header.rodata_offset
        elif self.has_data():
            end = self.header.data_offset
        else:
            return ((self.size_aligned - start) // 16) * 16
        
        return end - start
    
    def get_rodata_size(self) -> int:
        """Calculates the size (in bytes) of this DLL's .rodata section, 
        excluding the GOT and relocation tables"""
        if not self.has_rodata():
            return 0

        start = self.header.rodata_offset + self.reloc_table.get_size()

        if self.has_data():
            end = self.header.data_offset
        else:
            return ((self.size_aligned - start) // 16) * 16

        return end - start
    
    def get_data_size(self) -> int:
        """Calculates the size (in bytes) of this DLL's .data section"""
        if not self.has_data():
            return 0
        
        start = self.header.data_offset
        end = self.size_aligned

        return ((end - start) // 16) * 16
    
    def get_rom_size(self) -> int:
        """Calculates the total ROM size (in bytes, unaligned) of this DLL"""
        return self.header.size + \
            self.get_text_size() + \
            self.get_rodata_size() + \
            self.reloc_table.get_size() + \
            self.get_data_size()

    def get_ram_size(self) -> int:
        """Calculates the total size (in bytes) of this DLL when loaded into RAM (**excluding BSS!**)"""
        return self.get_text_size() + \
            self.get_rodata_size() + \
            self.get_data_size()

    @staticmethod
    def parse(data: bytes, 
              number: str, 
              include_funcs=True,
              known_symbols: "dict[int, str]"={}):
        header = DLLHeader.parse(data)
        reloc_table = DLLRelocationTable.parse(data, header)
        dll = DLL(number, len(data), header, reloc_table)
        
        if include_funcs:
            dll.functions = parse_functions(data, dll, reloc_table, known_symbols)

        return dll

class DLLHeader:
    """The header section (including exports)"""
    def __init__(self, 
                 size: int,
                 data_offset: int,
                 rodata_offset: int,
                 export_count: int,
                 ctor_offset: int,
                 dtor_offset: int,
                 export_offsets: "list[int]") -> None:
        self.size = size
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
    def parse(data: bytes) -> "DLLHeader":
        """Given a DLL file, parses and returns the header"""
        header_size = struct.unpack_from(">I", data, offset=0x0)[0]
        data_offsets = struct.unpack_from(">II", data, offset=0x4)
        export_count = struct.unpack_from(">H", data, offset=0xC)[0]
        ctor_dtor = struct.unpack_from(">II", data, offset=0x10)
        export_offsets = struct.unpack_from(">" + ("I" * export_count), data, offset=0x1C)
        
        return DLLHeader(
            size=header_size,
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
                 exists: bool,
                 global_offset_table: "list[int]",
                 gp_relocations: "list[int]",
                 data_relocations: "list[int]") -> None:
        self.exists = exists
        self.global_offset_table = global_offset_table
        self.gp_relocations = gp_relocations
        self.data_relocations = data_relocations

    def get_size(self) -> int:
        """Calculates the size of the relocation table in bytes"""
        if not self.exists:
            return 0
        
        # +4 to include table section end markers
        return len(self.global_offset_table) * 4 + 4 \
            + len(self.gp_relocations) * 4 + 4 \
            + len(self.data_relocations) * 4 + 4

    @staticmethod
    def parse(data: bytes, header: DLLHeader) -> "DLLRelocationTable":
        """Given a DLL file, parses and returns the relocation section"""
        if header.rodata_offset == 0xFFFF_FFFF:
            # No relocation table
            return DLLRelocationTable(False, [], [], [])
        
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
        
        return DLLRelocationTable(True, global_offset_table, gp_relocations, data_relocations)

class DLLInst:
    label: "str | None" = None

    def __init__(self,
                 original: CsInsn,
                 address: int,
                 mnemonic: str,
                 op_str: str,
                 is_branch_delay_slot: bool,
                 has_relocation: bool,
                 ref: "str | None") -> None:
        self.original = original
        self.address = address
        self.mnemonic = mnemonic
        self.op_str = op_str
        self.is_branch_delay_slot = is_branch_delay_slot
        self.has_relocation = has_relocation
        self.ref = ref

    def is_op_modified(self):
        """Whether the operand string was modified during parsing.
        Use original.op_str to get the real value."""
        return self.op_str != self.original.op_str

class DLLRelocation:
    def __init__(self, offset: int, type: str, expression: str, got_index: int) -> None:
        self.offset = offset
        self.type = type
        self.expression = expression
        self.got_index = got_index

class DLLFunction:
    def __init__(self,
                 insts: "list[DLLInst]",
                 address: int,
                 symbol: str,
                 is_static: bool,
                 auto_symbols: "OrderedDict[str, int]",
                 relocations: "list[DLLRelocation]") -> None:
        self.insts = insts
        self.address = address
        self.symbol = symbol
        self.is_static = is_static
        self.auto_symbols = auto_symbols
        """A map of symbols (to their address) automatically generated while parsing the function."""
        self.relocations = relocations
        """All instruction relocations in the function, sorted by their position in the original DLL's GOT."""

def parse_functions(data: bytes, 
                    dll: DLL,
                    reloc_table: DLLRelocationTable,
                    known_symbols: "dict[int, str]"={}) -> "list[DLLFunction]":
    """Parses and returns all functions in the given Dinosaur Planet DLL."""
    header = dll.header

    # Determine where in the file the .text section ends
    text_end = header.size + dll.get_text_size()

    # Vars
    new_func = True
    last_mnemonic: "str | None" = None

    # Disassemble
    md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS64 + CS_MODE_BIG_ENDIAN)
    insts = [i for i in md.disasm(data[header.size:text_end], 0x0)]

    # Extract functions
    funcs: "list[DLLFunction]" = []
    cur_func_insts: "list[DLLInst]" = []
    cur_func_name = ""
    cur_func_addr = 0
    cur_func_is_static = False
    cur_func_has_gp_init = False
    cur_func_auto_syms: "OrderedDict[str, int]" = OrderedDict()
    cur_func_relocs: "list[DLLRelocation]" = []
    cur_func_inst_index = 0
    cur_func_branch_dests: "list[int]" = []
    cur_func_forward_branches: "set[int]" = set()

    def add_function():
        if cur_func_name == "":
            return

        # Discard trailing nops
        for idx in range(len(cur_func_insts) - 1, 0, -1):
            i = cur_func_insts[idx]
            if i.mnemonic == "nop" and not i.is_branch_delay_slot:
                cur_func_insts.pop(idx)
            else:
                break

        # Ensure function ends with jr $ra
        # Otherwise, it's not a function
        if len(cur_func_insts) >= 2:
            jr = cur_func_insts[-2] # -2 to account for the delay slot after jr
            if jr.mnemonic != "jr" or jr.op_str != "$ra":
                return

        # Sort relocations by GOT index
        cur_func_relocs.sort(key=lambda r: r.got_index)

        # Add branch labels
        for addr in cur_func_branch_dests:
            idx = (addr - cur_func_addr) // 4
            if idx >= 0 and idx < len(cur_func_insts):
                cur_func_insts[idx].label = ".L{:X}".format(addr)

        # Add function
        funcs.append(DLLFunction(
            insts=cur_func_insts,
            address=cur_func_addr,
            symbol=cur_func_name,
            is_static=cur_func_is_static,
            auto_symbols=cur_func_auto_syms,
            relocations=cur_func_relocs
        ))

    for idx, i in enumerate(insts):
        # Check if this instruction is a branch delay slot of the previous instruction
        is_delay_slot = last_mnemonic is not None and __mnemonic_has_delay_slot(last_mnemonic)
        
        if new_func and i.mnemonic != "nop" and not is_delay_slot:
            # Add previous function
            add_function()
            
            # New function, determine name and type
            if i.address == header.ctor_offset:
                cur_func_name = known_symbols.get(i.address, "dll_{}_ctor".format(dll.number))
            elif i.address == header.dtor_offset:
                cur_func_name = known_symbols.get(i.address, "dll_{}_dtor".format(dll.number))
            else:
                cur_func_name = known_symbols.get(i.address, "dll_{}_func_{:X}".format(dll.number, i.address))
                cur_func_is_static = not i.address in header.export_offsets
            
            cur_func_addr = i.address
            cur_func_insts = []
            cur_func_has_gp_init = False
            cur_func_auto_syms = OrderedDict()
            cur_func_relocs = []
            cur_func_branch_dests = []
            cur_func_forward_branches = set()
            cur_func_inst_index = 0
            new_func = False
        
        # Pre-process instruction
        mnemonic = i.mnemonic
        op_str: str = i.op_str
        operands = [op.strip() for op in op_str.split(",")]
        num_operands = len(operands)
        ref: "str | None" = None
        has_relocation = False

        if __mnemonic_is_branch(mnemonic):
            # Replace branch address with label
            branch_target = int(operands[-1], 0)
            op_label = ".L{:X}".format(branch_target)
            op_str = ", ".join(operands[:-1] + [op_label])
            # Save target
            cur_func_branch_dests.append(branch_target)
            # If the branch target is ahead of this instruction, save it to assist in
            # detecting the function end
            if branch_target > i.address:
                cur_func_forward_branches.add(branch_target)
        elif cur_func_inst_index < 2 and num_operands > 0 and operands[0] == "$gp":
            # Add _gp_disp to $gp initializer stub
            # Note: The $gp initializer stub gets modified when compiled,
            # we need to convert it back to the original code
            cur_func_has_gp_init = True
            if cur_func_inst_index == 0:
                assert num_operands == 2
                op_str = ", ".join(operands[:-1] + [r"%hi(_gp_disp)"])
            elif cur_func_inst_index == 1:
                assert num_operands == 3
                op_str = ", ".join(operands[:-1] + [r"%lo(_gp_disp)"])
                # Additionally, change the %lo mnemonic from ori to addiu
                assert mnemonic == "ori"
                mnemonic = "addiu"
        elif cur_func_inst_index == 2 and cur_func_has_gp_init:
            # Change $gp initializer nop to 'addu gp,gp,t9'
            assert mnemonic == "nop"
            mnemonic = "addu"
            op_str = "$gp, $gp, $t9" 
        elif num_operands > 0 and operands[-1].endswith("($gp)"):
            # Replace offset($gp) with %got/call16(symbol)($gp) and add relocation entry
            # Lookup GOT symbol value referenced from the $gp addend
            gp_mem_op = operands[-1]
            offset = 0 if gp_mem_op == "($gp)" else int(gp_mem_op[:-5], 0)
            got_index = offset // 4
            symbol_addr = reloc_table.global_offset_table[got_index]
            # Determine if this is a CALL16 or GOT16 relocation
            is_call16 = __is_reloc_call16(idx, insts)
            # Make symbol
            if got_index == 0:
                symbol = ".text"
            elif got_index == 1:
                symbol = ".rodata"
            elif got_index == 2:
                symbol = ".data"
            elif got_index == 3:
                symbol = ".bss"
            else:
                if is_call16:
                    symbol = known_symbols.get(symbol_addr, "CALL_{:X}".format(symbol_addr))
                else:
                    symbol = known_symbols.get(symbol_addr, "GOT_{:X}".format(symbol_addr))
            if got_index >= 4:
                # Don't emit a symbol mapping for sections, they are implied
                cur_func_auto_syms[symbol] = symbol_addr
            ref = symbol
            # Modify operand
            if is_call16:
                op_str = ", ".join(operands[:-1]) + rf", %call16({symbol})($gp)"
            else:
                op_str = ", ".join(operands[:-1]) + rf", %got({symbol})($gp)"
            # Add relocation entry (unless the symbol is a section)
            # TODO: it would be nice to have actual entries for sections, but it's very tricky
            if got_index >= 4:
                cur_func_relocs.append(DLLRelocation(
                    offset=i.address,
                    type="R_MIPS_CALL16" if is_call16 else "R_MIPS_GOT16",
                    expression=symbol,
                    got_index=got_index
                ))
            has_relocation = True
        elif mnemonic == "move":
            # Replace with the actual instruction
            # TODO: make constants for some of these
            opcode = i.bytes[3] & 0b00111111
            op_str += ", $zero"

            if opcode == 37:
                mnemonic = "or"
            elif opcode == 45:
                mnemonic = "daddu"
            elif opcode == 33:
                mnemonic = "addu"
            else:
                raise NotImplementedError(f"INVALID INSTRUCTION {i} {opcode}")

        # Add instruction
        cur_func_insts.append(DLLInst(
            original=i,
            address=i.address,
            mnemonic=mnemonic,
            op_str=op_str,
            is_branch_delay_slot=is_delay_slot,
            ref=ref,
            has_relocation=has_relocation
        ))
        
        # If we reached a branch target, pop it
        if i.address in cur_func_forward_branches:
            cur_func_forward_branches.remove(i.address)
        
        # Check for function end
        if mnemonic == "jr" and i.op_str == "$ra" and len(cur_func_forward_branches) == 0:
            # Reached a jr $ra and we're not inside of a branch, must be the function end
            new_func = True

        # Track last instruction
        last_mnemonic = mnemonic
        cur_func_inst_index += 1
    
    # Add final function
    add_function()

    return funcs

def __mnemonic_has_delay_slot(mnemonic: str) -> bool:
    return (mnemonic.startswith("b") or mnemonic.startswith("j")) and mnemonic != "break"

def __mnemonic_is_branch(mnemonic: str) -> bool:
    return (mnemonic.startswith("b") or mnemonic == "j") and mnemonic != "break"

def __is_reloc_call16(idx: int, insts: "list[CsInsn]") -> bool:
    # GOT value must be stored in $t9
    if not insts[idx].op_str.startswith("$t9"):
        return False
    # Check if one of the next few instructions is jalr $t9
    for k in range(1, 10):
        if idx + k >= len(insts):
            break
        inst = insts[idx + k]
        if inst.op_str.startswith("$t9") and inst.op_str.endswith("($gp)"):
            # $t9 was changed, must not have been a call
            break
        if inst.mnemonic == "jalr" and inst.op_str == "$t9":
            return True
        
    return False
