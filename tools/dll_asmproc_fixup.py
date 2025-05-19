# Fixes up object files produced by asm-processor to:
# - Convert symbols to local bind that should have been local but aren't due to 
#   limitations with how asm-processor works.

from io import FileIO
from pathlib import Path
import re
import subprocess
import sys
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Section, SymbolTableSection, Symbol
from elftools.elf.relocation import RelocationSection, Relocation
from elftools.elf.enums import ENUM_RELOC_TYPE_MIPS
import os
from typing import TextIO
import shutil

EXPORT_PATTERN = re.compile(r"\s*\.dword\s*(\S+)")
VERBOSE = False

def fix_local_syms(syms: list[Symbol], rel_text_relocations: list[Relocation], exports: set[str]):
    # Find GOT16/LO16 relocations referencing defined symbols that we know should be local symbols.
    # These will only show up from GLOBAL_ASM.
    for reloc in rel_text_relocations:
        reloc_type = reloc["r_info_type"]
        
        if reloc_type != ENUM_RELOC_TYPE_MIPS["R_MIPS_GOT16"] and reloc_type != ENUM_RELOC_TYPE_MIPS["R_MIPS_LO16"]:
            continue
            
        sym_idx = reloc["r_info_sym"]
        sym = syms[sym_idx]
        assert isinstance(sym, Symbol)

        sym_shndx = sym.entry["st_shndx"]
        if sym_shndx == "SHN_UNDEF" or sym_shndx == "SHN_ABS":
            continue

        sym_type = sym.entry["st_info"]["type"]
        if sym_type == "STT_SECTION":
            continue

        # Assume any defined symbol that isn't a DLL export is local
        is_local_sym = not sym.name in exports
        if not is_local_sym:
            continue

        # Convert to local symbol
        sym.entry["st_info"]["bind"] = "STB_LOCAL"
        if sym_type == "STT_NOTYPE":
            sym.entry["st_info"]["type"] = "STT_OBJECT"

def fix_up(obj: ELFFile, out_obj: FileIO, exports: set[str]):
    # Load symbols
    symtab_idx = obj.get_section_index(".symtab")
    symtab = obj.get_section(symtab_idx)
    assert isinstance(symtab, SymbolTableSection)
    syms: list[Symbol] = [sym for sym in symtab.iter_symbols()]

    # Load relocation sections
    rel_sections: "dict[str, tuple[RelocationSection, list[Relocation]]]" = {}
    for section in obj.iter_sections():
        if isinstance(section, RelocationSection):
            rel_sections[section.name] = (section, [rel for rel in section.iter_relocations()])

    # Fix local symbols
    rel_text_tuple = rel_sections.get(".rel.text")
    if rel_text_tuple != None:
        (_, rel_text_relocations) = rel_text_tuple
        fix_local_syms(syms, rel_text_relocations, exports)
    
    # Re-sort symbols
    new_syms = syms.copy()
    new_syms.sort(key=lambda s: 0 if s.entry["st_info"]["bind"] == "STB_LOCAL" else 1)

    sym_to_new_idx: dict[Symbol, int] = {}
    last_local_sym = 0
    for i, sym in enumerate(new_syms):
        sym_to_new_idx[sym] = i
        if sym.entry["st_info"]["bind"] == "STB_LOCAL":
            last_local_sym = i

    # sh_info of .symtab is one plus the index of the last local symbol
    symtab.header["sh_info"] = last_local_sym + 1
    
    # Re-map symbol references
    for (section, relocations) in rel_sections.values():
        for reloc in relocations:
            sym = syms[reloc["r_info_sym"]]
            reloc.entry["r_info_sym"] = sym_to_new_idx[sym]

    # Write new relocations
    for (section, relocations) in rel_sections.values():
        out_obj.seek(section["sh_offset"], os.SEEK_SET)
        for reloc in relocations:
            reloc.entry["r_info"] = ((reloc.entry["r_info_sym"] & 0xFFFFFF) << 8) | (reloc.entry["r_info_type"] & 0xFF)
            section.entry_struct.build_stream(reloc.entry, out_obj)
    
    # Write new symtab
    out_obj.seek(obj["e_shoff"] + symtab_idx * obj["e_shentsize"], os.SEEK_SET)
    obj.structs.Elf_Shdr.build_stream(symtab.header, out_obj)

    out_obj.seek(symtab["sh_offset"], os.SEEK_SET)
    for sym in new_syms:
        obj.structs.Elf_Sym.build_stream(sym.entry, out_obj)

def read_exports_s(exports_s: TextIO) -> "set[str]":
    exports: "set[str]" = set()

    for line in exports_s.readlines():
        match = EXPORT_PATTERN.match(line)
        if match != None:
            sym_name = match.group(1)
            exports.add(sym_name)
    
    return exports

def main():
    # Split our args from the actual command
    args = sys.argv[1:]

    exports_s_path = args[0] # We expect the DLL's exports.s file as the first argument
    command_args = args[1:]

    # Run command
    retcode = subprocess.call(command_args)

    # Don't fix up output if the command failed
    if retcode != 0:
        sys.exit(retcode)

    # Get out file from args
    out_index = command_args.index("-o")
    out_file_path = Path(command_args[out_index + 1])

    # Copy output file so we can replace it while keeping the original
    copied_out_file_path = out_file_path.with_suffix(".beforefixup.o")
    shutil.copy(out_file_path, copied_out_file_path)

    # Do fix up
    with open(copied_out_file_path, "rb") as obj_file, \
         open(out_file_path, "r+b") as output_file, \
         open(exports_s_path, "r", encoding="utf-8") as exports_file:
        exports = read_exports_s(exports_file)
        
        with ELFFile(obj_file) as obj:
            fix_up(obj, output_file, exports)

if __name__ == "__main__":
    main()
