#!/usr/bin/env python3
from __future__ import annotations
import os
from pathlib import Path
from typing import TextIO
import re

from dino.dlls_txt import DLLsTxt
from dino.dll_syms_txt import DLLSymsTxt

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
PROJECT_DIR = SCRIPT_DIR.parent

SRC_DIR = PROJECT_DIR.joinpath("src")

FUNC_START_REGEX = re.compile(r"^[\s/*]*((?:static)?\s*(\w+)[ *\n]+?(\w+)\s*\([\w *,\[\]]*\)\s*{)", re.MULTILINE)
NON_WORD_REGEX = re.compile(r"[^\w]+")
SYMBOL_NAME_REGEX = re.compile(r"^[a-zA-Z_]\w*")

def find_end_brace(contents: str, offset: int):
    depth = 1
    while depth > 0 and offset < len(contents):
        c = contents[offset]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
        offset += 1

    return offset

def get_funcs_from_c(file: TextIO, funcs: list[str], tokens: set[str]):
    contents = file.read()

    for token in NON_WORD_REGEX.split(contents):
        if SYMBOL_NAME_REGEX.match(token) != None:
            tokens.add(token)

    offset = 0
    while offset < len(contents):
        func_start = FUNC_START_REGEX.match(contents, pos=offset)
        if func_start == None:
            offset += 1
            continue

        func_name = func_start.group(3)

        end = find_end_brace(contents, func_start.end(1))
        offset = end

        funcs.append(func_name)

def main():
    with open(SRC_DIR.joinpath("dlls/dlls.txt"), "r", encoding="utf-8") as dlls_txt_file:
        dlls_txt = DLLsTxt.parse(dlls_txt_file)

    for number, dll_dir in dlls_txt.path_map.items():
        dll_src_dir = SRC_DIR.joinpath("dlls").joinpath(dll_dir)
        
        with open(dll_src_dir.joinpath("syms.txt"), "r", encoding="utf-8") as syms_txt_file:
            syms = DLLSymsTxt.parse(syms_txt_file)
        
        c_funcs: list[str] = []
        tokens: set[str] = set()
        for path in dll_src_dir.glob("*.c"):
            with open(path, "r", encoding="utf-8") as file:
                get_funcs_from_c(file, c_funcs, tokens)
        
        text_syms: set[str] = set()
        for sym in syms.sections[".text"].syms.values():
            text_syms.add(sym.name)

        for func in c_funcs:
            if func not in text_syms:
                print(f"DLL {number} .text symbol {func} not found in syms.txt.")
        
        # for section_name, section in syms.sections.items():
        #     if section_name == ".rodata":
        #         continue
        #     for sym in section.syms.values():
        #         if sym.name.startswith("_oob_"):
        #             continue
        #         if sym.name not in tokens:
        #             print(f"DLL {number} {section_name} syms.txt symbol {sym.name} not found in source code.")

if __name__ == "__main__":
    main()
