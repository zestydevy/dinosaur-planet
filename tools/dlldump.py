#!/usr/bin/env python3

import argparse
from io import BufferedReader
from pathlib import Path

from dino.dll import DLL, DLLHeader, DLLRelocationTable, DLLFunction, parse_dll_functions

def dump_header(dll: DLL):
    print("HEADER")
    print("===================")

    header = dll.header
    text_size = dll.get_text_size()
    rodata_size = dll.get_rodata_size()
    rodata_rom_size = rodata_size + dll.reloc_table.get_size()
    data_size = dll.get_data_size()
    dll_size = dll.get_rom_size()

    print(f"DLL ROM size:       {hex(dll_size)} ({dll_size} bytes, unaligned)")
    print(f"Header size:        {hex(header.size)} ({header.size} bytes)")
    print(f"TEXT size:          {hex(text_size)} ({text_size} bytes)")
    print(f"DATA size:          {hex(data_size)} ({data_size} bytes)")
    print(f"RODATA size:        {hex(rodata_size)} ({rodata_size} bytes)")
    print(f"RODATA ROM size:    {hex(rodata_rom_size)} ({rodata_rom_size} bytes)")
    print()
    print(f"TEXT offset:        {hex(header.size)}")
    print(f"DATA offset:        {hex(header.data_offset)}{' (not present)' if header.data_offset == 0xFFFF_FFFF else ''}")
    print(f"RODATA offset:      {hex(header.rodata_offset)}{' (not present)' if header.rodata_offset == 0xFFFF_FFFF else ''}")
    print(f"Export count:       {hex(header.export_count)} ({header.export_count})")
    print(f"Constructor offset: {hex(header.ctor_offset)}")
    print(f"Destructor offset:  {hex(header.dtor_offset)}")
    print()
    print("Export offsets:")
    for offset in header.export_offsets:
        print(f"  {hex(offset)}")

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
        for offset in table.global_offset_table:
            if got_i == 0:
                print(f"  {hex(offset).ljust(12)}(.text)")
            elif got_i == 1:
                print(f"  {hex(offset).ljust(12)}(.rodata)")
            elif got_i == 2:
                print(f"  {hex(offset).ljust(12)}(.data)")
            elif got_i == 3:
                print(f"  {hex(offset).ljust(12)}(.bss)")
            else:
                print(f"  {hex(offset)}")
            got_i += 1
        
        if len(table.global_offset_table) == 0:
            print("  (none)")
        
        print("$gp relocations:")
        for offset in table.gp_relocations:
            print(f"  {hex(offset)}")

        if len(table.gp_relocations) == 0:
            print("  (none)")
        
        print("DATA relocations:")
        for offset in table.data_relocations:
            print(f"  {hex(offset)}")

        if len(table.data_relocations) == 0:
            print("  (none)")
    else:
        print("No GOT or relocation tables present.")

def dump_text_disassembly(dll_functions: "list[DLLFunction]",
                          only_symbols: "list[str] | None",
                          orig_operands: bool):
    print(".text")
    print("===================")
    
    first = True

    for func in dll_functions:
        if only_symbols is not None and not func.symbol in only_symbols:
            continue

        if not first:
            print()
        else:
            first = False
        
        print("glabel {}{}".format(func.symbol, " (static)" if func.is_static else ""))

        for i in func.insts:
            if i.label is not None:
                print(f"{i.label}:")
            
            mnemonic = (' ' + i.mnemonic) if i.is_branch_delay_slot else i.mnemonic

            if orig_operands and i.is_op_modified():
                print("0x{:x}:\t{:<11}{:<24} # (original: {})"
                    .format(i.address, mnemonic, i.op_str, i.original.op_str))
            else:
                print("0x{:x}:\t{:<11}{:<24}".format(i.address, mnemonic, i.op_str))
    
    if only_symbols is not None and first:
        print("(no matching symbols found)")

def main():
    parser = argparse.ArgumentParser(description="Display information from Dinosaur Planet DLLs.")
    parser.add_argument("dll", type=argparse.FileType("rb"), help="The Dinosaur Planet .dll file to read.")
    parser.add_argument("-x", "--header", action="store_true", help="Display the contents of the header.")
    parser.add_argument("-r", "--reloc", action="store_true", help="Display the contents of the relocation table.")
    parser.add_argument("-d", "--disassemble", action="store_true", help="Display assembler contents of the executable section.")
    parser.add_argument("--symbols", action="append", type=str, help="When disassembling, only show these symbols.")
    parser.add_argument("--orig", action="store_true", help="Also show unmodified instruction operands.")

    args = parser.parse_args()

    if not args.header and not args.reloc and not args.disassemble:
        print("At least one display option must be provided.")
        parser.print_help()
        return 

    with args.dll as dll_file:
        dll_file: BufferedReader
        number = Path(dll_file.name).name.split(".")[0]
        data = bytearray(dll_file.read())
        dll = DLL.parse(data, number=number)

        if args.header:
            dump_header(dll)
            print()
        
        if args.reloc:
            dump_relocation_table(dll)
            print()
        
        if args.disassemble:
            dll_functions = parse_dll_functions(data, dll)
            dump_text_disassembly(dll_functions, args.symbols, args.orig or False)
            print()

if __name__ == "__main__":
    main()
