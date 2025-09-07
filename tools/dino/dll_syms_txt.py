from collections import OrderedDict
from typing import TextIO
import re

SYMBOL_LINE_REGEX = re.compile(r"([\.\w]+)\s*=\s*(\w+)([;:])")

class DLLSymSection:
    def __init__(self, offset: int, syms: OrderedDict[int, str]):
        self.offset = offset
        self.syms = syms

class DLLSymsTxt:
    """
    Parser for DLL syms.txt files
    """
    
    def __init__(self, 
                 absolute: "OrderedDict[int, str]", 
                 sections: "OrderedDict[str, DLLSymSection]"):
        self.absolute = absolute 
        self.sections = sections 

    def to_absolute(self) -> "OrderedDict[int, str]":
        abs: "OrderedDict[int, str]" = OrderedDict()
        for (sym_addr, sym) in self.absolute.items():
            abs[sym_addr] = sym 
        for section in self.sections.values():
            for (sym_addr, sym) in section.syms.items():
                abs[section.offset + sym_addr] = sym
        
        return abs

    @staticmethod
    def parse(file: TextIO):
        absolute: "OrderedDict[int, str]" = OrderedDict()
        sections: "OrderedDict[str, DLLSymSection]" = OrderedDict()

        syms: "OrderedDict[int, str] | None" = absolute

        for line in file.readlines():
            stripped = line.lstrip()
            if len(stripped) == 0 or stripped.isspace() or stripped.startswith("#"):
                continue

            pairs = SYMBOL_LINE_REGEX.findall(stripped)
            for pair in pairs:
                name: str = pair[0]
                offset = int(pair[1], base=0)
                type: str = pair[2]

                if type == ":":
                    # Change section
                    if name in sections:
                        raise Exception(f"DLL syms.txt contains duplicate '{name}' section.")
                    syms = OrderedDict()
                    sections[name] = DLLSymSection(offset, syms)
                else:
                    # Add to current section
                    syms[offset] = name
        
        return DLLSymsTxt(absolute, sections)

