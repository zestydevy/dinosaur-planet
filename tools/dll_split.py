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
import yaml

from dino.dll import DLL, DLLHeader, DLLRelocation, DLLRelocationTable, DLLFunction, parse_dll_functions
from dino.dll_tab import DLLTab
from dino.dll_imports_tab import DLLImportsTab

ASM_PATH = Path("asm")
BIN_PATH = Path("bin")
SRC_PATH = Path("src")

global_asm_pattern = re.compile(r"#pragma GLOBAL_ASM\(\"asm\/nonmatchings\/dlls\/[0-9]+\/(.+)\.s\"\)")
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

        # Load DLLSIMPORT.tab
        if self.verbose:
            print("Loading DLLSIMPORT.tab...")
        
        import_tab_path = BIN_PATH.joinpath("assets/DLLSIMPORTTAB.bin")
        with open(import_tab_path, "rb") as import_tab_file:
            import_tab = DLLImportsTab.parse(import_tab_file.read())

        # Load core syms
        if self.verbose:
            print("Loading core syms...")
        
        core_syms = self.__get_core_symbols()
        
        # Extract each DLL that has a src directory
        dll_dirs = [Path(dir) for dir in glob.glob(f"{SRC_PATH}/dlls/*") if isdir(dir)]
        for dir in dll_dirs:
            number = dir.name

            # Skip DLL if not in list
            if len(only_dlls) > 0 and not number in only_dlls:
                continue

            # Load DLL config
            dll_config_path = dir.joinpath(f"{number}.yaml")
            if not dll_config_path.exists():
                print(f"WARN: Missing {dll_config_path}!")
                continue
            
            with open(dll_config_path, "r") as file:
                dll_config = yaml.safe_load(file)

            link_original_rodata = "link_original_rodata" in dll_config and dll_config["link_original_rodata"] or False
            link_original_data = "link_original_data" in dll_config and dll_config["link_original_data"] or False
            link_original_bss = "link_original_bss" in dll_config and dll_config["link_original_bss"] or False

            # Load known symbols for DLL
            syms_txt_path = SRC_PATH.joinpath(f"dlls/{number}/syms.txt")
            known_symbols = self.__get_existing_symbols(syms_txt_path)

            # Load DLL
            dll_path = BIN_PATH.joinpath(f"assets/dlls/{number}.dll")
            if not dll_path.exists():
                print(f"WARN: No such DLL {dll_path}!")
                continue

            with open(dll_path, "rb") as dll_file:
                if self.verbose:
                    print("[{}] Parsing...".format(number))
                start = timer()
                
                # Parse DLL header
                data = bytearray(dll_file.read())
                dll = DLL.parse(data, number)

                # Load extern GOT symbols via DLLIMPORTS.tab
                for got_entry in dll.reloc_table.global_offset_table:
                    if (got_entry & 0x80000000) != 0:
                        index = (got_entry & 0x7FFFFFFF) - 1
                        if index >= 0 and index < len(import_tab.imports):
                            real_addr = import_tab.imports[index]
                            if real_addr in core_syms:
                                known_symbols[got_entry] = core_syms[real_addr]
                
                # Parse functions
                dll_functions = parse_dll_functions(data, dll, known_symbols=known_symbols)

                # Get DLL .bss size
                bss_size = tab.entries[int(number) - 1].bss_size
                
                end = timer()
                if self.verbose:
                    print("[{}] Parsing complete (took {:.3} seconds).".format(number, end - start))
            
                # Extract DLL
                if self.verbose:
                    print("[{}] Extracting...".format(number))
                start = timer()

                self.extract_dll(dll, dll_functions, data, 
                    bss_size=bss_size,
                    skip_rodata=not link_original_rodata,
                    skip_data=not link_original_data,
                    skip_bss=not link_original_bss,
                )

                end = timer()
                if self.verbose:
                    print("[{}] Extracting complete (took {:.3} seconds).".format(number, end - start))

    def extract_dll(self, 
                    dll: DLL, 
                    dll_functions: "list[DLLFunction]",
                    data: bytearray, 
                    bss_size: int,
                    skip_data: bool,
                    skip_rodata: bool,
                    skip_bss: bool):
        # Determine paths
        src_path = SRC_PATH.joinpath(f"dlls/{dll.number}")
        asm_path = ASM_PATH.joinpath(f"nonmatchings/dlls/{dll.number}")
        asm_data_path = ASM_PATH.joinpath(f"nonmatchings/dlls/{dll.number}/data")

        # Determine what needs to be extracted
        c_file_path = src_path.joinpath(f"{dll.number}.c")
        emit_funcs = self.__get_functions_to_extract(c_file_path, dll.number)

        rodata_size = dll.get_rodata_size()
        data_size = dll.get_data_size()

        emit_rodata = not skip_rodata and dll.has_rodata() and rodata_size > 0
        emit_data = not skip_data and dll.has_data() and data_size > 0
        emit_bss = not skip_bss and bss_size > 0

        # Create directories if necessary
        if emit_funcs is None or len(emit_funcs) > 0:
            os.makedirs(asm_path, exist_ok=True)
        if emit_rodata or emit_data or emit_bss:
            os.makedirs(asm_data_path, exist_ok=True)

        # Extract .text
        if emit_funcs is None or len(emit_funcs) > 0:
            self.__extract_text_asm(asm_path, dll, dll_functions, emit_funcs)

        # Extract .rodata
        if emit_rodata:
            rodata_start = dll.header.rodata_offset + dll.reloc_table.get_size() # exclude relocation tables
            rodata_end = rodata_start + rodata_size
            self.__extract_rodata_asm(asm_data_path, dll, data[rodata_start:rodata_end])
        
        # Extract .data
        if emit_data:
            data_start = dll.header.data_offset
            data_end = data_start + data_size
            self.__extract_data_asm(asm_data_path, dll, data[data_start:data_end])
        
        # Extract .bss
        if emit_bss:
            self.__extract_bss_asm(asm_data_path, dll, bss_size)

        # Create exports.s if it doesn't exist
        exports_s_path = src_path.joinpath("exports.s")
        if not exports_s_path.exists():
            self.__create_exports_s(exports_s_path, dll, dll_functions)

        # Create syms.txt if it doens't exist
        syms_txt_path = src_path.joinpath("syms.txt")
        if not syms_txt_path.exists():
            self.__create_syms_txt(syms_txt_path, dll, dll_functions)

        # Create <dll>.c stub if it doesn't exist
        if not c_file_path.exists():
            self.__create_c_stub(c_file_path, asm_path, dll_functions)

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

    def __create_c_stub(self, c_path: Path, asm_path: Path, functions: "list[DLLFunction]"):
        with open(c_path, "w", encoding="utf-8") as c_file:
            c_file.write("#include <PR/ultratypes.h>\n")
            
            for func in functions:
                c_file.write("\n")
                c_file.write(f'#pragma GLOBAL_ASM("{asm_path.as_posix()}/{func.symbol}.s")\n')

    def __create_syms_txt(self, 
                          syms_path: Path, 
                          dll: DLL, 
                          dll_functions: "list[DLLFunction]"):
        with open(syms_path, "w", encoding="utf-8") as syms_file:
            addrs_found: "set[int]" = set()
            func_addrs: "set[int]" = set()
            got_syms_found = 0
            # Write function symbols
            for func in dll_functions:
                syms_file.write("{} = 0x{:X};\n".format(func.symbol, func.address))
                func_addrs.add(func.address)

            # Group external and local symbols that were found
            extern_symbols: "list[tuple[str, int]]" = []
            local_symbols: "list[tuple[str, int]]" = []
            for func in dll_functions:
                for name, value in func.auto_symbols.items():
                    if value in addrs_found:
                        continue
                    got_syms_found += 1
                    addrs_found.add(value)
                    if value in func_addrs:
                        # Already have a symbol entry for local functions
                        continue
                    if (value & 0x80000000) != 0:
                        extern_symbols.append((name, value))
                    else:
                        local_symbols.append((name, value))
            
            extern_symbols.sort(key=lambda p: p[1])
            local_symbols.sort(key=lambda p: p[1])

            # Write external symbols
            if len(extern_symbols) > 0:
                syms_file.write("\n")
                for (name, value) in extern_symbols:
                    syms_file.write("{} = 0x{:X};\n".format(name, value))
            
            # Write local symbols
            if len(local_symbols) > 0:
                syms_file.write("\n")
                for (name, value) in local_symbols:
                    syms_file.write("{} = 0x{:X};\n".format(name, value))
            
            assert got_syms_found == max(0, len(dll.reloc_table.global_offset_table) - 4)

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
                # Write relocations
                for reloc in func.relocations:
                    s_file.write(".reloc {}+0x{:X}, {}, {}-0x{:X}\n"
                        .format(func.symbol, reloc.offset - func.address, reloc.type, reloc.expression, reloc.got_index * 4))
                if len(func.relocations) > 0:
                    s_file.write("\n")

                # Write instructions
                s_file.write(f"glabel {func.symbol}\n")

                for i in func.insts:
                    if i.label is not None:
                        s_file.write(f"{i.label}:\n")

                    rom_addr = i.address + dll.header.size
                    ram_addr = i.address
                    inst_bytes = i.original.bytes.hex().upper()
                    mnemonic = (' ' + i.mnemonic) if i.is_branch_delay_slot else i.mnemonic
                    # Note: Use original operand string if the instruction has a relocation since we're
                    # specifying relocations with separate directives (need to emit the original $gp addend
                    # rather than something like %got to avoid duplicate relocation entries)
                    op_str = i.original.op_str if i.has_relocation else i.op_str
                    ref = (f' /* ref: {i.ref} */') if i.ref is not None else ''
                    
                    s_file.write("/* {:0>4X} {:0>6X} {} */ {:<11}{}{}\n"
                        .format(rom_addr, ram_addr, inst_bytes, mnemonic, op_str, ref))

    def __extract_rodata_asm(self, dir: Path, dll: DLL, data: bytearray):
        rodata_path = dir.joinpath(f"{dll.number}.rodata.s")
        with open(rodata_path, "w", encoding="utf-8") as rodata_file:
            # Set .rodata section
            rodata_file.write(".section .rodata, \"a\"\n")
            # Write data
            rodata_rom_offset = dll.header.rodata_offset + dll.reloc_table.get_size()
            rodata_ram_offset = dll.header.rodata_offset - dll.header.size
            for i in range(0, len(data), 4):
                word = struct.unpack_from(">I", data, offset=i)[0]

                rom_addr = rodata_rom_offset + i
                ram_addr = rodata_ram_offset + i
                
                rodata_file.write("/* {:0>4X} {:0>6X} */ .4byte 0x{:X}\n"
                    .format(rom_addr, ram_addr, word))

    def __extract_data_asm(self, dir: Path, dll: DLL, data: bytearray):
        data_path = dir.joinpath(f"{dll.number}.data.s")
        with open(data_path, "w", encoding="utf-8") as data_file:
            # Set .data section
            data_file.write(".data\n")
            # Write relocations
            for offset in dll.reloc_table.data_relocations:
                data_file.write(".reloc 0x{:X}, \"R_MIPS_32\", .data\n".format(offset))
            # Write data
            data_rom_offset = dll.header.data_offset
            data_ram_offset = dll.header.data_offset - dll.header.size - dll.reloc_table.get_size()
            for i in range(0, len(data), 4):
                word = struct.unpack_from(">I", data, offset=i)[0]

                rom_addr = data_rom_offset + i
                ram_addr = data_ram_offset + i
                
                data_file.write("/* {:0>4X} {:0>6X} */ .4byte 0x{:X}\n"
                    .format(rom_addr, ram_addr, word))

    def __extract_bss_asm(self, dir: Path, dll: DLL, bss_size: int):
        assert bss_size > 0

        bss_path = dir.joinpath(f"{dll.number}.bss.s")
        with open(bss_path, "w", encoding="utf-8") as bss_file:
            # Set .bss section
            bss_file.write(".bss\n")
            # Write .bss size
            bss_file.write(".ds.s 0x{:X}, 0\n".format(bss_size))

    def __get_functions_to_extract(self, path: Path, dll_number: str) -> "list[str] | None":
        """Returns None if all functions should be extracted (i.e. there is no .c file to derive the list from)"""
        if not path.exists():
            return None
        
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

    def __get_core_symbols(self) -> "dict[int, str]":
        map: "dict[int, str]" = {}

        def read_syms(file: TextIO):
            for line in file.readlines():
                pairs = symbol_pattern.findall(line.strip())
                for pair in pairs:
                    addr_str: str = pair[1]
                    if addr_str.lower().startswith("0x"):
                        addr = int(addr_str, base=16)
                    else:
                        addr = int(addr_str)
                    map[addr] = pair[0]

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

        return map

def main():
    parser = argparse.ArgumentParser(description="Extract assembly and data from Dinosaur Planet DLLs and stub out an environment for recompiling each.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project (default=..).", default="..")
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
