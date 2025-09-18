#!/usr/bin/env python3

import argparse
import re
from io import BufferedReader
from pathlib import Path

from dino.dll import DLL
from dino.dll_analysis import DLLFunction, get_all_dll_functions
from dino.dll_code_printer import stringify_instruction
from dino.dll_symbols import DLLSymbols

symbol_pattern = re.compile(r"(\S+)\s*=\s*(\S+);")

def dump_header(dll: DLL):
    print("HEADER")
    print("===================")

    header = dll.header
    text_size = dll.get_text_size()
    rodata_size = dll.get_rodata_size()
    rodata_full_size = rodata_size + dll.reloc_table.get_size()
    data_size = dll.get_data_size()
    dll_size = dll.get_size()
    bss_offset = dll.get_bss_offset()

    print(f"DLL size:           {hex(dll_size)} ({dll_size} bytes)")
    print(f"DLL size (aligned): {hex(dll.size_aligned)} ({dll.size_aligned} bytes)")
    print(f"Header size:        {hex(header.size)} ({header.size} bytes)")
    print(f"TEXT size:          {hex(text_size)} ({text_size} bytes)")
    print(f"DATA size:          {hex(data_size)} ({data_size} bytes)")
    print(f"RODATA size:        {hex(rodata_size)} ({rodata_size} bytes)")
    print(f"RODATA full size:   {hex(rodata_full_size)} ({rodata_full_size} bytes)")
    print()
    print(f"TEXT offset:        {hex(header.size)}")
    print(f"DATA offset:        {hex(header.data_offset)}{' (not present)' if header.data_offset == 0xFFFF_FFFF else ''}")
    print(f"RODATA offset:      {hex(header.rodata_offset)}{' (not present)' if header.rodata_offset == 0xFFFF_FFFF else ''}")
    print(f"BSS offset:         {hex(bss_offset)}")
    print(f"Export count:       {hex(header.export_count)} ({header.export_count})")
    print(f"Constructor offset: {hex(header.ctor_offset)}")
    print(f"Destructor offset:  {hex(header.dtor_offset)}")
    print()
    print("Export offsets:")
    for i, offset in enumerate(header.export_offsets):
        print(f"{str(i)+":":<3} {hex(offset)}")

def dump_relocation_table(dll: DLL):
    print("RELOCATIONS")
    print("===================")

    if dll.reloc_table.exists:
        reloc_size = dll.reloc_table.get_size()
        print(f"Size: {hex(reloc_size)} ({reloc_size} bytes)")
        print()
        
        print("Global offset table:")
        table = dll.reloc_table
        got_i = 0
        for i, offset in enumerate(table.global_offset_table):
            comment: str | None = None
            if got_i == 0:
                comment = ".text"
            elif got_i == 1:
                comment = ".rodata"
            elif got_i == 2:
                comment = ".data"
            elif got_i == 3:
                comment = ".bss"
            elif (offset & 0x8000_0000) != 0:
                comment = f"import {offset & ~0x8000_0000}"
            elif offset >= 0x1_0000:
                comment = "local hi"
            else:
                if offset >= table.global_offset_table[3]:
                    comment = f".bss+0x{offset - table.global_offset_table[3]:X}"
                elif offset >= table.global_offset_table[2]:
                    comment = f".data+0x{offset - table.global_offset_table[2]:X}"
                elif offset >= table.global_offset_table[1]:
                    comment = f".rodata+0x{offset - table.global_offset_table[1]:X}"
                elif offset >= table.global_offset_table[0]:
                    comment = f".text+0x{offset - table.global_offset_table[0]:X}"
            
            if comment != None:
                print(f"{str(i)+":":<3} {hex(offset).ljust(12)}({comment})")
            else:
                print(f"{str(i)+":":<3} {hex(offset)}")
            got_i += 1
        
        if len(table.global_offset_table) == 0:
            print("  (none)")
        
        print()
        print("$gp relocations:")
        for i, offset in enumerate(table.gp_relocations):
            print(f"{str(i)+":":<3} {hex(offset)}")

        if len(table.gp_relocations) == 0:
            print("  (none)")
        
        print()
        print("DATA relocations:")
        for i, offset in enumerate(table.data_relocations):
            print(f"{str(i)+":":<3} {hex(offset)}")

        if len(table.data_relocations) == 0:
            print("  (none)")
    else:
        print("No GOT or relocation tables present.")

def dump_text_disassembly(dll_functions: "list[DLLFunction]",
                          only_funcs: "list[str] | None"):
    print(".text")
    print("===================")
    
    first = True

    for func in dll_functions:
        if only_funcs is not None and not func.symbol in only_funcs:
            continue

        if not first:
            print()
        else:
            first = False

        print("glabel {} ({})".format(func.symbol, func.type.name.lower()))

        for i in func.insts:
            inst_str, label = stringify_instruction(i, func)
            if label != None:
                print(label)
            print("{:#x}:\t{}".format(i.i.vram, inst_str))
    
    if only_funcs is not None and first:
        print("(function(s) not found)")

def read_syms(sym_files: list[str] | None):
    if sym_files == None:
        return {}
    
    map: "dict[int, str]" = {}

    for path in sym_files:
        with open(path, "r", encoding="utf-8") as syms_file:
            for line in syms_file.readlines():
                pairs = symbol_pattern.findall(line.strip())
                for pair in pairs:
                    addr_str: str = pair[1]
                    if addr_str.lower().startswith("0x"):
                        addr = int(addr_str, base=16)
                    else:
                        addr = int(addr_str)
                    map[addr] = pair[0]
    
    return map

def main():
    parser = argparse.ArgumentParser(description="Display information from Dinosaur Planet DLLs.")
    parser.add_argument("dll", type=argparse.FileType("rb"), help="The Dinosaur Planet .dll file to read.")
    parser.add_argument("-x", "--header", action="store_true", help="Display the contents of the header.")
    parser.add_argument("-r", "--reloc", action="store_true", help="Display the contents of the relocation table.")
    parser.add_argument("-d", "--disassemble", action="store_true", help="Display assembler contents of the executable section.")
    parser.add_argument("-f", "--funcs", action="append", type=str, help="When disassembling, only show these functions.")
    parser.add_argument("-s", "--syms", action="append", type=str, help="Read known symbols from these files.")
    parser.add_argument("--orig", action="store_true", help="Show original unanalyzed assembly.")

    args = parser.parse_args()

    if not args.header and not args.reloc and not args.disassemble:
        print("At least one display option must be provided.")
        parser.print_help()
        return 

    with args.dll as dll_file:
        dll_file: BufferedReader
        number = Path(dll_file.name).name.split(".")[0]
        data = bytearray(dll_file.read())
        dll = DLL.parse(data)

        if args.header:
            dump_header(dll)
            print()
        
        if args.reloc:
            dump_relocation_table(dll)
            print()
        
        if args.disassemble:
            known_symbols = read_syms(args.syms)
            dll_symbols = DLLSymbols(dll, number, known_symbols)

            dll_functions = get_all_dll_functions(data, dll, dll_symbols, 
                                                  analyze=not args.orig)
            dump_text_disassembly(dll_functions, args.funcs)
            print()

if __name__ == "__main__":
    main()
