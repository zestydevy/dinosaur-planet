from .dll_analysis import DLLFunction, DLLInst, DLLInstRelocation, DLLInstRelocationType

def stringify_instruction(i: DLLInst, function: DLLFunction) -> "tuple[str, str | None]":
    label: str | None = None
    if i.is_branch_target or (function.jump_table_targets != None and i.i.vram in function.jump_table_targets):
        label = ".L{:X}:".format(i.i.vram)

    imm_override: str | None = None
    if i.relocation != None:
        imm_override = __reloc_to_syntax(i.relocation)
    elif i.is_branch:
        imm_override = ".L{:X}".format(i.i.getBranchVramGeneric())
    
    return i.i.disassemble(imm_override), label

def __reloc_to_syntax(reloc: DLLInstRelocation):
    match reloc.type:
        case DLLInstRelocationType.HI16:
            directive = r"%hi"
        case DLLInstRelocationType.LO16:
            directive = r"%lo"
        case DLLInstRelocationType.GOT16:
            directive = r"%got"
        case DLLInstRelocationType.CALL16:
            directive = r"%call16"
        case _:
            raise NotImplementedError()

    if reloc.offset != 0:
        return "{}({}+{:#x})".format(directive, reloc.symbol, reloc.offset)
    else:
        return "{}({})".format(directive, reloc.symbol)
