#!/usr/bin/env python3
import argparse
from io import BufferedWriter
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Section, SymbolTableSection
from elftools.elf.relocation import RelocationSection
from elftools.elf.enums import ENUM_RELOC_TYPE_MIPS
import math
import os
from typing import TextIO, TypedDict
import struct

HEADER_SIZE = 0x18

class ELF2DLLException(Exception):
    pass

class Exports(TypedDict):
    ctor: int
    dtor: int
    exports: "list[int]"

class ReadRelocations(TypedDict):
    got_and_relocs: "GOTAndRelocations | None"
    rodata_relocs: "list[GPRel32Reloc]"
    ri_gp_value: int | None

class GOTAndRelocations(TypedDict):
    got: "list[GOTEntry]"
    text_relocs: "list[TextReloc]"
    gp_relocs: "list[int]"
    data_relocs: "list[Word32Reloc]"

class GOTEntry(TypedDict):
    value: int
    section: str | None

# rel16 or lo16
class TextReloc(TypedDict):
    reloc_offset: int
    half: int

class Word32Reloc(TypedDict):
    reloc_offset: int
    word: int

class GPRel32Reloc(TypedDict):
    reloc_offset: int
    sa: int # S + A

def read_elf_exports(elf: ELFFile) -> Exports:
    syms = elf.get_section_by_name(".symtab")
    assert isinstance(syms, SymbolTableSection)

    rel_exports = elf.get_section_by_name(".rel.exports")
    if rel_exports == None:
        raise ELF2DLLException("ELF missing required .rel.exports section.")
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

def read_elf_relocations(elf: ELFFile, 
                         text_data: bytes | None, 
                         rodata_data: bytes | None, 
                         data_data: bytes | None) -> ReadRelocations:
    syms = elf.get_section_by_name(".symtab")
    assert isinstance(syms, SymbolTableSection)

    got: "list[GOTEntry]" = []
    text_relocs: "list[TextReloc]" = []
    gp_relocs: "list[int]" = []
    data_relocs: "list[Word32Reloc]" = []
    rodata_relocs: "list[GPRel32Reloc]" = []
    ri_gp_value: int | None = None

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
    his_to_got: "dict[int, int]" = {}

    reginfo = elf.get_section_by_name(".reginfo")
    if reginfo != None:
        assert isinstance(reginfo, Section)

        # Unpack from ELF_RegInfo
        ri_gp_value = struct.unpack_from(">i", reginfo.data(), 0x14)[0]

    rel_text = elf.get_section_by_name(".rel.text")
    if rel_text != None:
        assert isinstance(rel_text, RelocationSection)
        assert text_data != None

        for reloc in rel_text.iter_relocations():
            reloc_offset = reloc["r_offset"]
            reloc_type = reloc["r_info_type"]

            sym_idx = reloc["r_info_sym"]
            sym = syms.get_symbol(sym_idx)

            sym_value = sym.entry["st_value"]
            sym_shndx = sym.entry["st_shndx"]
            sym_type = sym.entry["st_info"]["type"]
            st_info_bind = sym.entry["st_info"]["bind"]

            if sym_shndx == "SHN_UNDEF" and sym.name != "_gp_disp":
                raise ELF2DLLException(f".rel.text reloc with offset {hex(reloc_offset)} references an undefined symbol: {sym.name}")

            if reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_GOT16"] or \
                    reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_CALL16"]:
                # GOT reloc
                got_idx: int | None = None

                if st_info_bind == "STB_LOCAL":
                    if sym_type == "STT_SECTION":
                        # For section symbols, use the existing GOT entry of one of the four supported sections
                        sym_sec_name = elf.get_section(sym_shndx).name
                        got_idx = secs_to_got.get(sym_sec_name)
                        if got_idx == None:
                            raise ELF2DLLException(f"Found reloc with offset {hex(reloc_offset)} pointing to unsupported section symbol: {sym_sec_name}")
                    else:
                        if sym_value >= 0x8000:
                            # For local symbols that are too big, we need to generate a GOT entry for the hi bits (sorta)
                            # Not sure if this logic is specific to Dinosaur Planet DLLs
                            assert(elf.get_section(sym_shndx).name == ".text") # Not sure this logic works anywhere else at the moment

                            remainder = sym_value % 0x1_0000
                            if remainder < (0x1_0000 // 2):
                                hi_got_value = sym_value - remainder
                            else:
                                hi_got_value = sym_value + (0x1_0000 - remainder)
                            
                            got_idx = his_to_got.get(hi_got_value)
                            if got_idx == None:
                                got_idx = len(got)
                                his_to_got[hi_got_value] = got_idx
                                
                                got.append({ "value": hi_got_value, "section": None })
                        else:
                            # For local symbols, use the existing GOT entry of the section the symbol is relative to
                            sym_sec_name = elf.get_section(sym_shndx).name
                            got_idx = secs_to_got.get(sym_sec_name)
                            if got_idx == None:
                                raise ELF2DLLException(f"Found reloc with offset {hex(reloc_offset)} pointing to unsupported section symbol: {sym_sec_name}")
                else:
                    assert st_info_bind == "STB_GLOBAL"

                    # For global symbols, create a GOT entry if one doesn't already exist
                    got_idx = syms_to_got.get(sym_idx)
                    if got_idx == None:
                        got_idx = len(got)
                        syms_to_got[sym_idx] = got_idx
                        
                        section: str | None = None
                        if sym_shndx != "SHN_ABS":
                            section = elf.get_section(sym_shndx).name
                        
                        got.append({ "value": sym_value, "section": section })

                assert got_idx != None

                # Record reloc location for later fixup
                text_relocs.append({ "reloc_offset": reloc_offset, "half": got_idx * 4 })
            elif reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_HI16"]:
                if sym.name == "_gp_disp":
                    # $gp prologue reloc
                    gp_relocs.append(reloc_offset)
                else:
                    # ((AHL + S) – (short)(AHL + S)) >> 16
                    ahl = struct.unpack_from(">H", text_data, reloc_offset + 2)[0]
                    s = sym_value
                    ahls = ahl + s
                    half = ((ahls - (ahls & 0xFFFF)) >> 16) & 0xFFFF
                    text_relocs.append({ "reloc_offset": reloc_offset, "half": half })
            elif reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_LO16"]:
                # Global syms won't have a lo16 pair (except _gp_disp but that's taken care of elsewhere)
                # Section sym lo16 pairs will already have the correct value from the linker so we can leave them alone
                if sym.name != "_gp_disp":
                    # AHL + S
                    ahl = struct.unpack_from(">H", text_data, reloc_offset + 2)[0]
                    text_relocs.append({ "reloc_offset": reloc_offset, "half": (ahl + sym_value) & 0xFFFF })
            elif reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_PC16"]:
                # Note: R_MIPS_PC16 is not used by IDO, this is to support ROM hacking
                # sign–extend(A) + S – P
                a = struct.unpack_from(">h", text_data, reloc_offset + 2)[0]
                s = sym_value
                p = reloc_offset
                # Note: The PC16 value needs to be shifted down by 2 bits because it's measured in
                # instructions and not bytes.
                text_relocs.append({ "reloc_offset": reloc_offset, "half": (((a << 2) + s - p) >> 2) & 0xFFFF })
            elif reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_JALR"]:
                # Note: R_MIPS_JALR is not used by IDO, this is to support ROM hacking
                # R_MIPS_JALR is a hint to linkers that a JALR instruction could possibly be turned into a direct call instead.
                # This can safely be ignored.
                pass
            else:
                raise ELF2DLLException(f"Unsupported .text reloc type '{reloc_type}' with offset {hex(reloc_offset)} and symbol {sym.name}")
    
    rel_data = elf.get_section_by_name(".rel.data")
    if rel_data != None:
        assert isinstance(rel_data, RelocationSection)
        assert data_data != None

        for reloc in rel_data.iter_relocations():
            reloc_offset = reloc["r_offset"]
            reloc_type = reloc["r_info_type"]

            sym_idx = reloc["r_info_sym"]
            sym = syms.get_symbol(sym_idx)

            if sym.entry["st_shndx"] == "SHN_UNDEF":
                raise ELF2DLLException(f".rel.data reloc with offset {hex(reloc_offset)} references an undefined symbol: {sym.name}")

            if reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_32"]:
                # data reloc
                s = sym.entry["st_value"]
                a = struct.unpack_from(">I", data_data, reloc_offset)[0]

                data_relocs.append({ "reloc_offset": reloc_offset, "word": s + a })
            else:
                raise ELF2DLLException(f"Unsupported .data reloc type '{reloc_type}' with offset {hex(reloc_offset)}")

    rel_rodata = elf.get_section_by_name(".rel.rodata")
    if rel_rodata != None:
        assert isinstance(rel_rodata, RelocationSection)
        assert rodata_data != None

        text_shndx = elf.get_section_index(".text")

        for reloc in rel_rodata.iter_relocations():
            reloc_offset = reloc["r_offset"]
            reloc_type = reloc["r_info_type"]

            sym_idx = reloc["r_info_sym"]
            sym = syms.get_symbol(sym_idx)

            if sym.entry["st_shndx"] == "SHN_UNDEF":
                raise ELF2DLLException(f".rel.rodata reloc with offset {hex(reloc_offset)} references an undefined symbol: {sym.name}")

            assert(sym.entry["st_shndx"] == text_shndx)

            sym_type = sym.entry["st_info"]["type"]
            sym_bind = sym.entry["st_info"]["bind"]

            if reloc_type == ENUM_RELOC_TYPE_MIPS["R_MIPS_GPREL32"]:
                # rodata reloc
                s = sym.entry["st_value"]
                a = struct.unpack_from(">i", rodata_data, reloc_offset)[0]

                if sym_bind != "STB_LOCAL" or sym_type != "STT_SECTION":
                    # TODO: i don't fully understand why this is necessary
                    s -= ri_gp_value

                rodata_relocs.append({ "reloc_offset": reloc_offset, "sa": s + a })
            else:
                raise ELF2DLLException(f"Unsupported .rodata reloc type '{reloc_type}' with offset {hex(reloc_offset)}")

    got_and_relocs: GOTAndRelocations | None
    if len(text_relocs) == 0 and len(gp_relocs) == 0 and len(data_relocs) == 0:
        # Don't emit GOT and reloc tables if no relocations are present
        got_and_relocs = None
    else:
        got_and_relocs = { 
            "got": got, 
            "text_relocs": text_relocs, 
            "gp_relocs": gp_relocs,
            "data_relocs": data_relocs
        }
    
    return {
        "got_and_relocs": got_and_relocs,
        "rodata_relocs": rodata_relocs,
        "ri_gp_value": ri_gp_value
    }

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

def link_text_relocs(writer: BufferedWriter, text_pos: int, text_relocs: "list[TextReloc]"):
    for reloc in text_relocs:
        writer.seek(text_pos + reloc["reloc_offset"] + 2, os.SEEK_SET)
        writer.write(struct.pack(">H", reloc["half"]))

def link_data_relocs(writer: BufferedWriter, data_pos: int, data_relocs: "list[Word32Reloc]"):
    for reloc in data_relocs:
        writer.seek(data_pos + reloc["reloc_offset"], os.SEEK_SET)
        writer.write(struct.pack(">I", reloc["word"]))

def link_rodata_relocs(writer: BufferedWriter, shifted_rodata_pos: int, rodata_relocs: "list[GPRel32Reloc]", gp0: int, gp: int):
    for reloc in rodata_relocs:
        # A + S + GP0 – GP
        word = reloc["sa"] + gp0 - gp
        
        writer.seek(shifted_rodata_pos + reloc["reloc_offset"], os.SEEK_SET)
        writer.write(struct.pack(">i", word))

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

def convert(elf: ELFFile, writer: BufferedWriter, bss_writer: TextIO, syms_writer: TextIO | None):
    # Read input sections
    text = elf.get_section_by_name(".text")
    rodata = elf.get_section_by_name(".rodata")
    data = elf.get_section_by_name(".data")
    bss = elf.get_section_by_name(".bss")

    # Note: Output section sizes must be padded to at least a 4-byte alignment
    #       (IDO pads these to 16 bytes already).
    text_data: bytes | None = None
    text_size: int = 0
    if isinstance(text, Section):
        text_size = align(text.data_size, 4)
        if text_size > 0:
            text_data = text.data()
    
    rodata_data: bytes | None = None
    rodata_size: int = 0
    if isinstance(rodata, Section):
        rodata_size = align(rodata.data_size, 4)
        if rodata_size > 0:
            rodata_data = rodata.data()
    
    data_data: bytes | None = None
    data_size: int = 0
    if isinstance(data, Section):
        data_size = align(data.data_size, 4)
        if data_size > 0:
            data_data = data.data()
    
    bss_size: int = 0
    if isinstance(bss, Section):
        bss_size = align(bss.data_size, 4)
    
    # Read exports and relocations
    exports = read_elf_exports(elf)
    exports_size = calc_exports_size(exports)

    read_relocations = read_elf_relocations(elf, text_data, rodata_data, data_data)
    got_and_relocs = read_relocations["got_and_relocs"]
    got_and_relocs_size = 0
    if got_and_relocs != None:
        got_and_relocs_size = calc_got_and_relocs_size(got_and_relocs)

    # Determine section output positions
    text_pos = HEADER_SIZE + exports_size
    rodata_pos = text_pos + text_size
    data_pos = rodata_pos + rodata_size + got_and_relocs_size
    bss_pos = data_pos + data_size

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
    
    ri_gp_value = read_relocations["ri_gp_value"]
    if ri_gp_value != None:
        # Apply .rodata relocs
        #
        # These don't get added to the special .rodata relocs section but do need to be linked ahead of time.
        # .rodata relocations are relative to .rodata's actual data, after the GOT and relocs.
        shifted_rodata_pos = rodata_pos + got_and_relocs_size
        
        link_rodata_relocs(writer, shifted_rodata_pos, read_relocations["rodata_relocs"], 
                           gp0=ri_gp_value, 
                           gp=rodata_pos - text_pos)
    else:
        assert(len(read_relocations["rodata_relocs"]) == 0)

    # Write .bss size to text file
    bss_writer.write(hex(bss_size))

    # Write symbols for debugging (if enabled)
    if syms_writer != None:
        syms = elf.get_section_by_name(".symtab")
        assert isinstance(syms, SymbolTableSection)

        rodata_offset = (rodata_pos + got_and_relocs_size) - text_pos
        data_offset = data_pos - text_pos
        bss_offset = bss_pos - text_pos

        for sym in syms.iter_symbols():
            sym_value = sym.entry["st_value"]
            sym_shndx = sym.entry["st_shndx"]
            sym_type = sym.entry["st_info"]["type"]

            if sym_shndx == "SHN_UNDEF" or sym_type == "STT_SECTION" or sym_type == "STT_FILE":
                continue
            if sym_shndx != "SHN_ABS":
                assert isinstance(sym_shndx, int)
                section = elf.get_section(sym_shndx)
                if section.name == ".text":
                    pass
                elif section.name == ".rodata":
                    sym_value += rodata_offset
                elif section.name == ".data":
                    sym_value += data_offset
                elif section.name == ".bss":
                    sym_value += bss_offset
                else:
                    continue

            syms_writer.write("{} = 0x{:X};\n".format(sym.name, sym_value))

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("elf", type=argparse.FileType("rb"), help="The DLL .elf file to convert.")
    parser.add_argument("-o", "--output", type=argparse.FileType("wb"), help="The path of the Dinosaur Planet DLL file to output.", required=True)
    parser.add_argument("-b", "--bss", type=argparse.FileType("w", encoding="utf-8"), help="Path to output the .bss size as a text file.", required=True)
    parser.add_argument("-s", "--syms-map", dest="syms_output", type=argparse.FileType("w", encoding="utf-8"), help="Path to output symbol mapping for debugging.")
    args = parser.parse_args()
    
    error = False
    try:
        with ELFFile(args.elf) as elf:
            convert(elf, args.output, args.bss, args.syms_output)
    except ELF2DLLException as ex:
        print(f"ERROR: {ex}")
        error = True
    finally:
        args.output.close()
        args.bss.close()
    
    if error:
        exit(1)

if __name__ == "__main__":
    main()
