#!/usr/bin/env python3
# A mini version of splat specifically for Dinosaur Planet DLLs
# dll_split.py is responsible for:
# - Extracting assembly and data from Dinosaur Planet DLLs
# - Extracting in such a way that is ready for the decomp project's build system
# - Creating .c, exports.s, and syms.txt stubs

import argparse
from genericpath import isdir
import glob
import os
from pathlib import Path
import re
import struct
from timeit import default_timer as timer
from typing import TextIO

from dino.dll import DLL
from dino.dll_analysis import DLLFunction, get_all_dll_functions
from dino.dll_build_config import DLLBuildConfig
from dino.dll_code_printer import stringify_instruction
from dino.dll_tab import DLLTab
from dino.dlls_txt import DLLsTxt

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
ASM_PATH = Path("asm")
BIN_PATH = Path("bin")
SRC_PATH = Path("src")

global_asm_pattern = re.compile(r"#pragma GLOBAL_ASM\(\"asm\/nonmatchings\/dlls\/\S+\/(.+)\.s\"\)")
symbol_pattern = re.compile(r"(\S+)\s*=\s*(\S+);")

class DLLSplitter:
    def __init__(self, verbose: bool) -> None:
        self.verbose = verbose

    def extract_dlls(self, only_dlls: "list[str]"):
        # Load DLLS.tab
        if self.verbose:
            print("Loading DLLS.tab...")
        
        tab_path = BIN_PATH.joinpath("assets/DLLS_tab.bin")
        with open(tab_path, "rb") as tab_file:
            tab = DLLTab.parse(tab_file.read())

        # Load core export syms
        if self.verbose:
            print("Loading core export syms...")
        
        core_export_syms = self.__get_core_export_symbols()
        
        # Extract each DLL that has a src directory
        dlls_src_path = SRC_PATH.joinpath("dlls")
        dlls_txt_path = dlls_src_path.joinpath("dlls.txt")
        assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
        
        with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
            dlls_txt = DLLsTxt.parse(dlls_txt_file)

        for (number, dll_dir) in dlls_txt.path_map.items():
            dir = dlls_src_path.joinpath(dll_dir)

            if not dir.exists():
                print(f"WARN: No such DLL src directory {dir}!")
                continue

            # Skip DLL if not in list
            if len(only_dlls) > 0 and not str(number) in only_dlls:
                continue

            # Check DLL path
            dll_path = BIN_PATH.joinpath(f"assets/dlls/{number}.dll")
            if not dll_path.exists():
                print(f"WARN: No such DLL {dll_path}!")
                continue

            # Load known symbols for DLL
            known_symbols = core_export_syms.copy()
            syms_txt_path = dir.joinpath("syms.txt")
            known_symbols.update(self.__get_existing_symbols(syms_txt_path))

            # Load DLL
            with open(dll_path, "rb") as dll_file:
                if self.verbose:
                    print("[{}] Parsing...".format(number))
                start = timer()
                
                # Parse DLL header
                data = bytearray(dll_file.read())
                dll = DLL.parse(data, str(number))
                
                # Parse functions
                dll_functions = get_all_dll_functions(data, dll, known_symbols=known_symbols, analyze=True)

                # Get DLL .bss size
                bss_size = tab.entries[number - 1].bss_size
                
                end = timer()
                if self.verbose:
                    print("[{}] Parsing complete (took {:.3} seconds).".format(number, end - start))
            
                # Load or create DLL config
                dll_config_path = dir.joinpath("dll.yaml")
                if dll_config_path.exists():
                    with open(dll_config_path, "r") as file:
                        dll_config = DLLBuildConfig.parse(file)
                else:
                    dll_config = DLLBuildConfig(compile=True)
                    with open(dll_config_path, "w") as file:
                        dll_config.save(file)

                # Extract DLL
                if self.verbose:
                    print("[{}] Extracting...".format(number))
                start = timer()

                self.extract_dll(dll, dll_functions, data, bss_size=bss_size, dll_dir=dll_dir)

                end = timer()
                if self.verbose:
                    print("[{}] Extracting complete (took {:.3} seconds).".format(number, end - start))

    def extract_dll(self, 
                    dll: DLL, 
                    dll_functions: "list[DLLFunction]",
                    data: bytearray, 
                    bss_size: int,
                    dll_dir: str):
        # Determine paths
        src_path = SRC_PATH.joinpath(f"dlls/{dll_dir}")
        asm_path = ASM_PATH.joinpath(f"nonmatchings/dlls/{dll_dir}")

        # Determine what needs to be extracted
        has_c_files = False
        emit_funcs: "list[str] | None" = None
        for c_file_path in [Path(path) for path in glob.glob(f"{src_path}/*.c", recursive=True)]:
            if not has_c_files:
                has_c_files = True
                emit_funcs = []
            emit_funcs.extend(self.__get_functions_to_extract(c_file_path))

        # Create directories if necessary
        if not has_c_files or len(emit_funcs) > 0:
            os.makedirs(asm_path, exist_ok=True)

        # Extract .text
        if not has_c_files or len(emit_funcs) > 0:
            self.__extract_text_asm(asm_path, dll, dll_functions, emit_funcs)

        # Create exports.s if it doesn't exist
        exports_s_path = src_path.joinpath("exports.s")
        if not exports_s_path.exists():
            self.__create_exports_s(exports_s_path, dll, dll_functions)

        # Create syms.txt if it doens't exist
        syms_txt_path = src_path.joinpath("syms.txt")
        if not syms_txt_path.exists():
            self.__create_syms_txt(syms_txt_path, dll, dll_functions, bss_size)

        # Create <dll>.c stub if no .c files exist
        if not has_c_files:
            c_file_path = src_path.joinpath(f"{dll.number}.c")
            self.__create_c_stub(c_file_path, asm_path, dll, dll_functions, data, bss_size)

    def __create_exports_s(self, path: Path, dll: DLL, dll_functions: "list[DLLFunction]"):
        funcs_by_address: "dict[int, str]" = {}
        for func in dll_functions:
            funcs_by_address[func.address] = func.symbol

        with open(path, "w", encoding="utf-8") as exports_s:
            exports_s.write(".option pic2\n")
            exports_s.write(".section \".exports\"\n")
            exports_s.write(".global _exports\n")
            exports_s.write("_exports:\n")
            exports_s.write("\n")

            exports_s.write(f"# ctor/dtor\n")
            exports_s.write(f".dword {funcs_by_address[dll.header.ctor_offset]}\n")
            exports_s.write(f".dword {funcs_by_address[dll.header.dtor_offset]}\n")
            exports_s.write("\n")

            exports_s.write(f"# export table\n")
            for offset in dll.header.export_offsets:
                func_symbol = funcs_by_address[offset]
                exports_s.write(f".dword {func_symbol}\n")

    def __create_c_stub(self, c_path: Path, asm_path: Path, dll: DLL, functions: "list[DLLFunction]", dll_data: bytearray, bss_size: int):
        # Collect local refs
        rodata_refs: "set[int] | list[int]" = set()
        data_refs: "set[int] | list[int]" = set()
        bss_refs: "set[int] | list[int]" = set()
        jtbl_refs: "set[int] | list[int]" = set()

        extern_offsets = dll.reloc_table.global_offset_table[4:]

        for func in functions:
            if func.local_rodata_refs != None:
                for ref in func.local_rodata_refs:
                    rodata_refs.add(ref)
            if func.local_data_refs != None:
                for ref in func.local_data_refs:
                    data_refs.add(ref)
            if func.local_bss_refs != None:
                for ref in func.local_bss_refs:
                    bss_refs.add(ref)
            if func.jump_table_refs != None:
                for ref in func.jump_table_refs:
                    rodata_refs.add(ref)
                    jtbl_refs.add(ref)
        
        # Write
        with open(c_path, "w", encoding="utf-8") as c_file:
            # Includes
            c_file.write("#include \"PR/ultratypes.h\"\n")

            # .rodata stubs
            if dll.has_rodata():
                rodata_start = dll.header.rodata_offset + dll.reloc_table.get_size() # exclude relocation tables
                rodata_end = rodata_start + dll.get_rodata_size()
                rodata = dll_data[rodata_start:rodata_end]

                rodata_refs.add(0)
                rodata_refs.add(len(rodata))
                rodata_refs = list(rodata_refs)
                rodata_refs.sort()

                c_file.write("\n")
                for i in range(len(rodata_refs) - 1):
                    offset = rodata_refs[i]
                    size = rodata_refs[i + 1] - offset
                    self.__write_c_stub_data(
                        c_file, 
                        rodata[offset:offset+size], 
                        "_rodata{}_".format("" if not offset in jtbl_refs else "_jtbl"), 
                        const=True,
                        static=not (rodata_start + offset) in extern_offsets,
                        field_offset=offset)
            
            # .data stubs
            if dll.has_data():
                data_start = dll.header.data_offset
                data_end = data_start + dll.get_data_size()
                data = dll_data[data_start:data_end]

                data_refs.add(0)
                data_refs.add(len(data))

                # Break up data refs by relocated pointer destinations
                # These aren't necessarily the starts of individual variables but we need them to be
                # individual to generate the initial stubs correctly
                for data_reloc in dll.reloc_table.data_relocations:
                    word = struct.unpack_from(">I", data, offset=data_reloc)[0]
                    data_refs.add(word)

                data_refs = list(data_refs)
                data_refs.sort()

                c_file.write("\n")
                for i in range(len(data_refs) - 1):
                    offset = data_refs[i]
                    size = data_refs[i + 1] - offset
                    self.__write_c_stub_data(
                        c_file, 
                        data[offset:offset+size], 
                        "_data_", 
                        const=False,
                        static=not (data_start + offset) in extern_offsets,
                        field_offset=offset,
                        relocs=dll.reloc_table.data_relocations)
            
            # .bss stubs
            if bss_size > 0:
                bss_refs.add(0)
                bss_refs.add(bss_size)
                bss_refs = list(bss_refs)
                bss_refs.sort()

                bss_start = dll.get_bss_offset()

                c_file.write("\n")
                for i in range(len(bss_refs) - 1):
                    offset = bss_refs[i]
                    size = bss_refs[i + 1] - offset
                    static = not (bss_start + offset) in extern_offsets
                    c_file.write("{}u8 _bss_{:X}[{:#x}];\n"
                        .format("static " if static else "", offset, size))
            
            # function stubs
            for func in functions:
                c_file.write("\n")
                c_file.write(f'#pragma GLOBAL_ASM("{asm_path.as_posix()}/{func.symbol}.s")\n')

    def __write_c_stub_data(self, c_file: TextIO, data: bytearray, prefix: str, const: bool, static: bool, field_offset: int, relocs: set[int] | None=None):
        data_len = len(data)

        # Note: Force writing u32s if this data contains a reloc. We need 4 bytes for the pointer
        has_reloc = False
        if relocs != None:
            for i in range(0, len(data), 4):
                if (field_offset + i) in relocs:
                    has_reloc = True
                    break

        data_type = "u8"
        if (data_len % 4) == 0 or has_reloc:
            data_type = "u32"
        elif (data_len % 2) == 0:
            data_type = "u16"

        name = "{}{:X}".format(prefix, field_offset)

        c_file.write("{}{}{} {}[] = {{\n".format(
            "static " if static else "", 
            "const " if const else "", 
            data_type, 
            name))

        if (data_len % 4) == 0:
            for i in range(0, len(data), 4):
                word = struct.unpack_from(">I", data, offset=i)[0]
                if i > 0:
                    c_file.write(", ")
                    if (i % (4 * 8)) == 0:
                        c_file.write("\n    ")
                else:
                    c_file.write("    ")
                if relocs != None and (field_offset + i) in relocs:
                    # Assumes reloc'd pointers always point to the same section
                    c_file.write("(u32)&{}{:X}".format(prefix, word))
                else:
                    c_file.write("{:0=#10x}".format(word))
        elif (data_len % 2) == 0:
            for i in range(0, len(data), 2):
                half = struct.unpack_from(">H", data, offset=i)[0]
                if i > 0:
                    c_file.write(", ")
                    if (i % (2 * 12)) == 0:
                        c_file.write("\n    ")
                else:
                    c_file.write("    ")
                c_file.write("{:0=#6x}".format(half))
        else:
            for i in range(0, len(data)):
                byte = data[i]
                if i > 0:
                    c_file.write(", ")
                    if (i % (20)) == 0:
                        c_file.write("\n    ")
                else:
                    c_file.write("    ")
                c_file.write("{:0=#4x}".format(byte))
        
        c_file.write("\n};\n")

        # We might have leftovers if the existence of relocs forced us into writing u32s
        if has_reloc and (data_len % 4) != 0:
            leftover = data_len % 4
            c_file.write("{}{}{} {}_extra[] = {{\n".format(
                "static " if static else "", 
                "const " if const else "", 
                data_type, 
                name))
            for i in range(leftover, len(data)):
                byte = data[i]
                if i > 0:
                    c_file.write(", ")
                    if (i % (20)) == 0:
                        c_file.write("\n    ")
                else:
                    c_file.write("    ")
                c_file.write("{:0=#4x}".format(byte))
            c_file.write("\n};\n")

    def __create_syms_txt(self, 
                          syms_path: Path, 
                          dll: DLL,
                          dll_functions: "list[DLLFunction]",
                          bss_size: int):
        text_size = dll.get_text_size()
        text_start = dll.header.size
        
        with open(syms_path, "w", encoding="utf-8") as syms_file:
            # Write function symbols
            syms_file.write("# functions\n")
            for func in dll_functions:
                syms_file.write("{} = 0x{:X};\n".format(func.symbol, func.address))
            
            # Write extern symbols
            externs: "list[int]" = []
            # Skip section entries
            for got_entry in dll.reloc_table.global_offset_table[4:]:
                # Skip imports (already covered by export_symbol_addrs.txt)
                # Skip functions, already covered by above
                if (got_entry & 0x80000000) == 0 and got_entry >= text_size:
                    externs.append(got_entry)
            if len(externs) > 0:
                syms_file.write("\n# external symbols\n")
                for got_entry in externs:
                    syms_file.write("D_{0:X} = 0x{0:X};\n".format(got_entry))
            
            # Write discovered .rodata, .data, and .bss variables (if any)
            rodata_refs: "set[int] | list[int]" = set()
            data_refs: "set[int] | list[int]" = set()
            bss_refs: "set[int] | list[int]" = set()

            for func in dll_functions:
                if func.local_rodata_refs != None:
                    for ref in func.local_rodata_refs:
                        rodata_refs.add(ref)
                if func.local_data_refs != None:
                    for ref in func.local_data_refs:
                        data_refs.add(ref)
                if func.local_bss_refs != None:
                    for ref in func.local_bss_refs:
                        bss_refs.add(ref)

            # Note: Variable syms need to be relative to start of .text
            if len(rodata_refs) > 0 or len(data_refs) > 0 or len(bss_refs) > 0:
                syms_file.write("\n# variables\n")

                if dll.has_rodata() and len(rodata_refs) > 0:
                    rodata_refs.add(0)
                    rodata_refs = list(rodata_refs)
                    rodata_refs.sort()

                    rodata_start = dll.header.rodata_offset + dll.reloc_table.get_size() # exclude relocation tables

                    for ref in rodata_refs:
                        syms_file.write("_rodata_{:X} = 0x{:X};\n".format(ref, rodata_start + ref - text_start))
                
                if dll.has_data():
                    data_refs.add(0)
                    data_refs = list(data_refs)
                    data_refs.sort()

                    data_start = dll.header.data_offset

                    for ref in data_refs:
                        syms_file.write("_data_{:X} = 0x{:X};\n".format(ref, data_start + ref - text_start))
                
                if bss_size > 0:
                    bss_refs.add(0)
                    bss_refs = list(bss_refs)
                    bss_refs.sort()
                    
                    bss_start = dll.get_bss_offset()

                    for ref in bss_refs:
                        syms_file.write("_bss_{:X} = 0x{:X};\n".format(ref, bss_start + ref - text_start))

    def __extract_text_asm(self, 
                           dir: Path, 
                           dll: DLL, 
                           dll_functions: "list[DLLFunction]",
                           funcs: "list[str] | None"):
        for func in dll_functions:
            if funcs is not None and not func.symbol in funcs:
                continue

            s_path = dir.joinpath(f"{func.symbol}.s")
            with open(s_path, "w", encoding="utf-8") as s_file:
                s_file.write(f"glabel {func.symbol}\n")

                for idx, i in enumerate(func.insts):
                    inst_str, label = stringify_instruction(idx, i, func)
                    if label != None:
                        s_file.write(f"{label}\n")
                    
                    rom_addr = i.i.address + dll.header.size
                    ram_addr = i.i.address
                    inst_bytes = i.i.bytes.hex().upper()

                    s_file.write("/* {:0>4X} {:0>6X} {} */ {}\n"
                        .format(rom_addr, ram_addr, inst_bytes, inst_str))

    def __get_functions_to_extract(self, path: Path) -> "list[str]":
        """Returns None if all functions should be extracted (i.e. there is no .c file to derive the list from)"""
        if not path.exists():
            return []
        
        emit_funcs: "list[str]" = []
        with open(path, "r", encoding="utf-8") as c_file:
            for line in c_file.readlines():
                symbols = global_asm_pattern.findall(line.strip())
                for symbol in symbols:
                    emit_funcs.append(symbol)
        
        return emit_funcs

    def __get_existing_symbols(self, path: Path) -> "dict[int, str]":
        if not path.exists():
            return {}
        
        symbols: "dict[int, str]" = {}

        with open(path, "r", encoding="utf-8") as syms_file:
            line_number = 1
            for line in syms_file.readlines():
                pairs = symbol_pattern.findall(line.strip())
                for pair in pairs:
                    addr_str: str = pair[1]
                    if addr_str.lower().startswith("0x"):
                        addr = int(addr_str, base=16)
                    else:
                        addr = int(addr_str)

                    if addr in symbols:
                        print(f"WARN: Duplicate symbol addr {hex(addr)} in {path} line {line_number}!")
                    
                    symbols[addr] = pair[0]
                line_number += 1

        return symbols

    def __get_core_export_symbols(self) -> "dict[int, str]":
        map: "dict[int, str]" = {}

        with open("export_symbol_addrs.txt", "r", encoding="utf-8") as syms_file:
            for line in syms_file.readlines():
                pairs = symbol_pattern.findall(line.strip())
                for pair in pairs:
                    addr_str: str = pair[1]
                    if addr_str.lower().startswith("0x"):
                        addr = int(addr_str, base=16)
                    else:
                        addr = int(addr_str)
                    map[addr] = pair[0]

        return map

def main():
    parser = argparse.ArgumentParser(description="Extract assembly and data from Dinosaur Planet DLLs and stub out an environment for recompiling each.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable debug logging.", default=False)
    parser.add_argument("-q", "--quiet", action="store_true", help="Don't display informational messages.", default=False)
    parser.add_argument("dlls", nargs="*", action="extend", help="The numbers of each DLL to extract. Don't specify any to extract all that have src directories.")

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    # Extract DLLs
    start = timer()
    
    splitter = DLLSplitter(verbose=args.verbose)
    splitter.extract_dlls(args.dlls)
    
    end = timer()
    if not args.quiet:
        print("DLL extraction complete (took {:.3} seconds).".format(end - start))

if __name__ == "__main__":
    main()
