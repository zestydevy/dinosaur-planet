#!/usr/bin/env python3
import argparse
from genericpath import isdir
import glob
import hashlib
import os
from pathlib import Path

from dino.dll import DLL
from dino.dll_tab import DLLTab
from dino.dlls_txt import DLLsTxt

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")
BUILD_ASSETS_DIR = Path("build/bin/assets")
SRC_DLLS_DIR = Path("src/dlls")

def iter_diffs(base: DLL, base_data: bytes, base_bss_size: int, targ: DLL, targ_data: bytes, targ_bss_size: int):
    if base.header.size != targ.header.size:
        yield ".text offset mismatch: expected {:#x}, found {:#x}".format(targ.header.size, base.header.size)
    if base.header.rodata_offset != targ.header.rodata_offset:
        yield ".rodata offset mismatch: expected {:#x}, found {:#x}".format(targ.header.rodata_offset, base.header.rodata_offset)
    if base.header.data_offset != targ.header.data_offset:
        yield ".data offset mismatch: expected {:#x}, found {:#x}".format(targ.header.data_offset, base.header.data_offset)
    
    base_bss_offset = base.get_bss_offset()
    targ_bss_offset = targ.get_bss_offset()
    if base_bss_offset != targ_bss_offset:
        yield ".bss offset mismatch: expected {:#x}, found {:#x}".format(targ_bss_offset, base_bss_offset)

    if base.header.ctor_offset != targ.header.ctor_offset:
        yield "Ctor offset mismatch: expected {:#x}, found {:#x}".format(targ.header.ctor_offset, base.header.ctor_offset)
    if base.header.dtor_offset != targ.header.dtor_offset:
        yield "Dtor offset mismatch: expected {:#x}, found {:#x}".format(targ.header.dtor_offset, base.header.dtor_offset)

    if base.header.export_count != targ.header.export_count:
        yield "Export count mismatch: expected {}, found {}".format(targ.header.export_count, base.header.export_count)
    else:
        for idx in range(base.header.export_count):
            base_export = base.header.export_offsets[idx]
            targ_export = targ.header.export_offsets[idx]
            if base_export != targ_export:
                yield "First export mismatch at idx {}: expected {:#x}, found {:#x}".format(idx, targ_export, base_export)
                break
    
    if base.get_text_size() != targ.get_text_size():
        yield ".text size mismatch: expected {:#x}, found {:#x}".format(targ.get_text_size(), base.get_text_size())
    else:
        for idx in range(base.get_text_size()):
            base_byte = base_data[base.header.size + idx]
            targ_byte = targ_data[targ.header.size + idx]
            if base_byte != targ_byte:
                yield "First .text mismatch at .text+{:#x}: expected 0x{:02x}, found 0x{:02x}".format(idx, targ_byte, base_byte)
                break
    
    if len(base.reloc_table.global_offset_table) != len(targ.reloc_table.global_offset_table):
        yield "GOT count mismatch: expected {}, found {}".format(len(targ.reloc_table.global_offset_table), len(base.reloc_table.global_offset_table))
    else:
        for idx in range(4, len(base.reloc_table.global_offset_table)):
            base_value = base.reloc_table.global_offset_table[idx]
            targ_value = targ.reloc_table.global_offset_table[idx]
            if base_value != targ_value:
                yield "First GOT mismatch at GOT[{}]: expected {:#x}, found {:#x}".format(idx, targ_value, base_value)
                break
    if len(base.reloc_table.data_relocations) != len(targ.reloc_table.data_relocations):
        yield "Data relocation count mismatch: expected {}, found {}".format(len(targ.reloc_table.data_relocations), len(base.reloc_table.data_relocations))
    else:
        for idx in range(len(base.reloc_table.data_relocations)):
            base_value = base.reloc_table.data_relocations[idx]
            targ_value = targ.reloc_table.data_relocations[idx]
            if base_value != targ_value:
                yield "First data relocation mismatch at GOT[{}]: expected {:#x}, found {:#x}".format(idx, targ_value, base_value)
                break
    if len(base.reloc_table.gp_relocations) != len(targ.reloc_table.gp_relocations):
        yield "$gp relocation count mismatch: expected {}, found {}".format(len(targ.reloc_table.gp_relocations), len(base.reloc_table.gp_relocations))
    else:
        for idx in range(len(base.reloc_table.gp_relocations)):
            base_value = base.reloc_table.gp_relocations[idx]
            targ_value = targ.reloc_table.gp_relocations[idx]
            if base_value != targ_value:
                yield "First $gp relocation mismatch at GOT[{}]: expected {:#x}, found {:#x}".format(idx, targ_value, base_value)
                break

    if base.get_rodata_size() != targ.get_rodata_size():
        yield ".rodata size mismatch: expected {:#x}, found {:#x}".format(targ.get_rodata_size(), base.get_rodata_size())
    else:
        base_rodata_offset = base.header.rodata_offset + base.reloc_table.get_size()
        targ_rodata_offset = targ.header.rodata_offset + targ.reloc_table.get_size()
        for idx in range(base.get_rodata_size()):
            base_byte = base_data[base_rodata_offset + idx]
            targ_byte = targ_data[targ_rodata_offset + idx]
            if base_byte != targ_byte:
                yield "First .rodata mismatch at .rodata+{:#x}: expected 0x{:02x}, found 0x{:02x}".format(idx, targ_byte, base_byte)
                break
    
    if base.get_data_size() != targ.get_data_size():
        yield ".data size mismatch: expected {:#x}, found {:#x}".format(targ.get_data_size(), base.get_data_size())
    else:
        for idx in range(base.get_data_size()):
            base_byte = base_data[base.header.data_offset + idx]
            targ_byte = targ_data[targ.header.data_offset + idx]
            if base_byte != targ_byte:
                yield "First .data mismatch at .data+{:#x}: expected 0x{:02x}, found 0x{:02x}".format(idx, targ_byte, base_byte)
                break
    
    if base_bss_size != targ_bss_size:
        yield ".bss size mismatch: expected {:#x}, found {:#x}".format(targ_bss_size, base_bss_size)

def diff(base_tab: DLLTab, targ_tab: DLLTab, number: str, quiet_match: bool):
    # Load DLLs
    target_dll_path = BIN_ASSETS_DIR.joinpath(f"dlls/{number}.dll")
    base_dll_path = BUILD_ASSETS_DIR.joinpath(f"dlls/{number}.dll")

    if not target_dll_path.exists():
        print(f"ERR: DLL not found @ {target_dll_path.absolute()}")
        exit(1)
    if not base_dll_path.exists():
        print(f"ERR: DLL not found @ {base_dll_path.absolute()}")
        exit(1)
    
    # Load DLL .bss sizes
    number_int = int(number)
    base_bss_size = base_tab.entries[number_int - 1].bss_size
    targ_bss_size = targ_tab.entries[number_int - 1].bss_size
    
    # Diff
    with open(target_dll_path, "rb") as target_file, \
         open(base_dll_path, "rb") as base_file:
        targ_data = target_file.read()
        base_data = base_file.read()
        target = DLL.parse(targ_data)
        base = DLL.parse(base_data)
        
        found_diff = False
        for diff in iter_diffs(base, base_data, base_bss_size, target, targ_data, targ_bss_size):
            if not found_diff:
                print(f"DLL {number}:")
            print(diff)
            found_diff = True
        
        if not found_diff:
            base_md5 = hashlib.md5(base_data).hexdigest()
            targ_md5 = hashlib.md5(targ_data).hexdigest()

            if base_md5 == targ_md5:
                if not quiet_match:
                    print(f"DLL {number}: OK.")
            else:
                print(f"DLL {number}:")
                print("MD5 mismatch: expected {}, found {}".format(targ_md5, base_md5))

def main():
    parser = argparse.ArgumentParser(description="Finds differences between an original and rebuilt DLL.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("dlls", nargs="*", action="extend", help="The number of the DLL to diff. Don't specify to diff all DLLs.")

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    dlls: "list[str]" = args.dlls
    quiet_match = len(dlls) == 0
    # Check all DLLs with a src directory if none were given
    if len(dlls) == 0:
        dlls_txt_path = SRC_DLLS_DIR.joinpath("dlls.txt")
        assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
        
        with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
            dlls_txt = DLLsTxt.parse(dlls_txt_file)
        
        for number, dll_dir in dlls_txt.path_map.items():
            src_dir = SRC_DLLS_DIR.joinpath(dll_dir)
            if src_dir.exists():
                dlls.append(str(number))

    # Load DLLS.tabs
    targ_dllstab_path = BIN_ASSETS_DIR.joinpath("DLLS_tab.bin")
    base_dllstab_path = BUILD_ASSETS_DIR.joinpath("DLLS_tab.bin")

    if not targ_dllstab_path.exists():
        print(f"ERR: DLLS.tab not found @ {targ_dllstab_path.absolute()}")
        exit(1)
    if not base_dllstab_path.exists():
        print(f"ERR: DLLS.tab not found @ {base_dllstab_path.absolute()}")
        exit(1)
    
    with open(targ_dllstab_path, "rb") as target_tab_file, \
         open(base_dllstab_path, "rb") as base_tab_file:
        target_tab = DLLTab.parse(target_tab_file.read())
        base_tab = DLLTab.parse(base_tab_file.read())
    
    # Diff DLLs
    for dll in dlls:
        diff(base_tab, target_tab, dll, quiet_match=quiet_match)

if __name__ == "__main__":
    main()
