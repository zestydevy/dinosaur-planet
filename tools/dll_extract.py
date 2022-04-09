#!/usr/bin/env python3

import argparse
from io import BufferedReader
import os
from pathlib import Path

from dino.dll import DLLHeader, DLLRelocationTable, DLLFunction, parse_functions

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

def create_syms_txt(syms_path: Path, reloc_table: DLLRelocationTable):
    with open(syms_path, "w", encoding="utf-8") as syms_file:
        for offset in reloc_table.global_offset_table:
            if offset >= 0x8000_0000:
                syms_file.write("D_%X = 0x%X;\n" %(offset, offset))

def extract_asm(dir: Path, functions: "list[DLLFunction]"):
    for func in functions:
        s_path = dir.joinpath(f"{func.symbol}.s")
        with open(s_path, "w", encoding="utf-8") as s_file:
            s_file.write("glabel %s\n" %(func.symbol))

            for i in func.insts:
                if i.label != None:
                    s_file.write("%s:\n" %(i.label))
                s_file.write(
                    "/* %X %s */ %s%s%s\n" 
                    %(
                        i.address, 
                        i.original.bytes.hex().upper(), 
                        ' ' if i.is_branch_delay_slot else '', 
                        i.mnemonic.ljust(11), 
                        i.op_str))

def extract(number: str,
            header: DLLHeader, 
            reloc_table: DLLRelocationTable, 
            functions: "list[DLLFunction]",
            src_path: Path,
            asm_path: Path):
    # Create directories if they don't exist
    os.makedirs(src_path, exist_ok=True)
    os.makedirs(asm_path, exist_ok=True)

    # Extract assembly
    extract_asm(asm_path, functions)

    # Create exports.s if it doesn't exist
    exports_s_path = src_path.joinpath("exports.s")
    if not exports_s_path.exists():
        create_exports_s(exports_s_path, functions)

    # Create syms.txt if it doens't exist
    syms_txt_path = src_path.joinpath("syms.txt")
    if not syms_txt_path.exists():
        create_syms_txt(syms_txt_path, reloc_table)

    # Create <dll>.c stub if it doesn't exist
    c_file_path = src_path.joinpath(f"{number}.c")
    if not c_file_path.exists():
        create_c_stub(c_file_path, asm_path, functions)

def main():
    parser = argparse.ArgumentParser(description="Extract assembly from a Dinosuar Planet DLL and set up a directory for it that is ready for recompilation.")
    parser.add_argument("dll", type=argparse.FileType("rb"), help="The Dinosuar Planet .dll file to read.")
    parser.add_argument("--src", type=str, help="A directory to create source file stubs in (e.g. ./src/dlls/12).", required=True)
    parser.add_argument("--asm", type=str, help="A directory extract assembly code into (e.g. ./asm/nonmatchings/dlls/12).", required=True)

    args = parser.parse_args()

    with args.dll as dll:
        dll: BufferedReader
        # Read DLL
        number = Path(dll.name).name.split(".")[0]
        data = bytearray(dll.read())
        header = DLLHeader.parse(data)
        relocation_table = DLLRelocationTable.parse(data, header)
        functions = parse_functions(data, header)
        # Extract
        extract(number, header, relocation_table, functions, Path(args.src), Path(args.asm))

if __name__ == "__main__":
    main()
