#!/usr/bin/env python3

import argparse
from io import BufferedReader

from dino.dll_tab import DLLTab, DLLTabEntry

def dump_header(tab: DLLTab):
    print("HEADER")
    print("===================")
    
    print("BANK  NAME     DLLS       COUNT")
    for i, bank in enumerate(tab.banks):
        print("{:<6d}{:<9s}{:<3d} - {:<5d}{:d}"
            .format(i, tab.bank_names[i], bank[0] + 1, bank[1] + 1, (bank[1] + 1) - bank[0]))
    print()
    print(f"Total DLLs: {len(tab.entries)}")

def dump_bank(tab: DLLTab, index: int):
    print(f"BANK {index} ({tab.bank_names[index]})")
    print("===================")
    write_entry_table_header()

    start, end = tab.banks[index]
    for i, entry in enumerate(tab.entries[start:end + 1]):
        dump_entry(entry, num=i + start + 1)

def write_entry_table_header():
    print("DLL   START      END        BSS SIZE")

def dump_entry(entry: DLLTabEntry, num: int):
    print("{:<6d}0x{:<9X}0x{:<9X}0x{:X}".format(num, entry.start_offset, entry.end_offset, entry.bss_size))

def main():
    parser = argparse.ArgumentParser(description="Display information from the Dinosaur Planet DLLS.tab file.")
    parser.add_argument("tab", type=argparse.FileType("rb"), help="The DLLS.tab file.")
    parser.add_argument("-a", "--all", action="store_true", default=False, help="Display all information in the tab file.")
    parser.add_argument("-x", "--header", action="store_true", default=False, help="Display the tab header.")
    parser.add_argument("-d", "--dll", action="append", help="Display the tab entry for the given DLL(s).")
    parser.add_argument("-b", "--bank", action="append", help="Display all DLLs in the given bank(s).")
    args = parser.parse_args()

    if not args.all and not args.header and args.dll is None and args.bank is None:
        print("At least one display option must be provided.")
        parser.print_help()
        return

    with args.tab as tab_file:
        tab_file: BufferedReader
        data = bytearray(tab_file.read())
        tab = DLLTab.parse(data)

        if args.all or args.header:
            dump_header(tab)
            print()

        if args.all:
            for i in range(0, len(tab.banks)):
                dump_bank(tab, i)
                print()
        elif args.bank is not None:
            banks = args.bank
            for bank in [int(bank) for bank in banks]:
                if bank < 0 or bank >= len(tab.banks):
                    print(f"No such bank: {bank}")
                else:
                    dump_bank(tab, bank)
                print()
        elif args.dll is not None:
            dlls = args.dll
            write_entry_table_header()
            for dll in [int(dll) for dll in dlls]:
                if dll < 1 or dll > len(tab.entries):
                    print(f"No such DLL: {dll}")
                else:
                    dump_entry(tab.entries[dll - 1], dll)
            print()

if __name__ == "__main__":
    main()
