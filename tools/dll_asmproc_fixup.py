# Fixes up object files produced by asm-processor to:
# - Patch GOT16/LO16 relocs that should have been for local symbols but couldn't due to 
#   that code coming from single-function asm.

from io import FileIO
from pathlib import Path
import re
import subprocess
import sys
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Section, SymbolTableSection
from elftools.elf.relocation import RelocationSection
from elftools.elf.enums import ENUM_RELOC_TYPE_MIPS
import os
from typing import TextIO
import shutil
import struct

EXPORT_PATTERN = re.compile(r"\s*\.dword\s*(\S+)")
VERBOSE = False

def fix_up(obj: ELFFile, out_obj: FileIO, exports: set[str]):
    syms = obj.get_section_by_name(".symtab")
    assert isinstance(syms, SymbolTableSection)

    # Note: Ideally, for correctness, we convert symbols that should be local bind but aren't to local,
    # but this is very non-trivial. We'd have to re-sort symbols (local binds must come first) and then
    # update all other parts of the ELF with the updated symbol indexes... Luckily, we don't need these
    # binds to be correct for decomp purposes. After this script runs, no relocs will be referencing
    # these incorrectly bound symbols so they're harmless to leave as is.

    # Map section indexes to their symbol indexes
    section_sym_idxs: dict[int, int] = {}
    i = 0
    found_section_syms = 0
    for sym in syms.iter_symbols():
        if sym.name == ".text":
            section_sym_idxs[obj.get_section_index(".text")] = i
            found_section_syms += 1
        elif sym.name == ".rodata":
            section_sym_idxs[obj.get_section_index(".rodata")] = i
            found_section_syms += 1
        elif sym.name == ".data":
            section_sym_idxs[obj.get_section_index(".data")] = i
            found_section_syms += 1
        elif sym.name == ".bss":
            section_sym_idxs[obj.get_section_index(".bss")] = i
            found_section_syms += 1
        if found_section_syms == 4:
            # Usually the first few symbols are the sections, we can take advantage of that
            # and bail out early to avoid parsing other symbols we don't need to look at
            break
        i += 1

    text = obj.get_section_by_name(".text")
    assert isinstance(text, Section)

    rel_text = obj.get_section_by_name(".rel.text")
    if rel_text != None:
        assert isinstance(rel_text, RelocationSection)
        for i in range(rel_text.num_relocations()):
            reloc = rel_text.get_relocation(i)
            reloc_offset = reloc["r_offset"]
            reloc_type = reloc["r_info_type"]

            # Find GOT16/LO16 relocations referencing defined symbols that we know should be local symbols.
            # These will only show up from GLOBAL_ASM.
            if reloc_type != ENUM_RELOC_TYPE_MIPS["R_MIPS_GOT16"] and reloc_type != ENUM_RELOC_TYPE_MIPS["R_MIPS_LO16"]:
                continue
                
            sym_idx = reloc["r_info_sym"]
            sym = syms.get_symbol(sym_idx)

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

            sym_value = sym.entry["st_value"]

            section_sym_idx = section_sym_idxs.get(sym_shndx, None)
            assert(section_sym_idx != None)

            if reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_GOT16"]:
                reloc.entry["r_info_sym"] = section_sym_idx

                # Note: No need to patch instruction here, elf2dll will take care of that.

                if VERBOSE:
                    print("Patched GOT16 {} @ {:#x} to {}".format(i, reloc_offset, sym.name))
            elif reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_LO16"]:
                reloc.entry["r_info_sym"] = section_sym_idx
                
                # Patch LO16 addend to what it should have been
                out_obj.seek(text['sh_offset'] + reloc_offset + 2, os.SEEK_SET)
                existing_addend = struct.unpack(">H", out_obj.read(2))[0]
                out_obj.seek(-2, os.SEEK_CUR)
                out_obj.write(struct.pack(">H", sym_value + existing_addend))

                if VERBOSE:
                    print("Patched LO16 {} @ {:#x} to {}({:#x})+{:#x}".format(i, reloc_offset, sym.name, sym_value, existing_addend))
            else:
                raise NotImplementedError()

            # Patch reloc in new obj file
            reloc.entry["r_info"] = ((reloc.entry["r_info_sym"] & 0xFFFFFF) << 8) | (reloc.entry["r_info_type"] & 0xFF)
            out_obj.seek(rel_text['sh_offset'] + i * rel_text.entry_size, os.SEEK_SET)
            rel_text.entry_struct.build_stream(reloc.entry, out_obj)
    pass

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
