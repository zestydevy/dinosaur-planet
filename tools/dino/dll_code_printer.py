from capstone.mips_const import *

from .dll_analysis import DLLFunction, DLLInst, DLLInstAlias, DLLInstRelocation, DLLInstRelocationType

def stringify_instruction(idx: int, i: DLLInst, function: DLLFunction) -> "tuple[str, str | None]":
    if function.has_gp_prologue == True:
        if idx == 0:
            return "{:<11}{}".format("lui", r"$gp, %hi(_gp_disp)"), None
        elif idx == 1:
            return "{:<11}{}".format("addiu", r"$gp, $gp, %lo(_gp_disp)"), None
        elif idx == 2:
            return "{:<11}{}".format("addu", r"$gp, $gp, $t9"), None

    label: str | None = None
    if i.is_branch_target or (function.jump_table_targets != None and i.i.address in function.jump_table_targets):
        label = ".L{:X}:".format(i.i.address)

    mnemonic = i.i.mnemonic
    operands: "list[str]" = []

    reloc = i.relocation

    op_count = len(i.i.operands)
    for op_idx, op in enumerate(i.i.operands):
        if op.type == MIPS_OP_IMM:
            # imm
            if reloc != None and op_idx == reloc.op_idx:
                operands.append(__reloc_to_syntax(reloc))
            elif i.is_branch and op_idx == (op_count - 1):
                operands.append(".L{:X}".format(op.imm))
            else:
                operands.append("{:#x}".format(op.imm))
        elif op.type == MIPS_OP_REG:
            # reg
            if reloc != None and op_idx == reloc.op_idx:
                print(function.symbol)
                print(reloc.op_idx, reloc.type)
                print(mnemonic, i.i.op_str)
                raise ValueError() # should be impossible
            operands.append("${}".format(i.i.reg_name(op.reg)))
        elif op.type == MIPS_OP_MEM:
            # mem
            if reloc != None and op_idx == reloc.op_idx:
                operands.append("{}(${})"
                    .format(__reloc_to_syntax(reloc), i.i.reg_name(op.mem.base)))
            else:
                operands.append("{:#x}(${})".format(op.mem.disp, i.i.reg_name(op.mem.base)))
        else:
            raise NotImplementedError()
    
    match i.alias:
        case DLLInstAlias.ZeroMove_Or:
            mnemonic = "or"
            operands.append("$zero")
        case DLLInstAlias.ZeroMove_Addu:
            mnemonic = "addu"
            operands.append("$zero")
        case DLLInstAlias.ZeroMove_Daddu:
            mnemonic = "daddu"
            operands.append("$zero")
    
    if i.i.id == MIPS_INS_CFC1 or i.i.id == MIPS_INS_CTC1:
        # For some reason, the assembler only allows cfc1 and ctc1 to reference the right-hand
        # register with its numeric name and not $ra (which is what Capstone gives us)
        assert operands[-1] == "$ra"
        operands[-1] = "$31"
    elif i.i.id == MIPS_INS_DIV and op_count < 3:
        # Capstone omits the first $zero operand sometimes, we need it back for the assembler
        operands.insert(0, "$zero") 
    
    if i.is_branch_delay_slot:
        mnemonic = " " + mnemonic

    op_str = ", ".join(operands)

    return "{:<11}{}".format(mnemonic, op_str), label

def __reloc_to_syntax(reloc: DLLInstRelocation):
    match reloc.type:
        case DLLInstRelocationType.GOT16:
            directive = r"%got"
        case DLLInstRelocationType.LO16:
            directive = r"%lo"
        case DLLInstRelocationType.CALL16:
            directive = r"%call16"
        case _:
            raise NotImplementedError()

    if reloc.offset != 0:
        return "{}({}+{:#x})".format(directive, reloc.symbol, reloc.offset)
    else:
        return "{}({})".format(directive, reloc.symbol)
