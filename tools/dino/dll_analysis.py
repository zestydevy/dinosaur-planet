from typing import TypedDict
from capstone import CS_ARCH_MIPS, CS_MODE_BIG_ENDIAN, CS_MODE_MIPS64, Cs, CsInsn
from capstone.mips import *
from capstone.mips_const import *
from enum import Enum
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

class DLLInstAlias(Enum):
    # move reg, $zero -> or reg, $zero, $zero
    ZeroMove_Or = 0
    # move reg, $zero -> addu reg, $zero, $zero
    ZeroMove_Addu = 1
    # move reg, $zero -> daddu reg, $zero, $zero
    ZeroMove_Daddu = 2

class DLLInstRelocationType(Enum):
    GOT16 = 0,
    LO16 = 1,
    CALL16 = 2

class DLLInstRelocation:
    def __init__(self,
                 type: DLLInstRelocationType,
                 symbol: str,
                 offset: int,
                 op_idx: int):
        self.type = type
        self.symbol = symbol
        self.offset = offset
        self.op_idx = op_idx

class DLLInst:
    is_branch_target: bool = False
    relocation: DLLInstRelocation | None = None

    def __init__(self,
                 i: CsInsn,
                 alias: DLLInstAlias | None,
                 is_branch: bool,
                 is_branch_delay_slot: bool) -> None:
        self.i = i
        self.alias = alias
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
                          analyze: bool = False):
    """Locates all functions in the DLL."""
    header = dll.header
    
    # Determine where in the file the .text section ends
    text_file_end = header.size + dll.get_text_size()

    # Vars
    new_func = True
    last_inst: "CsInsn | None" = None

    # Set up Capstone
    md = Cs(CS_ARCH_MIPS, CS_MODE_MIPS64 + CS_MODE_BIG_ENDIAN)
    md.detail = True

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
            return

        # Discard trailing nops
        for idx in range(len(cur_func_insts) - 1, 0, -1):
            i = cur_func_insts[idx]
            if i.i.id == MIPS_INS_NOP and not i.is_branch_delay_slot:
                cur_func_insts.pop(idx)
            else:
                break

        # Ensure function ends with jr $ra
        # Otherwise, it's not a function
        if len(cur_func_insts) >= 2:
            jr = cur_func_insts[-2] # -2 to account for the delay slot after jr
            if jr.i.id != MIPS_INS_JR or jr.i.operands[0].reg != MIPS_REG_RA:
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

    for i in md.disasm(dll_data[header.size:text_file_end], 0x0):
        # Check if this instruction is a branch delay slot of the previous instruction
        is_delay_slot = last_inst != None and \
            (last_inst.group(MIPS_GRP_BRANCH_RELATIVE) or last_inst.group(MIPS_GRP_JUMP) or last_inst.group(MIPS_GRP_CALL))
        
        if new_func and i.id != MIPS_INS_NOP and not is_delay_slot:
            # Add previous function
            add_function()
            
            # New function, determine name and type
            text_offset = i.address
            cur_func_name = symbols.get_func_name_or_default(i.address)
            if text_offset == header.ctor_offset:
                cur_func_type = DLLFunctionType.Ctor
            elif text_offset == header.dtor_offset:
                cur_func_type = DLLFunctionType.Dtor
            else:
                cur_func_type = DLLFunctionType.Export if text_offset in header.export_offsets else DLLFunctionType.Local
            
            cur_func_addr = i.address
            cur_func_insts = []
            cur_func_branch_dests = []
            cur_func_forward_branches = set()
            new_func = False
        
        is_branch = False
        if i.group(MIPS_GRP_BRANCH_RELATIVE) or i.id == MIPS_INS_J:
            is_branch = True
            # Save branch address
            branch_target = i.operands[-1].imm
            # Save target
            cur_func_branch_dests.append(branch_target)
            # If the branch target is ahead of this instruction, save it to assist in
            # detecting the function end
            if branch_target > i.address:
                cur_func_forward_branches.add(branch_target)
        
        # Resolve move instruction alias
        alias: DLLInstAlias | None = None
        if i.id == MIPS_INS_MOVE and i.operands[-1].reg == MIPS_REG_ZERO:
            opcode = i.bytes[3] & 0b00111111
            if opcode == 37:
                alias = DLLInstAlias.ZeroMove_Or
            elif opcode == 45:
                alias = DLLInstAlias.ZeroMove_Daddu
            elif opcode == 33:
                alias = DLLInstAlias.ZeroMove_Addu
        
        # Add instruction
        cur_func_insts.append(DLLInst(
            i=i,
            alias=alias,
            is_branch=is_branch,
            is_branch_delay_slot=is_delay_slot
        ))
        
        # If we reached a branch target, pop it
        if i.address in cur_func_forward_branches:
            cur_func_forward_branches.remove(i.address)
        
        # Check for function end
        if i.id == MIPS_INS_JR and i.operands[0].reg == MIPS_REG_RA and len(cur_func_forward_branches) == 0:
            # Reached a jr $ra and we're not inside of a branch, must be the function end
            new_func = True

        # Track last instruction
        last_inst = i
    
    # Add final function
    add_function()

    # Run full analysis for each function if requested
    if analyze:
        for func in funcs:
            analyze_dll_function(func, dll, dll_data, symbols)

    return funcs

class _GOTLoad(TypedDict):
    got_idx: int
    inst_idx: int

class _PossibleJumpTable(TypedDict):
    gp_lw_inst_idx: int             # lw reg, 0x4($gp)
    rodata_lw_inst_idx: int | None  # lw reg, disp(gp_lw_reg)
    addu_gp: bool                   # addu reg, reg, $gp
    rodata_offset: int | None

def analyze_dll_function(func: DLLFunction,
                         dll: DLL,
                         dll_data: bytes,
                         symbols: DLLSymbols):
    """Fully analyzes an individual DLL function."""
    insts = func.insts

    start_idx = 0

    # Check for Dino Planet flavored $gp prologue
    # lui $gp, 0
    # ori $gp, $gp, 0
    # nop
    if len(insts) >= 3:
        i1 = insts[0].i
        i2 = insts[1].i
        i3 = insts[2].i

        if i1.id == MIPS_INS_LUI and i1.operands[0].reg == MIPS_REG_GP and i1.operands[1].imm == 0 and \
           i2.id == MIPS_INS_ORI and i2.operands[0].reg == MIPS_REG_GP and i2.operands[1].reg == MIPS_REG_GP and i2.operands[2].imm == 0 and \
           i3.id == MIPS_INS_NOP:
            func.has_gp_prologue = True
            start_idx = 3
    
    # Reverse analyze relocations
    reg_to_got_loads: "dict[int, _GOTLoad]" = {}
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

        if i.id == MIPS_INS_LW and i.operands[1].mem.base == MIPS_REG_GP:
            # Found load from GOT, record reg so we can determine the relocation from use
            got_offset = i.operands[1].mem.disp
            assert got_offset % 4 == 0
            got_idx = got_offset // 4

            reg_to_got_loads[i.operands[0].reg] = { "got_idx": got_idx, "inst_idx": idx }
        else:
            if i.id == MIPS_INS_JALR:
                got_load = reg_to_got_loads.pop(i.operands[0].reg, None)

                if got_load != None:
                    # Extern symbol lookup, $call16
                    base_inst = insts[got_load["inst_idx"]]
                    got_entry = dll.reloc_table.global_offset_table[got_load["got_idx"]]
                    base_inst.relocation = DLLInstRelocation(
                        type=DLLInstRelocationType.CALL16,
                        symbol=symbols.get_global_name_or_default(got_entry),
                        offset=0,
                        op_idx=1
                    )
            elif i.id == MIPS_INS_JR:
                possible_jtbl = reg_to_possible_jtable.pop(i.operands[0].reg, None)
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
                            offset=0,
                            op_idx=1
                        )
                        lo_inst.relocation = DLLInstRelocation(
                            type=DLLInstRelocationType.LO16,
                            symbol=sym,
                            offset=0,
                            op_idx=1
                        )
                        jump_table_refs.add(jtbl_rodata_offset)
                        local_rodata_ref_info[jtbl_rodata_offset] = DLLLocalRefInfo(
                            load_type=DLLLocalRefLoadType.LW
                        )
                    else:
                        print("WARN: Unresolved possible jump table load @ {:#x}. Invalidated @ {:#x}"
                            .format(insts[possible_jtbl["gp_lw_inst_idx"]].i.address, i.address))
            else:
                for op in i.operands[1:]:
                    # Try to resolve jump table loads
                    possible_jtbl = reg_to_possible_jtable.pop(op.reg, None)
                    if possible_jtbl != None:
                        if i.id == MIPS_INS_LW:
                            if possible_jtbl["rodata_lw_inst_idx"] == None:
                                # Found lw reg, disp(gp_lw_reg)
                                possible_jtbl["rodata_lw_inst_idx"] = idx
                                possible_jtbl["rodata_offset"] = op.mem.disp
                                reg_to_possible_jtable[i.operands[0].reg] = possible_jtbl
                                break
                        elif i.id == MIPS_INS_ADDU:
                            last_op = i.operands[-1]
                            if last_op.type == MIPS_OP_REG and last_op.reg == MIPS_REG_GP and not possible_jtbl["addu_gp"]:
                                # Found addu reg, reg, $gp
                                possible_jtbl["addu_gp"] = True
                                reg_to_possible_jtable[i.operands[0].reg] = possible_jtbl
                                break
                        print("WARN: Unresolved possible jump table load @ {:#x}. Invalidated @ {:#x}"
                            .format(insts[possible_jtbl["gp_lw_inst_idx"]].i.address, i.address))

                    # Try to resolve GOT loads
                    got_load: _GOTLoad | None = None
                    if op.type == MIPS_OP_REG:
                        got_load = reg_to_got_loads.pop(op.reg, None)
                    elif op.type == MIPS_OP_MEM:
                        got_load = reg_to_got_loads.pop(op.mem.base, None)
                    
                    if got_load != None:
                        # Found next use of register holding a GOT value, determine relocation type
                        base_inst = insts[got_load["inst_idx"]]
                        got_idx = got_load["got_idx"]
                        if got_idx == 1 and op.type == MIPS_OP_REG and i.id == MIPS_INS_ADDU and \
                           i.operands[0].type == MIPS_OP_REG and i.operands[0].reg == op.reg:
                            # This is most likely a jump table lookup
                            reg_to_possible_jtable[op.reg] = { 
                                "gp_lw_inst_idx": got_load["inst_idx"],
                                "rodata_lw_inst_idx": None,
                                "addu_gp": False
                            }
                        elif got_idx <= 3:
                            # Local symbol lookup, %got with %lo pair
                            # Extract addend
                            last_op = i.operands[-1]
                            addend: int | None = None
                            if last_op.type == MIPS_OP_IMM:
                                addend = last_op.imm
                            elif last_op.type == MIPS_OP_MEM:
                                addend = last_op.mem.disp
                            else:
                                assert(False)
                            # Add local ref if possible
                            if got_idx == 1:
                                local_rodata_refs.add(addend)
                                __infer_ref_info(local_rodata_ref_info, addend, i)
                            elif got_idx == 2:
                                local_data_refs.add(addend)
                                __infer_ref_info(local_data_ref_info, addend, i)
                            elif got_idx == 3:
                                local_bss_refs.add(addend)
                                __infer_ref_info(local_bss_ref_info, addend, i)
                            # Add relocations
                            (sym_name, offset) = symbols.get_local_or_encapsulating(got_idx, addend)
                            base_inst.relocation = DLLInstRelocation(
                                type=DLLInstRelocationType.GOT16,
                                symbol=sym_name,
                                offset=0,
                                op_idx=1
                            )
                            inst.relocation = DLLInstRelocation(
                                type=DLLInstRelocationType.LO16,
                                symbol=sym_name,
                                offset=offset,
                                # The addend to add a reloc to should always be the last operand
                                op_idx=len(i.operands) - 1
                            )
                        else:
                            # Extern symbol lookup, %got
                            got_entry = dll.reloc_table.global_offset_table[got_idx]
                            base_inst.relocation = DLLInstRelocation(
                                type=DLLInstRelocationType.GOT16,
                                symbol=symbols.get_global_name_or_default(got_entry),
                                offset=0,
                                op_idx=1
                            )

                        break
            
            # Invalidate loads if register is overwritten
            if len(i.operands) > 0:
                got_load = reg_to_got_loads.pop(i.operands[0].reg, None)
                if got_load != None:
                    print("WARN: Unresolved GOT load @ {:#x}. Register overwritten @ {:#x}"
                        .format(insts[got_load["inst_idx"]].i.address, i.address))
            
        if i.group(MIPS_GRP_BRANCH_RELATIVE) or i.group(MIPS_GRP_JUMP):
            # Clear after delay slot
            clear_loads_next_inst = True
        elif clear_loads_next_inst:
            # Any loads after a branch are either unresolved or valid extern %got loads 
            clear_loads_next_inst = False
            for dst_reg in list(reg_to_got_loads.keys()):
                if dst_reg >= MIPS_REG_S0 and dst_reg <= MIPS_REG_S7:
                    # GOT loads into saved registers are an exception here as they stay valid across
                    # basic blocks. Our analysis isn't fancy so just let leave these alone. This should
                    # be fine in most cases...
                    continue

                got_load = reg_to_got_loads.pop(dst_reg)
                base_inst = insts[got_load["inst_idx"]]
                got_idx = got_load["got_idx"]

                if got_idx <= 3:
                    print("WARN: Unresolved GOT load @ {:#x}. Reached control flow before usage @ {:#x}"
                        .format(base_inst.i.address, i.address))
                else:
                    # Extern symbol lookup, %got
                    got_entry = dll.reloc_table.global_offset_table[got_idx]
                    base_inst.relocation = DLLInstRelocation(
                        type=DLLInstRelocationType.GOT16,
                        symbol=symbols.get_global_name_or_default(got_entry),
                        offset=0,
                        op_idx=1
                    )
            while len(reg_to_possible_jtable) > 0:
                (_, possible_jtbl) = reg_to_possible_jtable.popitem()
                base_inst = insts[possible_jtbl["gp_lw_inst_idx"]]
                print("WARN: Unresolved possible jump table load @ {:#x}. Reached control flow before usage @ {:#x}"
                    .format(base_inst.i.address, i.address))

    
    # Any leftover loads are either unresolved or valid extern %got loads 
    while len(reg_to_got_loads) > 0:
        (_, got_load) = reg_to_got_loads.popitem()
        base_inst = insts[got_load["inst_idx"]]
        got_idx = got_load["got_idx"]
        if got_idx <= 3:
            print("WARN: Unresolved GOT load @ {:#x}. Reached end of function before usage."
                .format(base_inst.i.address))
        else:
            # Extern symbol lookup, %got
            got_entry = dll.reloc_table.global_offset_table[got_idx]
            base_inst.relocation = DLLInstRelocation(
                type=DLLInstRelocationType.GOT16,
                symbol=symbols.get_global_name_or_default(got_entry),
                offset=0,
                op_idx=1
            )
    while len(reg_to_possible_jtable) > 0:
        (_, possible_jtbl) = reg_to_possible_jtable.popitem()
        base_inst = insts[possible_jtbl["gp_lw_inst_idx"]]
        print("WARN: Unresolved possible jump table load @ {:#x}. Reached end of function before usage."
            .format(base_inst.i.address))
    
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

def __infer_ref_info(local_ref_info: dict[int, DLLLocalRefInfo], ref: int, i: CsInsn) -> DLLLocalRefInfo:
    ref_info = local_ref_info.get(ref, None)
    if ref_info == None:
        ref_info = DLLLocalRefInfo(load_type=DLLLocalRefLoadType.UNKNOWN)
        local_ref_info[ref] = ref_info
    
    if i.id == MIPS_INS_LW:
        ref_info.load_type = DLLLocalRefLoadType.LW
    elif i.id == MIPS_INS_LWC1:
        ref_info.load_type = DLLLocalRefLoadType.LWC1
    elif i.id == MIPS_INS_LDC1:
        ref_info.load_type = DLLLocalRefLoadType.LDC1
