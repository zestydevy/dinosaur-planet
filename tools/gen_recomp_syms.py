import argparse
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
from typing import TextIO
import math
import os
from pathlib import Path

from dino.dll import DLL
from dino.dll_tab import DLLTab

BIN_PATH = Path("bin")
BUILD_PATH = Path("build")

def gen_core_syms(syms_toml: TextIO):
    textStart = 0x80000400
    textSize = 0x89350
    textEnd = textStart + textSize

    funcs = []
    vrams: dict[int, int] = {}

    with open(BUILD_PATH.joinpath("dino.elf"), "rb") as file:
        elf = ELFFile(file)
        syms = elf.get_section_by_name(".symtab")
        assert isinstance(syms, SymbolTableSection)

        for sym in syms.iter_symbols():
            value = sym.entry["st_value"]
            size = sym.entry["st_size"]

            if value < textStart or value >= textEnd:
                continue

            if sym.name.startswith("L8"):
                continue
        
            func = { "name": sym.name, "vram": value, "size": size if size != 0 else None }

            idx = vrams.get(value)
            if idx == None:
                vrams[value] = len(funcs)
                funcs.append(func)
            elif funcs[idx]["name"].startswith("func_") or funcs[idx]["name"] == "":
                funcs[idx] = func
    
    funcs.sort(key=lambda f : f["vram"])
    
    syms_toml.write("[[section]]\n")
    syms_toml.write("name = \".segment\"\n")
    syms_toml.write("rom = 0x00001000\n")
    syms_toml.write("vram = 0x{:X}\n".format(textStart))
    syms_toml.write("size = 0x{:X}\n".format(textSize))
    syms_toml.write("\n")

    syms_toml.write("functions = [\n")
    i = 0
    for func in funcs:
        size = func["size"]
        if size == None:
            print(f"Inferring size for {func['name']}")
            if i < len(funcs) - 1:
                size = funcs[i + 1]["vram"] - func["vram"]
            else:
                size = textEnd - func["vram"]
        
        syms_toml.write("    {{ name = \"{}\", vram = 0x{:X}, size = 0x{:X} }},\n"
            .format(func["name"], func["vram"], size))

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

        dll = DLL.parse(dll_data, str(number), include_funcs=True)

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
            syms_toml.write("    {{ type = \"R_MIPS_HI16\", vram = 0x{:X}, target_vram = 0x{:X} }},\n"
                .format(dll_vram + dll.header.size + reloc, dll_vram + dll.header.rodata_offset))
            syms_toml.write("    {{ type = \"R_MIPS_LO16\", vram = 0x{:X}, target_vram = 0x{:X} }},\n"
                .format(dll_vram + dll.header.size + reloc + 4, dll_vram + dll.header.rodata_offset))
        syms_toml.write("]\n")
        syms_toml.write("\n")

        syms_toml.write("functions = [\n")
        assert dll.functions is not None
        for func in dll.functions:
            syms_toml.write("    {{ name = \"dll_{}_func_{:X}\", vram = 0x{:X}, size = 0x{:X} }},\n"
                .format(number, func.address, dll_vram + dll.header.size + func.address, len(func.insts) * 4))
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
