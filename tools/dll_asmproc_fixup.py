# Fixes up object files produced by asm-processor to:
# - Convert symbols to local bind that should have been local but aren't due to 
#   limitations with how asm-processor works.
# - Sort .rel.text relocations by function because asm-processor appended them
#   to the object file produced by the .c file (which breaks GOT order).
# - Sort .rel.text relocations such that they match the order of the original GOT,
#   since the assembler will not always emit relocations in the same order as IDO.

import bisect
from io import BufferedReader
import struct
import sys
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection, Symbol
from elftools.elf.relocation import RelocationSection, Relocation
import os
from typing import BinaryIO
import shutil

VERBOSE = False

def fix_rel_text_order(syms: list[Symbol], rel_text_relocations: list[Relocation], got: list[int]):
    got_index_map: dict[int, int] = {}
    for i, e in enumerate(got):
        got_index_map[e] = i

    # asm-processor appends relocations contributed by GLOBAL_ASM blocks, which breaks the order
    # that GOT entries are detected and thus results in a non-matching GOT.
    #
    # Group relocs by function (preserving reloc order within a function) and then sort the groups
    funcs: "list[int]" = []
    for sym in syms:
        sym_value = sym.entry["st_value"]
        sym_type = sym.entry["st_info"]["type"]

        if sym_type != "STT_FUNC" or (sym_value & 0x80000000) != 0:
            # Skip non-functions and imported functions
            continue

        bisect.insort(funcs, sym_value)
    
    relocs_by_func: "list[list[Relocation]]" = [[] for _ in range(len(funcs))]
    for reloc in rel_text_relocations:
        reloc_offset = reloc["r_offset"]
        func = bisect.bisect(funcs, reloc_offset) - 1
        relocs_by_func[func].append(reloc)
    
    i = 0
    for group in relocs_by_func:
        # Match sort of original GOT within each function
        pairs: list[tuple[int | None, Relocation]] = []
        got_relocs: list[tuple[int, Relocation]] = []

        # Extract relocs that point to a known GOT entry
        for reloc in group:
            sym = syms[reloc.entry["r_info_sym"]]
            sym_value = sym.entry["st_value"]
            got_index = got_index_map.get(sym_value)
            pairs.append((got_index, reloc))

            if got_index != None:
                got_relocs.append((got_index, reloc))

        # Sort known GOT relocs
        got_relocs.sort(key=lambda t: t[0])

        # Re-integrate sorted GOT relocs into original list
        k = 0
        local_sort: list[Relocation] = []
        for (got_index, reloc) in pairs:
            if got_index == None:
                local_sort.append(reloc)
            else:
                local_sort.append(got_relocs[k][1])
                k += 1

        # Update final reloc order
        for reloc in local_sort:
            rel_text_relocations[i] = reloc
            i += 1

def fix_local_syms(syms: list[Symbol], rel_exports_relocations: list[Relocation]):
    export_syms: set[int] = set()
    for reloc in rel_exports_relocations:
        export_syms.add(reloc["r_info_sym"])

    for idx, sym in enumerate(syms):
        sym_shndx = sym.entry["st_shndx"]
        if sym_shndx == "SHN_UNDEF" or sym_shndx == "SHN_ABS":
            continue

        sym_type = sym.entry["st_info"]["type"]
        if sym_type == "STT_SECTION":
            continue

        sym_bind = sym.entry["st_info"]["bind"]
        if sym_bind != "STB_GLOBAL":
            continue

        # Assume any defined symbol that isn't a DLL export is local
        if idx in export_syms:
            continue

        # Convert to local symbol
        sym.entry["st_info"]["bind"] = "STB_LOCAL"
        if sym_type == "STT_NOTYPE":
            sym.entry["st_info"]["type"] = "STT_OBJECT"

def read_got(dll: BufferedReader):
    got: list[int] = []

    dll.seek(0x8, os.SEEK_SET)
    rodata_offset = struct.unpack(">I", dll.read(4))[0]
    if rodata_offset == 0xFFFF_FFFF:
        return got
    
    dll.seek(rodata_offset, os.SEEK_SET)
    while True:
        word = struct.unpack(">I", dll.read(4))[0]
        if word == 0xFFFF_FFFE:
            break
        got.append(word)
    
    return got

def fix_up(obj: ELFFile, out_obj: BinaryIO, dll: BufferedReader):
    # Read original GOT
    got = read_got(dll)

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

    # Apply fixes
    rel_text_tuple = rel_sections.get(".rel.text")
    rel_exports_tuple = rel_sections.get(".rel.exports")
    if rel_text_tuple != None and rel_exports_tuple != None:
        (_, rel_text_relocations) = rel_text_tuple
        (_, rel_exports_relocations) = rel_exports_tuple
        # Convert syms to local that should have already been local
        fix_local_syms(syms, rel_exports_relocations)
        # Fix .rel.text relocation order due to asm-processor appending relocations
        fix_rel_text_order(syms, rel_text_relocations, got)
    
    # Re-sort symbols (local symbols must always be first and we modified symbol binds)
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

def main():
    # Read args
    args = sys.argv[1:]

    dll_path = args[0]
    in_path = args[1]
    out_path = args[2]

    # Clone input file so we can edit it
    shutil.copy(in_path, out_path)

    # Do fix up
    with open(in_path, "rb") as obj_file, \
         open(out_path, "r+b") as output_file, \
         open(dll_path, "rb") as dll_file:
        with ELFFile(obj_file) as obj:
            fix_up(obj, output_file, dll_file)

if __name__ == "__main__":
    main()
