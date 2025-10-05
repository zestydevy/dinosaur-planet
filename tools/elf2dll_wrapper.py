#!/usr/bin/env python3
import argparse
from io import BufferedReader
import struct
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Symbol
from elftools.elf.relocation import RelocationSection
import os
from typing import BinaryIO

import elf2dll

# Read GOT from a Dino DLL
def read_got(dll: BufferedReader | BinaryIO):
    dll.seek(0x8, os.SEEK_SET)
    rodata_offset = struct.unpack(">I", dll.read(4))[0]
    if rodata_offset == 0xFFFF_FFFF:
        return None
    
    got: list[int] = []
    dll.seek(rodata_offset, os.SEEK_SET)
    while True:
        word = struct.unpack(">I", dll.read(4))[0]
        if word == 0xFFFF_FFFE:
            break
        got.append(word)
    
    return got

# Read export sym indices from an ELF
def read_exports(obj: ELFFile):
    export_syms: set[int] = set()
    
    rel_exports = obj.get_section_by_name(".rel.exports")
    if rel_exports != None:
        assert isinstance(rel_exports, RelocationSection)
        for reloc in rel_exports.iter_relocations():
            export_syms.add(reloc["r_info_sym"])

    return export_syms

# Symbols referenced by GLOBAL_ASM code that are not defined in the same file will always be external
# to that code, however these are very often static variables and as such we need the symbol to be
# local bind before running elf2dll. We can infer which symbols should be local by comparing them
# to the export table, which defines public .text symbols. Technically this doesn't support public
# datasyms at the moment.
def sym_bind_hack(idx: int, sym: Symbol, export_syms: set[int]):
    sym_bind = sym.entry["st_info"]["bind"]

    sym_shndx = sym.entry["st_shndx"]
    if sym_shndx == "SHN_UNDEF" or sym_shndx == "SHN_ABS":
        return sym_bind

    sym_type = sym.entry["st_info"]["type"]
    if sym_type == "STT_SECTION":
        return sym_bind

    if sym_bind != "STB_GLOBAL":
        return sym_bind

    # Assume any defined symbol that isn't a DLL export is local
    if idx in export_syms:
        return sym_bind

    return "STB_LOCAL"

# When non-handwritten asm is used in the compilation, the order of the GOT and $gp relocation table
# will not necessarily be correct. The lexical order that relocations appear in a disassembly is not
# always the same order that IDO will emit them. This function uses the original DLL's GOT as a reference
# to re-sort the GOT of the recompiled DLL and also sorts the $gp relocation table by ascending address.
def got_relocs_hack(got_and_relocs: elf2dll.GOTAndRelocations, target_got: list[int] | None):
    if target_got == None:
        return

    entry_sections: dict[int, str | None] = {}
    base_indices: dict[int, int] = {}
    base_set: set[int] = set()
    for i, entry in enumerate(got_and_relocs["got"]):
        value = entry["value"]
        entry_sections[value] = entry["section"]
        base_indices[value] = i
        base_set.add(value)

    # Note: At this stage in elf2dll, the local section GOT entries are all zero. Just skip them.
    #       We don't need to resort those anyway since the order is always the same.
    target_got = target_got[4:]
    target_got = [v for v in target_got if v in base_set]
    target_got_set = set(target_got)
    resorted_got: list[elf2dll.GOTEntry] = []
    got_remap: dict[int, int] = {}
    k = 0
    for i, entry in enumerate(got_and_relocs["got"]):
        if i < 4:
            got_remap[i] = len(resorted_got)
            resorted_got.append(entry)
            continue

        value = entry["value"]

        if k < len(target_got) and value in target_got_set:
            # GOT entry exists in original DLL, emit in the order from the original GOT
            swapped_value = target_got[k]
            got_remap[base_indices[swapped_value]] = len(resorted_got)
            section = entry_sections.get(value)
            resorted_got.append({ "value": swapped_value, "section": section })
            k += 1
        else:
            got_remap[i] = len(resorted_got)
            resorted_got.append(entry)
    
    got_and_relocs["got"] = resorted_got

    for reloc in got_and_relocs["text_relocs"]:
        if reloc["is_got_index"]:
            got_idx = reloc["half"] // 4
            reloc["half"] = got_remap[got_idx] * 4

    got_and_relocs["gp_relocs"].sort()

def main():
    parser = argparse.ArgumentParser(description="Runs elf2dll while handling quirks from nonmatching asm.")
    parser.add_argument("elf", type=argparse.FileType("rb"), help="The DLL .elf file to convert.")
    parser.add_argument("-o", "--output", type=argparse.FileType("wb"), help="The path of the Dinosaur Planet DLL file to output.", required=True)
    parser.add_argument("-b", "--bss", type=argparse.FileType("w", encoding="utf-8"), help="Path to output the .bss size as a text file.", required=True)
    parser.add_argument("-d", "--dll", type=argparse.FileType("rb"), help="The original DLL to use as a reference.", required=True)
    args = parser.parse_args()

    with ELFFile(args.elf) as obj:
        # Set up elf2dll hack callbacks
        export_syms = read_exports(obj)
        original_got = read_got(args.dll)

        elf2dll.hack_sym_bind_override = lambda idx,sym: sym_bind_hack(idx, sym, export_syms)
        elf2dll.hack_got_reloc_override = lambda x: got_relocs_hack(x, original_got)

        # Run elf2dll
        error = False
        try:
            elf2dll.convert(obj, args.output, args.bss, syms_writer=None)
        except elf2dll.ELF2DLLException as ex:
            print(f"ERROR: {ex}")
            error = True

    # Done (elf is already closed by the above 'with')
    args.output.close()
    args.bss.close()
    args.dll.close()
    
    if error:
        exit(1)

if __name__ == "__main__":
    main()
