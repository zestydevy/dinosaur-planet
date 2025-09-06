import argparse
from typing import TextIO

from dino.dll_syms_txt import DLLSymsTxt

def convert(syms_file: TextIO, ld_file: TextIO):
    syms = DLLSymsTxt.parse(syms_file)

    ld_file.write("SECTIONS\n")
    ld_file.write("{\n")

    for (addr, sym) in syms.absolute.items():
        if sym.local:
            ld_file.write("    HIDDEN({} = 0x{:X});\n".format(sym.name, addr))
        else:
            ld_file.write("    {} = 0x{:X};\n".format(sym.name, addr))
    
    for name, section in syms.sections.items():
        ld_file.write("    {} : {{\n".format(name))
        for (addr, sym) in section.syms.items():
            if sym.local:
                ld_file.write("        HIDDEN({} = 0x{:X});\n".format(sym.name, addr))
            else:
                ld_file.write("        {} = 0x{:X};\n".format(sym.name, addr))
        ld_file.write("    }\n")

    ld_file.write("}\n")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("syms", type=argparse.FileType("r"), help="The DLL syms.txt file to convert.")
    parser.add_argument("-o", "--output", type=argparse.FileType("w"), help="The path of the DLL syms linker script to output.", required=True)
    args = parser.parse_args()
    
    convert(args.syms, args.output)

    args.syms.close()
    args.output.close()

if __name__ == "__main__":
    main()
