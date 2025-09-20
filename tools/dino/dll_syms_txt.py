from collections import OrderedDict
from typing import TextIO
import re

SYMBOL_LINE_REGEX = re.compile(r"([\.\w]+)\s*=\s*(\w+)([;:])")

class DLLSym:
    def __init__(self, name: str, attributes: dict[str, str]) -> None:
        self.name = name
        self.attributes = attributes

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
        abs: "OrderedDict[int, str]" = OrderedDict()
        for (sym_addr, sym) in self.absolute.items():
            abs[sym_addr] = sym.name
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
            if len(stripped) == 0 or stripped.isspace() or stripped.startswith("#") or stripped.startswith("//"):
                continue

            info, *extra = stripped.split("//")

            match = SYMBOL_LINE_REGEX.match(info)
            if match != None:
                name: str = match.group(1)
                offset = int(match.group(2), base=0)
                type: str = match.group(3)

                if type == ":":
                    # Change section
                    if name in sections:
                        raise Exception(f"DLL syms.txt contains duplicate '{name}' section.")
                    syms = OrderedDict()
                    sections[name] = DLLSymSection(offset, syms)
                else:
                    # Add to current section
                    attributes: dict[str, str] = {}
                    if len(extra) > 0:
                        pairs = extra[0].strip().split()
                        for pair in pairs:
                            kv = pair.split(":")
                            if len(kv) == 2:
                                attributes[kv[0]] = kv[1]
                    syms[offset] = DLLSym(name, attributes)
        
        return DLLSymsTxt(absolute, sections)

