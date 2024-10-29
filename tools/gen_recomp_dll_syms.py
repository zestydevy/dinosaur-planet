import argparse
import math
import os
from pathlib import Path

from dino.dll import DLL
from dino.dll_tab import DLLTab

BIN_PATH = Path("bin")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project (default=..).", default="..")
    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    with open(BIN_PATH.joinpath("assets/DLLS_tab.bin"), "rb") as tab_file:
        tab = DLLTab.parse(bytearray(tab_file.read()))

    with open(BIN_PATH.joinpath("assets/DLLS.bin"), "rb") as dlls_file:
        dlls_data = bytearray(dlls_file.read())

    dlls_rom_base = 0x38317CC
    dlls_vram_base = 0x81000000

    with open("dino.syms.dlls.toml", "w", encoding="utf-8") as syms_toml:
        number = 1
        dll_rom = dlls_rom_base
        dll_vram = dlls_vram_base

        for entry in tab.entries:
            dll_data = dlls_data[entry.start_offset:entry.end_offset]
            assert len(dll_data) == entry.size

            dll = DLL.parse(dll_data, str(number), include_funcs=True)

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
            syms_toml.write("\n")
            
            dll_rom += entry.size
            dll_vram += math.ceil((entry.size + entry.bss_size) / 4096) * 4096
            number += 1

if __name__ == "__main__":
    main()
