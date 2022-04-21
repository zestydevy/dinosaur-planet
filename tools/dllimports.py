#!/usr/bin/env python3

import argparse
from io import BufferedReader
import struct

class DLLImportsTab:
    def __init__(self, imports: "list[int]") -> None:
        self.imports = imports
        """A mapping of import index to base executable pointer. Each pointer
        refers to non-DLL VRAM (e.g. >=0x80000000)"""

    @staticmethod
    def parse(data: bytearray) -> "DLLImportsTab":
        imports: "list[int]" = []

        for i in range(0, len(data), 4):
            ptr = struct.unpack_from(">I", data, offset=i)[0]
            imports.append(ptr)

        return DLLImportsTab(imports)

def dump_all(imports: DLLImportsTab):
    for i, ptr in enumerate(imports.imports):
        print("{}: 0x{:X}".format(i, ptr))

def lookup_pointer(imports: DLLImportsTab, index: int):
    if index > 0x8000_0000:
        index = index & 0x7999_9999
    
    if index < 0 or index >= len(imports.imports):
        print(f"Index {index} out of range [0, {len(imports.imports) - 1}]")
    else:
        ptr = imports.imports[index]
        print("{}: 0x{:X}".format(index, ptr))

def lookup_index(imports: DLLImportsTab, pointer: int):
    try:
        index = imports.imports.index(pointer)
        print("{}: 0x{:X}".format(index, pointer))
    except ValueError:
        print("Pointer not found: 0x{:X}".format(pointer))

def parse_int(str: str):
    if str.lower().startswith("0x"):
        return int(str[2:], base=16)
    else:
        return int(str)

def main():
    parser = argparse.ArgumentParser(description="Query the Dinosaur Planet DLLSIMPORTTAB file.")
    parser.add_argument("imports", type=argparse.FileType("rb"), help="The DLLSIMPORTTAB file.")
    parser.add_argument("-a", "--all", action="store_true", help="Display all DLL import mappings.", default=False)
    parser.add_argument("-p", "--pointer", action="append", dest="pointers", help="Find the index of the given pointer.")
    parser.add_argument("-i", "--index", action="append", dest="indexes", help="Lookup the pointer at the given index.")
    args = parser.parse_args()

    if not args.all and not args.pointers and not args.indexes:
        print("At least one query option must be provided.")
        parser.print_help()
        return

    with args.imports as imports_file:
        imports_file: BufferedReader
        data = bytearray(imports_file.read())
        imports = DLLImportsTab.parse(data)

        if args.all:
            dump_all(imports)
        else:
            if args.indexes:
                for index in args.indexes:
                    lookup_pointer(imports, parse_int(index))
            if args.pointers:
                for pointer in args.pointers:
                    lookup_index(imports, parse_int(pointer))

if __name__ == "__main__":
    main()

