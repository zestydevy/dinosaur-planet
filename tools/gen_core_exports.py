#!/usr/bin/env python3
# Generates export_symbol_addrs.txt by matching DLLSIMPROT.tab entries to
# symbols defined in the various top-level symbol .txt files.

from pathlib import Path
import re
from typing import TextIO

from dino.dll_imports_tab import DLLImportsTab

symbol_pattern = re.compile(r"(\S+)\s*=\s*(\S+);")

def main():
    path = Path("bin/assets/DLLSIMPORTTAB.bin")
    with open(path, "rb") as file:
        imports = DLLImportsTab.parse(file.read())
    
    map: "dict[int, list[str]]" = {}

    def read_syms(file: TextIO):
        for line in file.readlines():
            pairs = symbol_pattern.findall(line.strip())
            for pair in pairs:
                addr_str: str = pair[1]
                if addr_str.lower().startswith("0x"):
                    addr = int(addr_str, base=16)
                else:
                    addr = int(addr_str)
                map.setdefault(addr, []).append(pair[0])

    with open("symbol_addrs.txt", "r", encoding="utf-8") as syms_file:
        read_syms(syms_file)
    with open("undefined_funcs.txt", "r", encoding="utf-8") as syms_file:
        read_syms(syms_file)
    with open("undefined_funcs_auto.txt", "r", encoding="utf-8") as syms_file:
        read_syms(syms_file)
    with open("undefined_syms.txt", "r", encoding="utf-8") as syms_file:
        read_syms(syms_file)
    with open("undefined_syms_auto.txt", "r", encoding="utf-8") as syms_file:
        read_syms(syms_file)
    
    with open("export_symbol_addrs.txt", "w", encoding="utf-8") as out:
        for i, addr in enumerate(imports.imports):
            comment: str | None = None
            is_func = addr < 0x80089750 # addr < .data
            if is_func:
                comment = "type:func"
            syms = map.get(addr)
            if syms == None:
                if is_func:
                    sym = "func_{:X}".format(addr)
                else:
                    sym = "D_{:X}".format(addr)
            else:
                sym = syms.pop(0)
            import_addr = (i + 1) | 0x80000000
            if comment == None:
                out.write("{} = 0x{:X};\n".format(sym, import_addr))
            else:
                out.write("{} = 0x{:X}; // {}\n".format(sym, import_addr, comment))

if __name__ == "__main__":
    main()
