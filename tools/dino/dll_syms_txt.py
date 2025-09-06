from collections import OrderedDict
from typing import TextIO
import re

SYMBOL_LINE_REGEX = re.compile(r"(?:(local)\s+)?([\.\w]+)\s*=\s*(\w+);")

class DLLSym:
    def __init__(self, name: str, local: bool):
        self.name = name
        self.local = local

class DLLSymSection:
    def __init__(self, offset: int, syms: OrderedDict[int, DLLSym]):
        self.offset = offset
        self.syms = syms

class DLLSymsTxt:
    """
    Parser for DLL syms.txt files
    """
    
    def __init__(self, 
                 absolute: "OrderedDict[int, DLLSym]", 
                 sections: "OrderedDict[str, DLLSymSection]"):
        self.absolute = absolute 
        self.sections = sections 

    def to_absolute(self) -> "OrderedDict[int, str]":
        abs = self.absolute.copy()
        for section in self.sections.values():
            for (sym_addr, sym) in section.syms.items():
                abs[section.offset + sym_addr] = sym.name
        
        return abs

    @staticmethod
    def parse(file: TextIO):
        absolute: "OrderedDict[int, DLLSym]" = OrderedDict()
        sections: "OrderedDict[str, DLLSymSection]" = OrderedDict()

        syms: "OrderedDict[int, DLLSym] | None" = absolute

        for line in file.readlines():
            stripped = line.lstrip()
            if len(stripped) == 0 or stripped.isspace() or stripped.startswith("#"):
                continue

            pairs = SYMBOL_LINE_REGEX.findall(stripped)
            for pair in pairs:
                local: str = pair[0]
                name: str = pair[1]
                offset = int(pair[2], base=0)

                if name.startswith("."):
                    # Change section
                    if name in sections:
                        raise Exception(f"DLL syms.txt contains duplicate '{name}' section.")
                    syms = OrderedDict()
                    sections[name] = DLLSymSection(offset, syms)
                else:
                    # Add to current section
                    syms[offset] = DLLSym(name, local=local == "local")
        
        return DLLSymsTxt(absolute, sections)

