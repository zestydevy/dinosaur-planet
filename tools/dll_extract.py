#!/usr/bin/env python3

import argparse
from io import BufferedReader
import os
from pathlib import Path

from dino.dll import DLL, DLLHeader, DLLRelocationTable, DLLFunction

def create_exports_s(path: Path, functions: "list[DLLFunction]"):
    with open(path, "w", encoding="utf-8") as exports_s:
        exports_s.write(".option pic2\n")
        exports_s.write(".section \".exports\"\n")
        exports_s.write(".global _exports\n")
        exports_s.write("_exports:\n")
        exports_s.write("\n")

        for func in functions:
            exports_s.write(f".dword {func.symbol}\n")

def create_c_stub(c_path: Path, asm_path: Path, functions: "list[DLLFunction]"):
    with open(c_path, "w", encoding="utf-8") as c_file:
        c_file.write("#include <PR/ultratypes.h>\n")
        
        for func in functions:
            c_file.write("\n")
            c_file.write(f'#pragma GLOBAL_ASM("{asm_path}/{func.symbol}.s")\n')

def create_syms_txt(syms_path: Path, dll: DLL):
    assert dll.functions is not None

    with open(syms_path, "w", encoding="utf-8") as syms_file:
        syms_added = 0
        for func in dll.functions:
            for name, value in func.auto_symbols.items():
                syms_file.write("%s = 0x%X;\n" %(name, value))
                syms_added += 1
        
        assert syms_added == len(dll.reloc_table.global_offset_table) - 4

def extract_asm(dir: Path, dll: DLL):
    assert dll.functions is not None
    functions = dll.functions

    for func in functions:
        s_path = dir.joinpath(f"{func.symbol}.s")
        with open(s_path, "w", encoding="utf-8") as s_file:
            s_file.write("glabel %s\n" %(func.symbol))

            for i in func.insts:
                if i.label is not None:
                    s_file.write("%s:\n" %(i.label))
                s_file.write(
                    "/* %04X %X %s */ %s%s%s\n" 
                    %(
                        (i.address + dll.header.header_size) - 0x8000_0000,
                        i.address, 
                        i.original.bytes.hex().upper(), 
                        ' ' if i.is_branch_delay_slot else '', 
                        i.mnemonic.ljust(10 if i.is_branch_delay_slot else 11), 
                        i.op_str))

def extract_data(dir: Path, dll: DLL, data: bytearray):
    # Calculate section sizes
    # TODO: This all assumes that the first four entires of the GOT are .text, .rodata, .data, and .bss!
    #       Can we verify this?
    rodata_size = dll.reloc_table.global_offset_table[2] - dll.reloc_table.global_offset_table[1]
    data_size = dll.reloc_table.global_offset_table[3] - dll.reloc_table.global_offset_table[2]

    # Calculate section starts/ends
    rodata_start = dll.header.rodata_offset + dll.reloc_table.size() # exclude relocation table
    rodata_end = rodata_start + rodata_size
    data_start = dll.header.data_offset
    data_end = data_start + data_size
    bss_start = data_end
    bss_end = len(data)

    # Write .rodata
    rodata_path = dir.joinpath(f"{dll.number}.rodata.bin")
    with open(rodata_path, "wb") as rodata_file:
        rodata_file.write(data[rodata_start:rodata_end])
    
    # Write .data
    data_path = dir.joinpath(f"{dll.number}.data.bin")
    with open(data_path, "wb") as data_file:
        data_file.write(data[data_start:data_end])
    
    # Write .bss
    bss_path = dir.joinpath(f"{dll.number}.bss.bin")
    with open(bss_path, "wb") as bss_file:
        bss_file.write(data[bss_start:bss_end])

def extract(dll: DLL, data: bytearray, src_path: Path, asm_path: Path):
    assert dll.functions is not None

    # Create directories if they don't exist
    os.makedirs(src_path, exist_ok=True)
    os.makedirs(asm_path, exist_ok=True)

    # Extract assembly
    extract_asm(asm_path, dll)

    # Extract .data and .rodata
    # TODO: where should these files go?
    extract_data(src_path, dll, data)

    # Create exports.s if it doesn't exist
    exports_s_path = src_path.joinpath("exports.s")
    if not exports_s_path.exists():
        create_exports_s(exports_s_path, dll.functions)

    # Create syms.txt if it doens't exist
    syms_txt_path = src_path.joinpath("syms.txt")
    if not syms_txt_path.exists():
        create_syms_txt(syms_txt_path, dll)

    # Create <dll>.c stub if it doesn't exist
    c_file_path = src_path.joinpath(f"{dll.number}.c")
    if not c_file_path.exists():
        create_c_stub(c_file_path, asm_path, dll.functions)

def main():
    parser = argparse.ArgumentParser(description="Extract assembly from a Dinosuar Planet DLL and set up a directory for it that is ready for recompilation.")
    parser.add_argument("dll", type=argparse.FileType("rb"), help="The Dinosuar Planet .dll file to read.")
    parser.add_argument("--src", type=str, help="A directory to create source file stubs in (e.g. ./src/dlls/12).", required=True)
    parser.add_argument("--asm", type=str, help="A directory extract assembly code into (e.g. ./asm/nonmatchings/dlls/12).", required=True)

    args = parser.parse_args()

    with args.dll as dll_file:
        dll_file: BufferedReader
        # Read DLL
        number = Path(dll_file.name).name.split(".")[0]
        data = bytearray(dll_file.read())
        dll = DLL.parse(data, number)
        # Extract
        extract(dll, data, Path(args.src), Path(args.asm))

if __name__ == "__main__":
    main()
