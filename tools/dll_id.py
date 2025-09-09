#!/usr/bin/env python3

import argparse

def main():
    parser = argparse.ArgumentParser(description="Converts between Dinosaur Planet DLL numbers and IDs.")
    parser.add_argument("ids_and_numbers", nargs="*", action="extend", help="One or more DLL IDs and/or numbers to list.")
    args = parser.parse_args()
    
    # Convert
    print("ID      Number  Bank")
    for id_or_number in [int(x, base=0) for x in args.ids_and_numbers]:
        # DLL ID was given
        if id_or_number >= 0x8000:
            id = id_or_number
            number = (id_or_number - 0x8000) + 209
            bank = 3
        elif id_or_number >= 0x2000:
            id = id_or_number
            number = (id_or_number - 0x2000) + 185 + 1
            bank = 2
        elif id_or_number >= 0x1000:
            id = id_or_number
            number = (id_or_number - 0x1000) + 103 + 1
            bank = 1
        # DLL number was given
        elif id_or_number >= 210:
            id = (id_or_number - 209) + 0x8000
            number = id_or_number
            bank = 3
        elif id_or_number >= (185 + 1):
            id = (id_or_number - (185 + 1)) + 0x2000
            number = id_or_number
            bank = 2
        elif id_or_number >= (103 + 1):
            id = (id_or_number - (103 + 1)) + 0x1000
            number = id_or_number
            bank = 1
        else:
            id = id_or_number
            number = id_or_number
            bank = 0
        
        print("0x{:<5x} {:<7} {}".format(id, number, bank))

if __name__ == "__main__":
    main()
