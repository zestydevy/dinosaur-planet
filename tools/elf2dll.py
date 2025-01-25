#!/usr/bin/env python3
import argparse
import bisect
from io import BufferedWriter
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Section, SymbolTableSection
from elftools.elf.relocation import RelocationSection, Relocation
from elftools.elf.enums import ENUM_RELOC_TYPE_MIPS
import math
import os
from typing import TextIO, TypedDict
import struct

HEADER_SIZE = 0x18

class Exports(TypedDict):
    ctor: int
    dtor: int
    exports: "list[int]"

class GOTAndRelocations(TypedDict):
    got: "list[GOTEntry]"
    text_relocs: "list[Rel16Reloc]"
    gp_relocs: "list[int]"
    data_relocs: "list[Word32Reloc]"

class GOTEntry(TypedDict):
    value: int
    section: str | None

class Rel16Reloc(TypedDict):
    reloc_offset: int
    got_index: int

class Word32Reloc(TypedDict):
    reloc_offset: int
    word: int

def read_elf_exports(elf: ELFFile) -> Exports:
    syms = elf.get_section_by_name(".symtab")
    assert isinstance(syms, SymbolTableSection)

    rel_exports = elf.get_section_by_name(".rel.exports")
    if rel_exports != None:
        assert isinstance(rel_exports, RelocationSection)

        export_list: "list[int]" = []
        exports: Exports = {
            "ctor": 0,
            "dtor": 0,
            "exports": export_list
        }

        i = 0
        for reloc in rel_exports.iter_relocations():
            sym = syms.get_symbol(reloc["r_info_sym"])
            value = sym.entry["st_value"]
            if i == 0:
                exports["ctor"] = value
            elif i == 1:
                exports["dtor"] = value
            else:
                export_list.append(value)
            i += 1
        
        return exports

def read_elf_relocations(elf: ELFFile) -> GOTAndRelocations | None:
    syms = elf.get_section_by_name(".symtab")
    assert isinstance(syms, SymbolTableSection)

    got: "list[GOTEntry]" = []
    text_relocs: "list[Rel16Reloc]" = []
    gp_relocs: "list[int]" = []
    data_relocs: "list[Word32Reloc]" = []

    # First four GOT entries are always the sections (in this order)
    got.append({ "value": 0, "section": ".text" })
    got.append({ "value": 0, "section": ".rodata" })
    got.append({ "value": 0, "section": ".data" })
    got.append({ "value": 0, "section": ".bss" })

    syms_to_got: "dict[int, int]" = {}
    secs_to_got: "dict[str, int]" = {
        ".text": 0,
        ".rodata": 1,
        ".data": 2,
        ".bss": 3
    }

    rel_text = elf.get_section_by_name(".rel.text")
    if rel_text != None:
        assert isinstance(rel_text, RelocationSection)

        for reloc in iter_text_relocations_sorted(rel_text, syms):
            reloc_offset = reloc["r_offset"]
            reloc_type = reloc["r_info_type"]

            sym_idx = reloc["r_info_sym"]
            sym = syms.get_symbol(sym_idx)

            if reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_GOT16"] or \
                    reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_CALL16"]:
                # GOT reloc
                sym_shndx = sym.entry["st_shndx"]
                sym_type = sym.entry["st_info"]["type"]

                got_idx: int | None = None
                if sym_type == "STT_SECTION":
                    # For section symbols, use the existing GOT entry of one of the four supported sections
                    sym_sec_name = elf.get_section(sym_shndx).name
                    got_idx = secs_to_got.get(sym_sec_name)
                    if got_idx == None:
                        raise Exception(f"Found reloc with offset {hex(reloc_offset)} pointing to unsupported section symbol: {sym_sec_name}")
                else:
                    # For normal symbols, create a GOT entry if one doesn't already exist
                    got_idx = syms_to_got.get(sym_idx)
                    if got_idx == None:
                        got_idx = len(got)
                        syms_to_got[sym_idx] = got_idx

                        sym_value = sym.entry["st_value"]
                        sym_shndx = sym.entry["st_shndx"]
                        
                        section: str | None = None
                        if sym_shndx == "SHN_UNDEF":
                            print(f"WARN: reloc with offset {hex(reloc_offset)} has undefined symbol: {sym.name}")
                        elif sym_shndx != "SHN_ABS":
                            section = elf.get_section(sym_shndx).name
                        
                        got.append({ "value": sym_value, "section": section })

                assert got_idx != None

                # Record reloc location for later fixup
                text_relocs.append({ "reloc_offset": reloc_offset, "got_index": got_idx })
            elif reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_HI16"] and sym.name == "_gp_disp":
                # $gp prologue reloc
                gp_relocs.append(reloc_offset)
            elif reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_LO16"]:
                pass
            else:
                raise Exception(f"Unsupported reloc type '{reloc_type}' with offset {hex(reloc_offset)}")
    
    rel_data = elf.get_section_by_name(".rel.data")
    if rel_data != None:
        assert isinstance(rel_data, RelocationSection)

        data_data = elf.get_section_by_name(".data").data()

        for reloc in rel_data.iter_relocations():
            reloc_offset = reloc["r_offset"]
            reloc_type = reloc["r_info_type"]

            sym_idx = reloc["r_info_sym"]
            sym = syms.get_symbol(sym_idx)

            if reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_32"]:
                # data reloc
                s = sym.entry["st_value"]
                a = struct.unpack_from(">I", data_data, reloc_offset)[0]

                data_relocs.append({ "reloc_offset": reloc_offset, "word": s + a })
            else:
                raise Exception(f"Unsupported reloc type '{reloc_type}' with offset {hex(reloc_offset)}")

    if len(text_relocs) == 0 and len(gp_relocs) == 0 and len(data_relocs) == 0:
        # Don't emit GOT and reloc tables if no relocations are present
        return None
    else:
        return { 
            "got": got, 
            "text_relocs": text_relocs, 
            "gp_relocs": gp_relocs,
            "data_relocs": data_relocs
        }

def iter_text_relocations_sorted(rel_text: RelocationSection, syms: SymbolTableSection):
    # asm_processor appends relocations contributed by GLOBAL_ASM blocks, which breaks the order
    # that GOT entries are detected and thus results in a non-matching GOT.
    #
    # Group relocs by function (preserving reloc order within a function) and then sort the groups
    funcs: "list[int]" = []
    for sym in syms.iter_symbols():
        sym_value = sym.entry["st_value"]
        sym_type = sym.entry["st_info"]["type"]

        if sym_type != "STT_FUNC" or (sym_value & 0x80000000) != 0:
            # Skip non-functions and imported functions
            continue

        bisect.insort(funcs, sym_value)
    
    relocs_by_func: "list[list[Relocation]]" = [[] for _ in range(len(funcs))]
    for reloc in rel_text.iter_relocations():
        reloc_offset = reloc["r_offset"]
        func = bisect.bisect(funcs, reloc_offset) - 1
        relocs_by_func[func].append(reloc)
    
    for group in relocs_by_func:
        for reloc in group:
            yield reloc

def replace_gp_prologue_with_dino_version(writer: BufferedWriter, text_pos: int, gp_relocs: "list[int]"):
    # Change addiu to ori and nop out 'addu $gp, $gp, $t9'
    dino_gp_prologue = bytearray([
        0x3c, 0x1c, 0x00, 0x00, # lui $gp, 0x0
        0x37, 0x9c, 0x00, 0x00, # ori $gp, $gp, 0x0
        0x00, 0x00, 0x00, 0x00, # nop
    ])

    for reloc_offset in gp_relocs:
        writer.seek(text_pos + reloc_offset, os.SEEK_SET)
        writer.write(dino_gp_prologue)

def link_text_relocs(writer: BufferedWriter, text_pos: int, text_relocs: "list[Rel16Reloc]"):
    for reloc in text_relocs:
        writer.seek(text_pos + reloc["reloc_offset"] + 2, os.SEEK_SET)
        writer.write(struct.pack(">H", reloc["got_index"] * 4))

def link_data_relocs(writer: BufferedWriter, data_pos: int, data_relocs: "list[Word32Reloc]"):
    for reloc in data_relocs:
        writer.seek(data_pos + reloc["reloc_offset"], os.SEEK_SET)
        writer.write(struct.pack(">I", reloc["word"]))

def calc_exports_size(exports: Exports) -> int:
    # +8 for the blank entries before and after the export list
    return len(exports["exports"]) * 4 + 8

def calc_got_and_relocs_size(got_and_relocs: GOTAndRelocations) -> int:
    size = 4 + 4 + 4 # Terminators for GOT, $gp relocs, and data relocs
    size += len(got_and_relocs["got"]) * 4
    size += len(got_and_relocs["gp_relocs"]) * 4
    size += len(got_and_relocs["data_relocs"]) * 4

    return size

def align(n: int, alignment: int) -> int:
    return math.ceil(n / alignment) * alignment

def convert(elf: ELFFile, writer: BufferedWriter, bss_writer: TextIO):
    # Read ELF
    exports = read_elf_exports(elf)
    exports_size = calc_exports_size(exports)

    got_and_relocs = read_elf_relocations(elf)
    got_and_relocs_size = 0
    if got_and_relocs != None:
        got_and_relocs_size = calc_got_and_relocs_size(got_and_relocs)

    # Determine section sizes and positions
    text = elf.get_section_by_name(".text")
    rodata = elf.get_section_by_name(".rodata")
    data = elf.get_section_by_name(".data")
    bss = elf.get_section_by_name(".bss")

    text_data: bytes | None = None
    text_size: int = 0
    text_pos = HEADER_SIZE + exports_size
    if isinstance(text, Section):
        text_size = text.data_size
        if text_size > 0:
            text_data = text.data()
    
    rodata_data: bytes | None = None
    rodata_size: int = 0
    rodata_pos = text_pos + text_size
    if isinstance(rodata, Section):
        rodata_size = rodata.data_size
        if rodata_size > 0:
            rodata_data = rodata.data()
    
    data_data: bytes | None = None
    data_size: int = 0
    data_pos = rodata_pos + rodata_size + got_and_relocs_size
    if isinstance(data, Section):
        data_size = data.data_size
        if data_size > 0:
            data_data = data.data()
    
    bss_size: int = 0
    bss_pos = data_pos + data_size
    if isinstance(bss, Section):
        bss_size = bss.data_size
    
    dll_size_aligned = align(data_pos + data_size, 16)
    
    # Fixup GOT entries
    if got_and_relocs != None:
        got_section_positions = {
            ".text": 0,
            ".rodata": (rodata_pos + got_and_relocs_size) - text_pos,
            ".data": data_pos - text_pos,
            ".bss": bss_pos - text_pos,
        }
        for got_entry in got_and_relocs["got"]:
            section_name = got_entry["section"]
            if section_name != None:
                got_entry["value"] += got_section_positions[section_name]
    
    # Header
    writer.write(struct.pack(">I", text_pos if text_data != None else 0xFFFFFFFF))
    writer.write(struct.pack(">I", data_pos if data_data != None else 0xFFFFFFFF))
    writer.write(struct.pack(">I", rodata_pos if rodata_data != None or got_and_relocs != None else 0xFFFFFFFF))
    writer.write(struct.pack(">HH", len(exports["exports"]), 0))
    writer.write(struct.pack(">I", exports["ctor"]))
    writer.write(struct.pack(">I", exports["dtor"]))
    
    # Exports
    writer.write(struct.pack(">I", 0))
    for export in exports["exports"]:
        writer.write(struct.pack(">I", export))
    writer.write(struct.pack(">I", 0))
    
    # .text
    writer.write(bytearray(max(text_pos - writer.tell(), 0)))
    if text_data != None:
        writer.write(text_data)
    
    # .rodata
    writer.write(bytearray(max(rodata_pos - writer.tell(), 0)))
    if got_and_relocs != None:
        for got_entry in got_and_relocs["got"]:
            writer.write(struct.pack(">I", got_entry["value"]))
        writer.write(struct.pack(">I", 0xFFFFFFFE))
        for gp_reloc in got_and_relocs["gp_relocs"]:
            writer.write(struct.pack(">I", gp_reloc))
        writer.write(struct.pack(">I", 0xFFFFFFFD))
        for data_reloc in got_and_relocs["data_relocs"]:
            writer.write(struct.pack(">I", data_reloc["reloc_offset"]))
        writer.write(struct.pack(">I", 0xFFFFFFFF))
    if rodata_data != None:
        writer.write(rodata_data)

    # .data
    writer.write(bytearray(max(data_pos - writer.tell(), 0)))
    if data_data != None:
        writer.write(data_data)

    # Align file size
    writer.write(bytearray(max(dll_size_aligned - writer.tell(), 0)))

    if got_and_relocs != None:
        # Fixup $gp prologues
        replace_gp_prologue_with_dino_version(writer, text_pos, got_and_relocs["gp_relocs"])

        # Fixup .text relocs
        link_text_relocs(writer, text_pos, got_and_relocs["text_relocs"])

        # Fixup .data relocs
        link_data_relocs(writer, data_pos, got_and_relocs["data_relocs"])
    
    # Write .bss size to text file
    bss_writer.write(hex(bss_size))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("elf", type=argparse.FileType("rb"), help="The DLL .elf file to convert.")
    parser.add_argument("-o", "--output", type=argparse.FileType("wb"), help="The path of the Dinosaur Planet DLL file to output.", required=True)
    parser.add_argument("-b", "--bss", type=argparse.FileType("w", encoding="utf-8"), help="Path to output the .bss size as a text file.", required=True)
    args = parser.parse_args()
    
    with ELFFile(args.elf) as elf:
        convert(elf, args.output, args.bss)
    
    args.output.close()
    args.bss.close()

if __name__ == "__main__":
    main()
