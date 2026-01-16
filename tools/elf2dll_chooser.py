#!/usr/bin/env python3
import argparse
from elftools.elf.elffile import ELFFile

import elf2dll
import elf2dll_wrapper

def main():
    parser = argparse.ArgumentParser(description="Runs either elf2dll or elf2dll_wrapper depending on the state of the DLL. Useful for modding.")
    parser.add_argument("elf", type=argparse.FileType("rb"), help="The DLL .elf file to convert.")
    parser.add_argument("-o", "--output", type=argparse.FileType("wb"), help="The path of the Dinosaur Planet DLL file to output.", required=True)
    parser.add_argument("-b", "--bss", type=argparse.FileType("w", encoding="utf-8"), help="Path to output the .bss size as a text file.", required=True)
    parser.add_argument("-s", "--syms-map", dest="syms_output", type=argparse.FileType("w", encoding="utf-8"), help="Path to output symbol mapping for debugging.")
    args = parser.parse_args()

    error = False
    with ELFFile(args.elf) as obj:
        orig_got = obj.get_section_by_name(".orig_got")

        try:
            if orig_got == None:
                elf2dll.convert(obj, args.output, args.bss, args.syms_output)
            else:
                elf2dll_wrapper.convert(obj, args.output, args.bss, args.syms_output)
        except elf2dll.ELF2DLLException as ex:
            print(f"ERROR: {ex}")
            error = True

    # Done (elf is already closed by the above 'with')
    args.output.close()
    args.bss.close()

    if error:
        exit(1)

if __name__ == "__main__":
    main()
