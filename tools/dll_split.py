#!/usr/bin/env python3
# A mini version of splat specifically for Dinosaur Planet DLLs
# dll_split.py is responsible for:
# - Extracting assembly and data from Dinosaur Planet DLLs
# - Extracting in such a way that is ready for the decomp project's build system
# - Creating .c, exports.s, and syms.txt stubs

import argparse
from genericpath import isdir
import glob
import os
from pathlib import Path
import re
import struct

import yaml

from dino.dll import DLL, DLLHeader, DLLRelocationTable, DLLFunction
from dino.dll_tab import DLLTab

ASM_PATH = Path("asm")
BIN_PATH = Path("bin")
SRC_PATH = Path("src")

def create_exports_s(path: Path, dll: DLL):
    assert dll.functions is not None

    funcs_by_address: "dict[int, str]" = {}
    for func in dll.functions:
        funcs_by_address[func.address] = func.symbol

    with open(path, "w", encoding="utf-8") as exports_s:
        exports_s.write(".option pic2\n")
        exports_s.write(".section \".exports\"\n")
        exports_s.write(".global _exports\n")
        exports_s.write("_exports:\n")
        exports_s.write("\n")

        exports_s.write(f".dword ctor\n")
        exports_s.write(f".dword dtor\n")
        exports_s.write("\n")

        for offset in dll.header.export_offsets:
            func_symbol = funcs_by_address[offset]
            exports_s.write(f".dword {func_symbol}\n")

def create_c_stub(c_path: Path, asm_path: Path, functions: "list[DLLFunction]"):
    with open(c_path, "w", encoding="utf-8") as c_file:
        c_file.write("#include <PR/ultratypes.h>\n")
        
        for func in functions:
            c_file.write("\n")
            c_file.write(f'#pragma GLOBAL_ASM("{asm_path}/{func.symbol}.s")\n')

def create_syms_txt(syms_path: Path, dll: DLL):
    assert dll.functions is not None

    with open(syms_path, "w", encoding="utf-8") as syms_file:
        addrs_found: "set[int]" = set()
        syms_added = 0
        for func in dll.functions:
            for name, value in func.auto_symbols.items():
                if not value in addrs_found:
                    addrs_found.add(value)
                    syms_file.write("{} = 0x{:X};\n".format(name, value))
                    syms_added += 1
        
        assert syms_added == max(0, len(dll.reloc_table.global_offset_table) - 4)

def extract_text_asm(dir: Path, dll: DLL, funcs: "list[str] | None"):
    assert dll.functions is not None
    functions = dll.functions

    for func in functions:
        if funcs is not None and not func.symbol in funcs:
            continue

        s_path = dir.joinpath(f"{func.symbol}.s")
        with open(s_path, "w", encoding="utf-8") as s_file:
            s_file.write(f"glabel {func.symbol}\n")

            for i in func.insts:
                if i.label is not None:
                    s_file.write(f"{i.label}:\n")

                rom_addr = i.address + dll.header.size
                ram_addr = i.address
                inst_bytes = i.original.bytes.hex().upper()
                mnemonic = (' ' + i.mnemonic) if i.is_branch_delay_slot else i.mnemonic
                
                s_file.write("/* {:0>4X} {:0>6X} {} */ {:<11}{}\n"
                    .format(rom_addr, ram_addr, inst_bytes, mnemonic, i.op_str))

def extract_rodata_asm(dir: Path, dll: DLL, data: bytearray):
    rodata_path = dir.joinpath(f"{dll.number}.rodata.s")
    with open(rodata_path, "w", encoding="utf-8") as rodata_file:
        # Set .rodata section
        rodata_file.write(".section .rodata, \"a\"\n")
        # Write data
        rodata_rom_offset = dll.header.rodata_offset + dll.reloc_table.get_size()
        rodata_ram_offset = dll.header.rodata_offset - dll.header.size
        for i in range(0, len(data), 4):
            word = struct.unpack_from(">I", data, offset=i)[0]

            rom_addr = rodata_rom_offset + i
            ram_addr = rodata_ram_offset + i
            
            rodata_file.write("/* {:0>4X} {:0>6X} */ .4byte 0x{:X}\n"
                .format(rom_addr, ram_addr, word))

def extract_data_asm(dir: Path, dll: DLL, data: bytearray):
    data_path = dir.joinpath(f"{dll.number}.data.s")
    with open(data_path, "w", encoding="utf-8") as data_file:
        # Set .data section
        data_file.write(".data\n")
        # Write relocations
        for offset in dll.reloc_table.data_relocations:
            data_file.write(".reloc 0x{:X}, \"R_MIPS_32\", .data\n".format(offset))
        # Write data
        data_rom_offset = dll.header.data_offset
        data_ram_offset = dll.header.data_offset - dll.header.size - dll.reloc_table.get_size()
        for i in range(0, len(data), 4):
            word = struct.unpack_from(">I", data, offset=i)[0]

            rom_addr = data_rom_offset + i
            ram_addr = data_ram_offset + i
            
            data_file.write("/* {:0>4X} {:0>6X} */ .4byte 0x{:X}\n"
                .format(rom_addr, ram_addr, word))

def extract_bss_asm(dir: Path, dll: DLL, bss_size: int):
    assert bss_size > 0

    bss_path = dir.joinpath(f"{dll.number}.bss.s")
    with open(bss_path, "w", encoding="utf-8") as bss_file:
        # Set .bss section
        bss_file.write(".bss\n")
        # Write .bss size
        bss_file.write(".ds.s 0x{:X}, 0\n".format(bss_size))

def get_functions_to_extract(path: Path, dll_number: str) -> "list[str] | None":
    """Returns None if all functions should be extracted (i.e. there is no .c file to derive the list from)"""
    if not path.exists():
        return None
    
    emit_funcs: "list[str]" = []
    global_asm_pattern = re.compile(rf"#pragma GLOBAL_ASM\(\"asm\/nonmatchings\/dlls\/{dll_number}\/(.+)\.s\"\)")
    with open(path, "r", encoding="utf-8") as c_file:
        for line in c_file.readlines():
            symbols = global_asm_pattern.findall(line.strip())
            for symbol in symbols:
                emit_funcs.append(symbol)
    
    return emit_funcs


def extract_dll(dll: DLL, 
                data: bytearray, 
                bss_size: int,
                skip_data: bool,
                skip_rodata: bool,
                skip_bss: bool):
    assert dll.functions is not None

    # Determine paths
    src_path = SRC_PATH.joinpath(f"dlls/{dll.number}")
    asm_path = ASM_PATH.joinpath(f"nonmatchings/dlls/{dll.number}")
    asm_data_path = ASM_PATH.joinpath(f"nonmatchings/dlls/{dll.number}/data")

    # Determine what needs to be extracted
    c_file_path = src_path.joinpath(f"{dll.number}.c")
    emit_funcs = get_functions_to_extract(c_file_path, dll.number)

    rodata_size = dll.get_rodata_size()
    data_size = dll.get_data_size()

    emit_rodata = not skip_rodata and dll.has_rodata() and rodata_size > 0
    emit_data = not skip_data and dll.has_data() and data_size > 0
    emit_bss = not skip_bss and bss_size > 0

    # Create directories if necessary
    if emit_funcs is None or len(emit_funcs) > 0:
        os.makedirs(asm_path, exist_ok=True)
    if emit_rodata or emit_data or emit_bss:
        os.makedirs(asm_data_path, exist_ok=True)

    # Extract .text
    if emit_funcs is None or len(emit_funcs) > 0:
        extract_text_asm(asm_path, dll, emit_funcs)

    # Extract .rodata
    if emit_rodata:
        rodata_start = dll.header.rodata_offset + dll.reloc_table.get_size() # exclude relocation tables
        rodata_end = rodata_start + rodata_size
        extract_rodata_asm(asm_data_path, dll, data[rodata_start:rodata_end])
    
    # Extract .data
    if emit_data:
        data_start = dll.header.data_offset
        data_end = data_start + data_size
        extract_data_asm(asm_data_path, dll, data[data_start:data_end])
    
    # Extract .bss
    if emit_bss:
        extract_bss_asm(asm_data_path, dll, bss_size)

    # Create exports.s if it doesn't exist
    exports_s_path = src_path.joinpath("exports.s")
    if not exports_s_path.exists():
        create_exports_s(exports_s_path, dll)

    # Create syms.txt if it doens't exist
    syms_txt_path = src_path.joinpath("syms.txt")
    if not syms_txt_path.exists():
        create_syms_txt(syms_txt_path, dll)

    # Create <dll>.c stub if it doesn't exist
    if not c_file_path.exists():
        create_c_stub(c_file_path, asm_path, dll.functions)

def extract_dlls(only_dlls: "list[str]"):
    # Load DLLS.tab
    tab_path = BIN_PATH.joinpath("assets/DLLS_tab.bin")
    with open(tab_path, "rb") as tab_file:
        tab = DLLTab.parse(bytearray(tab_file.read()))
    
    # Extract each DLL that has a src directory
    dll_dirs = [Path(dir) for dir in glob.glob(f"{SRC_PATH}/dlls/*") if isdir(dir)]
    for dir in dll_dirs:
        number = dir.name

        # Skip DLL if not in list
        if len(only_dlls) > 0 and not number in only_dlls:
            continue

        # Load DLL config
        dll_config_path = dir.joinpath(f"{number}.yaml")
        if not dll_config_path.exists():
            print(f"WARN: Missing {dll_config_path}!")
            continue
        
        with open(dll_config_path, "r") as file:
            dll_config = yaml.safe_load(file)

        link_original_rodata = "link_original_rodata" in dll_config and dll_config["link_original_rodata"] or False
        link_original_data = "link_original_data" in dll_config and dll_config["link_original_data"] or False
        link_original_bss = "link_original_bss" in dll_config and dll_config["link_original_bss"] or False

        # Load DLL
        dll_path = BIN_PATH.joinpath(f"assets/dlls/{number}.dll")
        if not dll_path.exists():
            print(f"WARN: No such DLL {dll_path}!")
            continue

        with open(dll_path, "rb") as dll_file:
            data = bytearray(dll_file.read())
            dll = DLL.parse(data, number)
            bss_size = tab.entries[int(number) - 1].bss_size
        
            # Extract DLL
            extract_dll(dll, data, 
                bss_size=bss_size,
                skip_rodata=not link_original_rodata,
                skip_data=not link_original_data,
                skip_bss=not link_original_bss,
            )

def parse_dll_yaml(path: Path):
    with open(path, "r") as file:
        return yaml.safe_load(file)

def main():
    parser = argparse.ArgumentParser(description="Extract assembly and data from Dinosaur Planet DLLs and stub out an environment for recompiling each.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project (default=..).", default="..")
    parser.add_argument("dlls", nargs="*", action="extend", help="The numbers of each DLL to extract. Don't specify any to extract all that have src directories.")

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    # Extract DLLs
    extract_dlls(args.dlls)

if __name__ == "__main__":
    main()
