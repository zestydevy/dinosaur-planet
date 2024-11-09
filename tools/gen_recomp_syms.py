import argparse
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection, Section
from typing import TextIO
import math
import os
from pathlib import Path

from dino.dll import DLL
from dino.dll_tab import DLLTab

BIN_PATH = Path("bin")
BUILD_PATH = Path("build")

# vram -> function def
FUNCTION_DEF_HACKS = {
    # These functions are supposed to fallthrough to the next function
    0x80016178: { "name": "cos16_precise", "size": 0x60 },
    0x800161d8: { "name": "cos16", "size": 0x44 },
    0x800162f4: { "name": "fcos16", "size": 0x54 },

    # 0x8001B4F0-0x8001C8D4 is handwritten assembly that trips up recomp, split it into a bunch of
    # functions that don't technically exist to get recomp working. This will basically make problematic
    # conditional branches turn into condotional tail calls into these fake functions
    # Note: Some of the fake functions must overlap fake functions below them
    0x8001B4F0: { "name": "func_8001B4F0", "size": 0x200 },
    0x8001B6F0: { "name": "func_8001B6F0", "size": 0x510 },
    0x8001BC00: { "name": "func_8001BC00", "size": 0xF0 },
    0x8001BCF0: { "name": "func_8001BCF0", "size": 0x280 },
    0x8001BF70: { "name": "func_8001BF70", "size": 0x268 },
    0x8001C1D8: { "name": "func_8001C1D8", "size": 0x440 }, # synthetic
    0x8001C5A0: { "name": "func_8001C5A0", "size": 0x78 }, # synthetic
    0x8001C618: { "name": "func_8001C618", "size": 0x7C }, # synthetic
    0x8001C660: { "name": "func_8001C660", "size": 0x34 }, # synthetic
    0x8001C694: { "name": "func_8001C694", "size": 0x20C }, # synthetic
    0x8001C6C8: { "name": "func_8001C6C8", "size": 0x1D8 }, # synthetic
    0x8001C834: { "name": "func_8001C834", "size": 0x6C }, # synthetic
    0x8001C83C: { "name": "func_8001C83C", "size": 0x64 }, # synthetic
    0x8001C8A0: { "name": "func_8001C8A0", "size": 0x34 }, # synthetic
}

def gen_core_syms(syms_toml: TextIO):
    text_start = 0x80000400
    text_size = 0x89350
    text_end = text_start + text_size
    segment_start = text_start
    segment_size = 0xA3AA0

    funcs = []
    vrams: dict[int, int] = {}

    # Grab functions from ROM .elf
    with open(BUILD_PATH.joinpath("dino.elf"), "rb") as file:
        elf = ELFFile(file)
        syms = elf.get_section_by_name(".symtab")
        assert isinstance(syms, SymbolTableSection)

        for sym in syms.iter_symbols():
            value = sym.entry["st_value"]
            size = sym.entry["st_size"]

            if value < text_start or value >= text_end:
                continue

            if sym.name.startswith("L8"):
                continue

            # Apply override hacks
            def_hack = FUNCTION_DEF_HACKS.get(value)
            if def_hack != None:
                assert def_hack["name"] == sym.name
                size = def_hack["size"]
                print("Overriding function {} definition to 0x{:X}".format(sym.name, size))
        
            func = { "name": sym.name, "vram": value, "size": size if size != 0 else None }

            idx = vrams.get(value)
            if idx == None:
                vrams[value] = len(funcs)
                funcs.append(func)
            elif funcs[idx]["name"].startswith("func_") or funcs[idx]["name"] == "":
                funcs[idx] = func
    
    # Add any functions defined in the hacks table that wasn't found in the .elf
    for (vram, hack) in FUNCTION_DEF_HACKS.items():
        if vrams.get(vram) == None:
            func = { "name": hack["name"], "vram": vram, "size": hack["size"] }
            funcs.append(func)
            print("Adding manual function definition {} @ 0x{:X} with size 0x{:X}".format(hack["name"], vram, hack["size"]))
    
    funcs.sort(key=lambda f : f["vram"])
    
    syms_toml.write("[[section]]\n")
    syms_toml.write("name = \".segment\"\n")
    syms_toml.write("rom = 0x00001000\n")
    syms_toml.write("vram = 0x{:X}\n".format(segment_start))
    syms_toml.write("size = 0x{:X}\n".format(segment_size))
    syms_toml.write("\n")

    syms_toml.write("functions = [\n")
    i = 0
    for func in funcs:
        name = func["name"]
        vram = func["vram"]
        size = func["size"]

        if size == None:
            print(f"Inferring size for {name}")
            if i < len(funcs) - 1:
                size = funcs[i + 1]["vram"] - vram
            else:
                size = text_end - vram
        
        syms_toml.write("    {{ name = \"{}\", vram = 0x{:X}, size = 0x{:X} }},\n"
            .format(name, vram, size))

        i += 1
    syms_toml.write("]\n")

def gen_dll_syms(syms_toml: TextIO, dlls_txt: TextIO):
    with open(BIN_PATH.joinpath("assets/DLLS_tab.bin"), "rb") as tab_file:
        tab = DLLTab.parse(bytearray(tab_file.read()))

    with open(BIN_PATH.joinpath("assets/DLLS.bin"), "rb") as dlls_file:
        dlls_data = bytearray(dlls_file.read())

    dlls_rom_base = 0x38317CC
    dlls_vram_base = 0x81000000

    number = 1
    dll_rom = dlls_rom_base
    dll_vram = dlls_vram_base

    for entry in tab.entries:
        dll_data = dlls_data[entry.start_offset:entry.end_offset]
        assert len(dll_data) == entry.size

        funcs = []
        vrams_to_funcs: dict[int, dict] = {}

        text_start = 0
        text_end = 0
        
        # Grab functions by parsing the DLL contents
        dll = DLL.parse(dll_data, str(number), include_funcs=True)
        assert dll.functions is not None
        for func in dll.functions:
            func_info = { 
                "name": "dll_{}_func_{:X}".format(number, func.address), 
                "vram": dll_vram + dll.header.size + func.address, 
                "size": len(func.insts) * 4
            }
            funcs.append(func_info)
            vrams_to_funcs[func_info["vram"]] = func_info

        # Get custom function names from the DLL .elf if we have decomp set up for this DLL
        dll_elf_path = BUILD_PATH.joinpath(f"src/dlls/{number}/{number}.elf")
        if os.path.exists(dll_elf_path):
            with open(dll_elf_path, "rb") as file:
                elf = ELFFile(file)
                text = elf.get_section_by_name(".text")
                syms = elf.get_section_by_name(".symtab")
                assert isinstance(text, Section)
                assert isinstance(syms, SymbolTableSection)

                text_start = text.header["sh_addr"]
                text_end = text_start + text.header["sh_size"]

                func_name_prefix = f"dll_{number}_"

                for sym in syms.iter_symbols():
                    value = sym.entry["st_value"]
                    st_shndx = sym.entry["st_shndx"]
                    st_info_type = sym.entry["st_info"]["type"]

                    # non-static = STT_FUNC, static = STT_NOTYPE
                    if st_shndx != "SHN_ABS" or (st_info_type != "STT_FUNC" and st_info_type != "STT_NOTYPE"):
                        continue
                    if value < text_start or value >= text_end:
                        continue

                    vram = dll_vram + dll.header.size + value

                    func_info = vrams_to_funcs.get(vram)
                    if func_info == None:
                        print("Failed to find DLL {} func {} at {:0X} ({:0X})"
                            .format(number, sym.name, vram, value))
                    else:
                        # Make sure the name is globally unique
                        # This isn't required in the decomp since DLLs are not linked with each other,
                        # but this is the case for recomp.
                        new_name = sym.name
                        if not new_name.startswith(func_name_prefix):
                            new_name = func_name_prefix + new_name
                        func_info["name"] = new_name
        
        funcs.sort(key=lambda f : f["vram"])

        dlls_txt.write(f".dll{number}\n")

        syms_toml.write("\n")
        syms_toml.write("[[section]]\n")
        syms_toml.write(f"name = \".dll{number}\"\n")
        syms_toml.write("rom = 0x{:X}\n".format(dll_rom + dll.header.size))
        syms_toml.write("vram = 0x{:X}\n".format(dll_vram + dll.header.size))
        syms_toml.write("size = 0x{:X}\n".format(entry.size))
        syms_toml.write("gp = 0x{:X}\n".format(dll_vram + dll.header.rodata_offset))
        syms_toml.write("\n")

        syms_toml.write("relocs = [\n")
        for reloc in dll.reloc_table.gp_relocations:
            syms_toml.write("    {{ type = \"R_MIPS_HI16\", vram = 0x{:X}, target_vram = 0x{:X}, unaligned = true }},\n"
                .format(dll_vram + dll.header.size + reloc, dll_vram + dll.header.rodata_offset))
            syms_toml.write("    {{ type = \"R_MIPS_LO16\", vram = 0x{:X}, target_vram = 0x{:X}, unaligned = true }},\n"
                .format(dll_vram + dll.header.size + reloc + 4, dll_vram + dll.header.rodata_offset))
        syms_toml.write("]\n")
        syms_toml.write("\n")

        syms_toml.write("functions = [\n")
        i = 0
        for func in funcs:
            name = func["name"]
            vram = func["vram"]
            size = func["size"]

            if size == None:
                print(f"Inferring size for {name}")
                if i < len(funcs) - 1:
                    size = funcs[i + 1]["vram"] - vram
                else:
                    assert(text_end != 0)
                    size = text_end - vram

            syms_toml.write("    {{ name = \"{}\", vram = 0x{:X}, size = 0x{:X} }},\n"
                .format(name, vram, size))
            i += 1
        syms_toml.write("]\n")
        
        dll_rom += entry.size
        dll_vram += math.ceil((entry.size + entry.bss_size) / 4096) * 4096
        number += 1

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project (default=..).", default="..")
    args = parser.parse_args()

    out_syms_path = Path("dino.syms.toml").absolute()
    out_dlls_txt_path = Path("dino.dlls.txt").absolute()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    # Gen syms
    with open(out_syms_path, "w", encoding="utf-8") as syms_toml:
        print("Generating core symbols...")
        gen_core_syms(syms_toml)
        print("Generating DLL symbols...")
        with open(out_dlls_txt_path, "w", encoding="utf-8") as dlls_txt:
            gen_dll_syms(syms_toml, dlls_txt)
    print("Done. Wrote symbols to {}".format(out_syms_path))

if __name__ == "__main__":
    main()
