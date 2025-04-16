#!/usr/bin/env python3
import argparse
import os
from pathlib import Path

from dino.dll import DLL
from dino.dll_tab import DLLTab
from dino.dll_analysis import get_all_dll_functions

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")
SRC_DLLS_DIR = Path("src/dlls")

def narrow_down(dll: DLL, dll_data: bytes, relative_addr: int):
    print("DLL {}".format(dll.number))

    if dll.has_data() and relative_addr >= dll.header.data_offset:
        print(".data+0x{:X}".format(relative_addr - dll.header.data_offset))
    elif dll.has_rodata() and relative_addr >= dll.header.rodata_offset:
        print(".rodata+0x{:X}".format(relative_addr - dll.header.rodata_offset))
    elif relative_addr >= dll.header.size and relative_addr < (dll.header.size + dll.get_text_size()):
        funcs = get_all_dll_functions(dll_data, dll)
        funcs.sort(key=lambda f: f.address, reverse=True)
        for func in funcs:
            func_romaddr = dll.header.size + func.address
            if relative_addr >= func_romaddr:
                print("func_{:X}+0x{:X}".format(func.address, relative_addr - func_romaddr))
                break
    else:
        print("dll+0x{:X}".format(relative_addr))

    pass

def main():
    parser = argparse.ArgumentParser(description="Finds what part of which DLL a given ROM address points to.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("romaddr", type=str, help="The DLLS.BIN relative ROM address or absolute ROM address to lookup.")

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    # Load DLLS.TAB/DLLS.BIN
    dllsbin_path = BIN_ASSETS_DIR.joinpath("DLLS.bin")
    dllstab_path = BIN_ASSETS_DIR.joinpath("DLLS_tab.bin")

    if not dllsbin_path.exists():
        print(f"ERR: DLLS.BIN not found @ {dllsbin_path.absolute()}")
        exit(1)
    if not dllstab_path.exists():
        print(f"ERR: DLLS.TAB not found @ {dllstab_path.absolute()}")
        exit(1)

    # Locate
    romaddr = int(args.romaddr, base=0)

    if romaddr > 0x38317CC:
        # Probably an absolute ROM address
        romaddr -= 0x38317CC
    
    found = False
    with open(dllstab_path, "rb") as tab_file, \
         open(dllsbin_path, "rb") as bin_file:
        tab = DLLTab.parse(tab_file.read())

        dllNumber = 1
        for entry in tab.entries:
            if romaddr >= entry.start_offset and romaddr < entry.end_offset:
                bin_file.seek(entry.start_offset)
                data = bytearray(bin_file.read(entry.size))
                dll = DLL.parse(data, number=dllNumber)
                narrow_down(dll, data, romaddr - entry.start_offset)
                found = True
                break
            dllNumber += 1

    if not found:
        print("Not a DLL ROM address.")

if __name__ == "__main__":
    main()
