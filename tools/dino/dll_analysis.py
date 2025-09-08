from typing import TypedDict
from enum import Enum
import rabbitizer
import struct

from .dll import DLL
from .dll_symbols import DLLSymbols

class DLLJumpTableEntry:
    def __init__(self,
                 value: int,
                 target: int):
        self.value = value
        self.target = target

class DLLJumpTable:
    def __init__(self,
                 address: int,
                 offset: int,
                 entries: list[DLLJumpTableEntry]):
        self.address = address
        self.offset = offset
        self.entries = entries

class DLLLocalRefLoadType(Enum):
    UNKNOWN = 0,
    LW = 1
    LWC1 = 2 # float
    LDC1 = 3 # double

class DLLLocalRefInfo:
    def __init__(self,
                 load_type: DLLLocalRefLoadType):
        self.load_type = load_type

class DLLInstRelocationType(Enum):
    HI16 = 0,
    LO16 = 1,
    GOT16 = 2,
    CALL16 = 3

class DLLInstRelocation:
    def __init__(self,
                 type: DLLInstRelocationType,
                 symbol: str,
                 offset: int):
        self.type = type
        self.symbol = symbol
        self.offset = offset

class DLLInst:
    is_branch_target: bool = False
    relocation: DLLInstRelocation | None = None

    def __init__(self,
                 i: rabbitizer.Instruction,
                 is_branch: bool,
                 is_branch_delay_slot: bool) -> None:
        self.i = i
        self.is_branch = is_branch
        self.is_branch_delay_slot = is_branch_delay_slot

class DLLFunctionType(Enum):
    Local = 0
    Export = 1,
    Ctor = 2,
    Dtor = 3

class DLLFunction:
    has_gp_prologue: bool | None = None
    local_rodata_refs: "set[int] | None" = None
    local_rodata_ref_info: "dict[int, DLLLocalRefInfo] | None" = None
    local_data_refs: "set[int] | None" = None
    local_data_ref_info: "dict[int, DLLLocalRefInfo] | None" = None
    local_bss_refs: "set[int] | None" = None
    local_bss_ref_info: "dict[int, DLLLocalRefInfo] | None" = None
    jump_table_refs: "set[int] | None" = None
    jump_tables: "dict[int, DLLJumpTable] | None" = None
    jump_table_targets: "set[int] | None" = None

    def __init__(self,
                 insts: "list[DLLInst]",
                 address: int,
                 symbol: str,
                 type: DLLFunctionType) -> None:
        self.insts = insts
        self.address = address
        self.symbol = symbol
        self.type = type

def get_all_dll_functions(dll_data: bytes, 
                          dll: DLL,
                          symbols: DLLSymbols,
                          analyze: bool = False,
                          bss_size: int | None=None):
    """Locates all functions in the DLL."""
    header = dll.header
    
    # Determine where in the file the .text section ends
    text_file_end = header.size + dll.get_text_size()

    # Vars
    new_func = True
    last_inst: "rabbitizer.Instruction | None" = None

    # Set up rabbitizer
    rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.O32
    rabbitizer.config.regNames_fprAbiNames = rabbitizer.Abi.O32
    rabbitizer.config.regNames_userFpcCsr = False
    rabbitizer.config.regNames_vr4300Cop0NamedRegisters = False
    rabbitizer.config.pseudos_pseudoMove = False
    rabbitizer.config.toolchainTweaks_treatJAsUnconditionalBranch = False
    rabbitizer.config.toolchainTweaks_gnuMode = True # Required for emitting matching asm

    # Extract functions
    funcs: "list[DLLFunction]" = []
    cur_func_insts: "list[DLLInst]" = []
    cur_func_name = ""
    cur_func_addr = 0
    cur_func_type = DLLFunctionType.Local
    cur_func_branch_dests: "list[int]" = []
    cur_func_forward_branches: "set[int]" = set()

    def add_function():
        if cur_func_name == "":
            # No functions were found
            return

        # Discard trailing nops
        for idx in range(len(cur_func_insts) - 1, 0, -1):
            i = cur_func_insts[idx]
            if i.i.uniqueId == rabbitizer.InstrId.cpu_nop and not i.is_branch_delay_slot:
                cur_func_insts.pop(idx)
            else:
                break

        # Ensure function ends with jr $ra
        # Otherwise, it's not a function
        if len(cur_func_insts) >= 2:
            jr = cur_func_insts[-2] # -2 to account for the delay slot after jr
            if not jr.i.isReturn():
                return

        # Record branch targets
        for addr in cur_func_branch_dests:
            idx = (addr - cur_func_addr) // 4
            if idx >= 0 and idx < len(cur_func_insts):
                cur_func_insts[idx].is_branch_target = True

        # Add function
        funcs.append(DLLFunction(
            insts=cur_func_insts,
            address=cur_func_addr,
            symbol=cur_func_name,
            type=cur_func_type
        ))

    rom = header.size
    vram = 0x0
    while rom < text_file_end:
        # Decode next instruction
        word = struct.unpack_from(">I", dll_data, rom)[0]
        i = rabbitizer.Instruction(word, vram, category=rabbitizer.InstrCategory.CPU)

        if not i.isValid():
            print("ERROR: Invalid instruction @ {:#x}. Stopping.".format(rom))
            exit(1)
            break

        # Check if this instruction is a branch delay slot of the previous instruction
        is_delay_slot = last_inst != None and last_inst.hasDelaySlot()
         
        if new_func and i.uniqueId != rabbitizer.InstrId.cpu_nop and not is_delay_slot:
            # Add previous function
            add_function()
            
            # New function, determine name and type
            text_offset = i.vram
            cur_func_name = symbols.get_func_name_or_default(i.vram)
            if text_offset == header.ctor_offset:
                cur_func_type = DLLFunctionType.Ctor
            elif text_offset == header.dtor_offset:
                cur_func_type = DLLFunctionType.Dtor
            else:
                cur_func_type = DLLFunctionType.Export if text_offset in header.export_offsets else DLLFunctionType.Local
            
            cur_func_addr = i.vram
            cur_func_insts = []
            cur_func_branch_dests = []
            cur_func_forward_branches = set()
            new_func = False
        
        is_branch = False
        if i.isBranch():
            is_branch = True
            # Save branch address
            branch_target = i.getBranchVramGeneric()
            # Save target
            cur_func_branch_dests.append(branch_target)
            # If the branch target is ahead of this instruction, save it to assist in
            # detecting the function end
            if branch_target > i.vram:
                cur_func_forward_branches.add(branch_target)
        
        # Add instruction
        cur_func_insts.append(DLLInst(
            i=i,
            is_branch=is_branch,
            is_branch_delay_slot=is_delay_slot
        ))
        
        # If we reached a branch target, pop it
        if i.vram in cur_func_forward_branches:
            cur_func_forward_branches.remove(i.vram)
        
        # Check for function end
        if i.isReturn() and len(cur_func_forward_branches) == 0:
            # Reached a jr $ra and we're not inside of a branch, must be the function end
            new_func = True

        # Track last instruction
        last_inst = i
        
        # Next
        rom += 4
        vram += 4
    
    # Add final function
    add_function()

    # Run full analysis for each function if requested
    if analyze:
        for func in funcs:
            analyze_dll_function(func, dll, dll_data, symbols, bss_size)

    return funcs

class _GOTLoad(TypedDict):
    got_idx: int
    inst_idx: int

class _PossibleJumpTable(TypedDict):
    gp_lw_inst_idx: int             # lw reg, 0x4($gp)
    rodata_lw_inst_idx: int | None  # lw reg, disp(gp_lw_reg)
    addu_gp: bool                   # addu reg, reg, $gp
    rodata_offset: int | None

__SAVED_REGISTERS = set([
    rabbitizer.RegGprO32.s0, rabbitizer.RegGprO32.s1, rabbitizer.RegGprO32.s2, rabbitizer.RegGprO32.s3,
    rabbitizer.RegGprO32.s4, rabbitizer.RegGprO32.s5, rabbitizer.RegGprO32.s6, rabbitizer.RegGprO32.s7
])

def analyze_dll_function(func: DLLFunction,
                         dll: DLL,
                         dll_data: bytes,
                         symbols: DLLSymbols,
                         bss_size: int | None=None):
    """Fully analyzes an individual DLL function."""
    insts = func.insts

    data_size = dll.get_data_size()

    start_idx = 0

    # Check for Dino Planet flavored $gp prologue
    # lui $gp, 0
    # ori $gp, $gp, 0
    # nop
    if len(insts) >= 3:
        i1 = insts[0].i
        i2 = insts[1].i
        i3 = insts[2].i

        if i1.uniqueId == rabbitizer.InstrId.cpu_lui and i1.rt == rabbitizer.RegGprO32.gp and i1.getProcessedImmediate() == 0 and \
           i2.uniqueId == rabbitizer.InstrId.cpu_ori and i2.rt == rabbitizer.RegGprO32.gp and i2.rs == rabbitizer.RegGprO32.gp and i2.getProcessedImmediate() == 0 and \
           i3.uniqueId == rabbitizer.InstrId.cpu_nop:
            # Replace with standard MIPS $gp prologue
            func.has_gp_prologue = True
            # lui     gp,0x0
            insts[0].i = rabbitizer.Instruction(0x3C1C0000, i1.vram, category=rabbitizer.InstrCategory.CPU)
            insts[0].relocation = DLLInstRelocation(DLLInstRelocationType.HI16, "_gp_disp", 0)
            # addiu   gp,gp,0x0
            insts[1].i = rabbitizer.Instruction(0x279C0000, i2.vram, category=rabbitizer.InstrCategory.CPU)
            insts[1].relocation = DLLInstRelocation(DLLInstRelocationType.LO16, "_gp_disp", 0)
            # addu    gp,gp,t9
            insts[2].i = rabbitizer.Instruction(0x0399E021, i3.vram, category=rabbitizer.InstrCategory.CPU)
            start_idx = 3
    
    # Reverse analyze relocations
    reg_to_got_loads: "dict[rabbitizer.Enum, _GOTLoad]" = {}
    reg_to_possible_jtable: "dict[int, _PossibleJumpTable]" = {}
    clear_loads_next_inst = False
    local_rodata_refs: "set[int]" = set()
    local_data_refs: "set[int]" = set()
    local_bss_refs: "set[int]" = set()
    jump_table_refs: "set[int]" = set()
    local_rodata_ref_info: "dict[int, DLLLocalRefInfo]" = {}
    local_data_ref_info: "dict[int, DLLLocalRefInfo]" = {}
    local_bss_ref_info: "dict[int, DLLLocalRefInfo]" = {}

    for idx, inst in enumerate(insts[start_idx:], start_idx):
        i = inst.i
        new_got_load = False

        if i.uniqueId == rabbitizer.InstrId.cpu_lw and i.rs == rabbitizer.RegGprO32.gp:
            # Found load from GOT, record reg so we can determine the relocation from use
            # lw rt, imm(rs)
            got_offset = i.getProcessedImmediate()
            assert got_offset % 4 == 0
            got_idx = got_offset // 4

            existing_got_load = reg_to_got_loads.pop(i.rt, None)
            if existing_got_load != None:
                print("WARN: Unresolved GOT load @ {:#x}. Register overwritten @ {:#x}"
                    .format(insts[existing_got_load["inst_idx"]].i.vram, i.vram))

            reg_to_got_loads[i.rt] = { "got_idx": got_idx, "inst_idx": idx }
            new_got_load = True
        elif i.uniqueId == rabbitizer.InstrId.cpu_jalr:
            got_load = reg_to_got_loads.pop(i.rs, None)
            if got_load != None:
                # Extern symbol lookup, $call16
                base_inst = insts[got_load["inst_idx"]]
                got_entry = dll.reloc_table.global_offset_table[got_load["got_idx"]]
                base_inst.relocation = DLLInstRelocation(
                    type=DLLInstRelocationType.CALL16,
                    symbol=symbols.get_global_name_or_default(got_entry),
                    offset=0
                )
        elif i.uniqueId == rabbitizer.InstrId.cpu_jr:
            possible_jtbl = reg_to_possible_jtable.pop(i.rs, None)
            if possible_jtbl != None:
                if possible_jtbl["rodata_lw_inst_idx"] != None and possible_jtbl["addu_gp"]:
                    # Found jr to jump table
                    jtbl_rodata_offset = possible_jtbl["rodata_offset"]
                    sym = symbols.get_jtable_name_or_default(jtbl_rodata_offset)

                    hi_inst = insts[possible_jtbl["gp_lw_inst_idx"]]
                    lo_inst = insts[possible_jtbl["rodata_lw_inst_idx"]]
                    hi_inst.relocation = DLLInstRelocation(
                        type=DLLInstRelocationType.GOT16,
                        symbol=sym,
                        offset=0
                    )
                    lo_inst.relocation = DLLInstRelocation(
                        type=DLLInstRelocationType.LO16,
                        symbol=sym,
                        offset=0
                    )
                    jump_table_refs.add(jtbl_rodata_offset)
                    local_rodata_ref_info[jtbl_rodata_offset] = DLLLocalRefInfo(
                        load_type=DLLLocalRefLoadType.LW
                    )
                else:
                    print("WARN: Unresolved possible jump table load @ {:#x}. Invalidated @ {:#x}"
                        .format(insts[possible_jtbl["gp_lw_inst_idx"]].i.vram, i.vram))
        else:
            # Try to resolve jump table loads
            handled_jtable = False
            possible_jtbl: _PossibleJumpTable | None = None
            if i.uniqueId == rabbitizer.InstrId.cpu_lw:
                # lw rt, imm(rs)
                possible_jtbl = reg_to_possible_jtable.pop(i.rs, None)
                if possible_jtbl != None and possible_jtbl["rodata_lw_inst_idx"] == None:
                    # Found lw reg, disp(gp_lw_reg)
                    possible_jtbl["rodata_lw_inst_idx"] = idx
                    possible_jtbl["rodata_offset"] = i.getProcessedImmediate()
                    reg_to_possible_jtable[i.rt] = possible_jtbl
                    handled_jtable = True
            elif i.uniqueId == rabbitizer.InstrId.cpu_addu:
                # addu rd, rs, rt
                possible_jtbl = reg_to_possible_jtable.pop(i.rs, None)
                if possible_jtbl != None and i.rt == rabbitizer.RegGprO32.gp and not possible_jtbl["addu_gp"]:
                    # Found addu reg, reg, $gp
                    possible_jtbl["addu_gp"] = True
                    reg_to_possible_jtable[i.rd] = possible_jtbl
                    handled_jtable = True

            if possible_jtbl != None and not handled_jtable:
                print("WARN: Unresolved possible jump table load @ {:#x}. Invalidated @ {:#x}"
                    .format(insts[possible_jtbl["gp_lw_inst_idx"]].i.vram, i.vram))

            if not handled_jtable:
                for reg in __get_read_registers(i):
                    possible_jtbl = reg_to_possible_jtable.pop(reg, None)
                    if possible_jtbl != None:
                        print("WARN: Unresolved possible jump table load @ {:#x}. Invalidated @ {:#x}"
                            .format(insts[possible_jtbl["gp_lw_inst_idx"]].i.vram, i.vram))

                    # Try to resolve GOT loads
                    got_load = reg_to_got_loads.pop(reg, None)
                    if got_load == None:
                        continue
                    
                    # Found next use of register holding a GOT value, determine relocation type
                    base_inst = insts[got_load["inst_idx"]]
                    got_idx = got_load["got_idx"]
                    # addu rd, rs, rt
                    if got_idx == 1 and i.uniqueId == rabbitizer.InstrId.cpu_addu and i.getDestinationGpr() == reg:
                        # This is most likely a jump table lookup
                        reg_to_possible_jtable[reg] = { 
                            "gp_lw_inst_idx": got_load["inst_idx"],
                            "rodata_lw_inst_idx": None,
                            "addu_gp": False
                        }
                    else:
                        got_entry = dll.reloc_table.global_offset_table[got_idx]
                        # Check if local symbol offset was too big and got split across the hi/lo pair
                        # addiu rt, rs, imm
                        split_local = (got_entry & 0x8000_0000) == 0 and got_entry >= 0x1_0000 and (got_entry & 0xFFFF) == 0 \
                                and i.uniqueId == rabbitizer.InstrId.cpu_addiu and i.rt == i.rs
                        if got_idx <= 3 or split_local:
                            # Local symbol lookup, %got with %lo pair
                            # Extract addend
                            addend = i.getProcessedImmediate()
                            # Determine section and symbol offset
                            if split_local:
                                (section, sym_offset) = symbols.convert_absolute_to_relative_address(got_entry + addend)
                            else:
                                section = got_idx
                                sym_offset = addend
                            # Add local ref if possible
                            misalignment = 0
                            ref_inside_section = True # A ref may be the end of the section, which we don't want a symbol for
                            if section == 1:
                                local_rodata_refs.add(sym_offset)
                                __infer_ref_info(local_rodata_ref_info, sym_offset, i)
                            elif section == 2:
                                # .data symbols will always be 4-byte aligned
                                if sym_offset < data_size:
                                    if sym_offset % 4 == 0:
                                        local_data_refs.add(sym_offset)
                                        __infer_ref_info(local_data_ref_info, sym_offset, i)
                                    else:
                                        misalignment = sym_offset % 4
                                        sym_offset -= misalignment
                                        local_data_refs.add(sym_offset)
                                        __default_ref_info(local_data_ref_info, sym_offset)
                                else:
                                    ref_inside_section = False
                            elif section == 3:
                                if bss_size == None or sym_offset < bss_size:
                                    local_bss_refs.add(sym_offset)
                                    __infer_ref_info(local_bss_ref_info, sym_offset, i)
                                else:
                                    ref_inside_section = False
                            # Add relocations
                            if ref_inside_section:
                                (sym_name, offset) = symbols.get_local_or_encapsulating(section, sym_offset)
                                base_inst.relocation = DLLInstRelocation(
                                    type=DLLInstRelocationType.GOT16,
                                    symbol=sym_name,
                                    offset=0
                                )
                                inst.relocation = DLLInstRelocation(
                                    type=DLLInstRelocationType.LO16,
                                    symbol=sym_name,
                                    offset=offset + misalignment
                                )
                        else:
                            # Extern symbol lookup, %got
                            base_inst.relocation = DLLInstRelocation(
                                type=DLLInstRelocationType.GOT16,
                                symbol=symbols.get_global_name_or_default(got_entry),
                                offset=0
                            )

                    break
            
        if not new_got_load:
            # Invalidate loads if register is overwritten
            # Note: Register is only overwritten if that instruction loads to operand 0
            dst_reg = i.getDestinationGpr()
            if dst_reg != None:
                got_load = reg_to_got_loads.pop(dst_reg, None)
                if got_load != None:
                    print("WARN: Unresolved GOT load @ {:#x}. Register overwritten @ {:#x}"
                        .format(insts[got_load["inst_idx"]].i.vram, i.vram))
            
            
        if i.hasDelaySlot():
            # Clear after delay slot
            clear_loads_next_inst = True
        elif clear_loads_next_inst:
            # Any loads after a branch are either unresolved or valid extern %got loads 
            clear_loads_next_inst = False
            for dst_reg in list(reg_to_got_loads.keys()):
                if dst_reg in __SAVED_REGISTERS:
                    # GOT loads into saved registers are an exception here as they stay valid across
                    # basic blocks. Our analysis isn't fancy so just let leave these alone. This should
                    # be fine in most cases...
                    continue

                got_load = reg_to_got_loads.pop(dst_reg)
                base_inst = insts[got_load["inst_idx"]]
                got_idx = got_load["got_idx"]

                if got_idx <= 3:
                    print("WARN: Unresolved GOT load @ {:#x}. Reached control flow before usage @ {:#x}"
                        .format(base_inst.i.vram, i.vram))
                else:
                    # Extern symbol lookup, %got
                    got_entry = dll.reloc_table.global_offset_table[got_idx]
                    base_inst.relocation = DLLInstRelocation(
                        type=DLLInstRelocationType.GOT16,
                        symbol=symbols.get_global_name_or_default(got_entry),
                        offset=0
                    )
            while len(reg_to_possible_jtable) > 0:
                (_, possible_jtbl) = reg_to_possible_jtable.popitem()
                base_inst = insts[possible_jtbl["gp_lw_inst_idx"]]
                print("WARN: Unresolved possible jump table load @ {:#x}. Reached control flow before usage @ {:#x}"
                    .format(base_inst.i.vram, i.vram))

    
    # Any leftover loads are either unresolved or valid extern %got loads 
    while len(reg_to_got_loads) > 0:
        (_, got_load) = reg_to_got_loads.popitem()
        base_inst = insts[got_load["inst_idx"]]
        got_idx = got_load["got_idx"]
        if got_idx <= 3:
            print("WARN: Unresolved GOT load @ {:#x}. Reached end of function before usage."
                .format(base_inst.i.vram))
        else:
            # Extern symbol lookup, %got
            got_entry = dll.reloc_table.global_offset_table[got_idx]
            base_inst.relocation = DLLInstRelocation(
                type=DLLInstRelocationType.GOT16,
                symbol=symbols.get_global_name_or_default(got_entry),
                offset=0
            )
    while len(reg_to_possible_jtable) > 0:
        (_, possible_jtbl) = reg_to_possible_jtable.popitem()
        base_inst = insts[possible_jtbl["gp_lw_inst_idx"]]
        print("WARN: Unresolved possible jump table load @ {:#x}. Reached end of function before usage."
            .format(base_inst.i.vram))
    
    # Attach found local syms
    func.local_rodata_refs = local_rodata_refs
    func.local_rodata_ref_info = local_rodata_ref_info
    func.local_data_refs = local_data_refs
    func.local_data_ref_info = local_data_ref_info
    func.local_bss_refs = local_bss_refs
    func.local_bss_ref_info = local_bss_ref_info
    func.jump_table_refs = jump_table_refs

    # Analyze jump tables
    __analyze_dll_function_jump_tables(func, dll, dll_data)

def __get_read_registers(i: rabbitizer.Instruction):
    regs = []

    if i.readsRd():
        regs.append(i.rd)
    if i.readsRs():
        regs.append(i.rs)
    if i.readsRt():
        regs.append(i.rt)

    if i.readsFd():
        regs.append(i.fd)
    if i.readsFs():
        regs.append(i.fs)
    if i.readsFt():
        regs.append(i.ft)

    return regs

def __analyze_dll_function_jump_tables(func: DLLFunction,
                                       dll: DLL,
                                       dll_data: bytes):
    assert(func.jump_table_refs != None)
    if not dll.has_rodata():
        return
    
    jump_tables: dict[int, DLLJumpTable] = {}
    targets: set[int] = set()

    func_start = func.address
    func_end = func.address + len(func.insts) * 4

    rodata_start = dll.header.rodata_offset + dll.reloc_table.get_size() # exclude relocation tables
    rodata_end = rodata_start + dll.get_rodata_size()
    rodata = dll_data[rodata_start:rodata_end]
    rodata_size = rodata_end - rodata_start

    for ref in func.jump_table_refs:
        entries: list[DLLJumpTableEntry] = []
        offset = ref
        while offset < rodata_size and \
              (offset == ref or not offset in func.jump_table_refs) and \
              not offset in func.local_rodata_refs:
            value = struct.unpack_from(">i", rodata, offset)[0]
            target = dll.header.rodata_offset - dll.header.size + value
            if target < func_start or target >= func_end:
                break
            entries.append(DLLJumpTableEntry(value, target))
            targets.add(target)
            offset += 4
        
        jump_tables[ref] = DLLJumpTable(rodata_start - dll.header.size + ref, ref, entries)

    func.jump_tables = jump_tables
    func.jump_table_targets = targets

def __default_ref_info(local_ref_info: dict[int, DLLLocalRefInfo], ref: int) -> DLLLocalRefInfo:
    if not ref in local_ref_info:
        local_ref_info[ref] = DLLLocalRefInfo(load_type=DLLLocalRefLoadType.UNKNOWN)

def __infer_ref_info(local_ref_info: dict[int, DLLLocalRefInfo], ref: int, i: rabbitizer.Instruction) -> DLLLocalRefInfo:
    ref_info = local_ref_info.get(ref, None)
    if ref_info == None:
        ref_info = DLLLocalRefInfo(load_type=DLLLocalRefLoadType.UNKNOWN)
        local_ref_info[ref] = ref_info
    
    if i.uniqueId == rabbitizer.InstrId.cpu_lw:
        ref_info.load_type = DLLLocalRefLoadType.LW
    elif i.uniqueId == rabbitizer.InstrId.cpu_lwc1:
        ref_info.load_type = DLLLocalRefLoadType.LWC1
    elif i.uniqueId == rabbitizer.InstrId.cpu_ldc1:
        ref_info.load_type = DLLLocalRefLoadType.LDC1
