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
import rabbitizer
import re
import struct
from timeit import default_timer as timer
from typing import TextIO

from dino.dll import DLL
from dino.dll_analysis import DLLFunction, DLLLocalRefInfo, DLLLocalRefLoadType, get_all_dll_functions
from dino.dll_build_config import DLLBuildConfig
from dino.dll_code_printer import stringify_instruction
from dino.dll_tab import DLLTab
from dino.dlls_txt import DLLsTxt
from dino.dll_symbols import DLLSymbols
from dino.dll_syms_txt import DLLSymsTxt

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
ASM_PATH = Path("asm")
BIN_PATH = Path("bin")
SRC_PATH = Path("src")

global_asm_pattern = re.compile(r"#pragma GLOBAL_ASM\(\"asm\/nonmatchings\/dlls\/\S+\/(.+)\.s\"\)")
symbol_pattern = re.compile(r"(\S+)\s*=\s*(\S+);")

class DLLFunctionWithLateRodata:
    def __init__(self, 
                 func: DLLFunction, 
                 late_rodata_start: int,
                 late_rodata_end: int) -> None:
        self.func = func
        self.late_rodata_start = late_rodata_start
        self.late_rodata_end = late_rodata_end

class DLLSplitter:
    def __init__(self, verbose: bool, disassemble_all: bool) -> None:
        self.verbose = verbose
        self.disassemble_all = disassemble_all

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

                # Get DLL .bss size
                bss_size = tab.entries[number - 1].bss_size
                
                # Parse DLL header
                data = bytearray(dll_file.read())
                dll = DLL.parse(data)
                
                # Parse functions
                dll_symbols = DLLSymbols(dll, number, known_symbols)
                dll_functions = get_all_dll_functions(data, dll, dll_symbols, analyze=True, bss_size=bss_size)
                
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

                self.extract_dll(dll, number, dll_functions, dll_symbols, data, bss_size=bss_size, dll_dir=dll_dir)

                end = timer()
                if self.verbose:
                    print("[{}] Extracting complete (took {:.3} seconds).".format(number, end - start))

    def extract_dll(self, 
                    dll: DLL, 
                    dll_number: int,
                    dll_functions: "list[DLLFunction]",
                    dll_symbols: DLLSymbols,
                    data: bytearray, 
                    bss_size: int,
                    dll_dir: str):
        # Determine paths
        src_path = SRC_PATH.joinpath(f"dlls/{dll_dir}")
        nonmatching_asm_path = ASM_PATH.joinpath(f"nonmatchings/dlls/{dll_dir}")
        matching_asm_path = ASM_PATH.joinpath(f"matchings/dlls/{dll_dir}")

        # Determine what needs to be extracted
        has_c_files = False
        nonmatching_funcs: "list[str] | None" = None
        for c_file_path in [Path(path) for path in glob.glob(f"{src_path}/*.c", recursive=True)]:
            if not has_c_files:
                has_c_files = True
                nonmatching_funcs = []
            nonmatching_funcs.extend(self.__get_nonmatching_functions_from_c(c_file_path))

        # Create directories if necessary
        if not has_c_files or len(nonmatching_funcs) > 0:
            os.makedirs(nonmatching_asm_path, exist_ok=True)
        if self.disassemble_all and (not has_c_files or len(dll_functions) != len(nonmatching_funcs)):
            os.makedirs(matching_asm_path, exist_ok=True)

        # Migrate .late_rodata
        (funcs_with_late_rodata, late_rodata_start) = self.__migrate_late_rodata(dll_functions, dll.get_rodata_size())

        # Extract .text
        if not has_c_files or len(nonmatching_funcs) > 0 or self.disassemble_all:
            self.__extract_text_asm(nonmatching_asm_path, matching_asm_path, dll, funcs_with_late_rodata, dll_symbols, data, nonmatching_funcs)

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
            c_file_path = src_path.joinpath(f"{dll_number}.c")
            self.__create_c_stub(c_file_path, nonmatching_asm_path, dll, dll_functions, data, bss_size, late_rodata_start)

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

    def __create_c_stub(self, 
                        c_path: Path, 
                        asm_path: Path, 
                        dll: DLL, 
                        functions: "list[DLLFunction]", 
                        dll_data: bytearray, 
                        bss_size: int,
                        late_rodata_start: int):
        # Map exports
        exports: "dict[int, int]" = {}
        for i, offset in enumerate(dll.header.export_offsets):
            exports[offset] = i
        
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
                    if offset >= late_rodata_start:
                        break
                    self.__write_c_stub_data(
                        c_file, 
                        rodata[offset:offset+size], 
                        "_rodata{}_".format("" if not offset in jtbl_refs else "_jtbl"), 
                        const=True,
                        static=not (rodata_start + offset - dll.header.size) in extern_offsets,
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
                        static=not (data_start + offset - dll.header.size) in extern_offsets,
                        field_offset=offset,
                        relocs=dll.reloc_table.data_relocations)
            
            # .bss stubs
            if bss_size > 0:
                bss_refs.add(0)
                bss_refs.add(bss_size)
                bss_refs = list(bss_refs)
                bss_refs.sort()
                self.__adjust_refs_for_c_stub(bss_refs)

                bss_start = dll.get_bss_offset()

                c_file.write("\n")
                for i in range(len(bss_refs) - 1):
                    offset = bss_refs[i]
                    size = bss_refs[i + 1] - offset
                    static = not (bss_start + offset - dll.header.size) in extern_offsets
                    c_file.write("/*0x{:X}*/ {}u8 _bss_{:X}[{:#x}];\n"
                        .format(offset, "static " if static else "", offset, size))
            
            # function stubs
            func_num = 0
            for func in functions:
                metadata = ["offset: 0x{:X}".format(func.address)]
                
                if func.address == dll.header.ctor_offset:
                    metadata.append("ctor")
                elif func.address == dll.header.dtor_offset:
                    metadata.append("dtor")
                else:
                    metadata.append(f"func: {func_num}")
                    func_num += 1

                export_idx = exports.get(func.address, None)
                if export_idx != None:
                    metadata.append(f"export: {export_idx}")

                c_file.write("\n")
                c_file.write(f"// {" | ".join(metadata)}\n")
                c_file.write(f'#pragma GLOBAL_ASM("{asm_path.as_posix()}/{func.symbol}.s")\n')

    def __adjust_refs_for_c_stub(self, refs: list[int]):
        i = 0
        while i < len(refs) - 1:
            offset = refs[i]
            size = refs[i + 1] - offset

            offset_alignment = self.__determine_alignment(offset)
            if offset_alignment < 16:
                # If the size we're trying to stub is bigger than the ref's alignment, IDO will
                # add padding before the stubbed variable and mess everything up. So we need
                # to split up these into additional "fake refs".
                max_size = offset_alignment
                if max_size < size:
                    refs.insert(i + 1, offset + max_size)

            i += 1

    def __determine_alignment(self, offset: int) -> int:
        if (offset % 16) == 0:
            return 16
        elif (offset % 8) == 0:
            return 8
        elif (offset % 4) == 0:
            return 4
        elif (offset % 2) == 0:
            return 2
        else:
            return 1

    def __write_c_stub_data(self, 
                            c_file: TextIO, 
                            data: bytearray, 
                            prefix: str, 
                            const: bool, 
                            static: bool, 
                            field_offset: int, 
                            relocs: set[int] | None=None):
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

        c_file.write("/*0x{:X}*/ {}{}{} {}[] = {{\n".format(
            field_offset,
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
                "u8", 
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
            # Write extern symbols
            externs: "list[int]" = []
            # Skip section entries
            for got_entry in dll.reloc_table.global_offset_table[4:]:
                # Skip imports (already covered by export_symbol_addrs.txt)
                # Skip functions, already covered by above
                if (got_entry & 0x80000000) == 0 and got_entry >= text_size:
                    externs.append(got_entry)
            if len(externs) > 0:
                for got_entry in externs:
                    syms_file.write("D_{0:X} = 0x{0:X};\n".format(got_entry))
                syms_file.write("\n")
            
            # Write function symbols
            syms_file.write(".text = 0x0:\n")
            for func in dll_functions:
                syms_file.write("{} = 0x{:X};\n".format(func.symbol, func.address))
            
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

            # Note: Section syms need to be relative to start of .text
            if dll.has_rodata() and len(rodata_refs) > 0:
                syms_file.write("\n.rodata = 0x{:X}:\n"
                                .format(dll.header.rodata_offset + dll.reloc_table.get_size() - dll.header.size))
                rodata_refs.add(0)
                rodata_refs = list(rodata_refs)
                rodata_refs.sort()

                rodata_start = dll.header.rodata_offset + dll.reloc_table.get_size() # exclude relocation tables

                for ref in rodata_refs:
                    absolute = rodata_start + ref - text_start
                    syms_file.write("_rodata_{:X} = 0x{:X}; # absolute: 0x{:X}\n"
                                    .format(ref, ref, absolute))
            
            if dll.has_data():
                syms_file.write("\n.data = 0x{:X}:\n".format(dll.header.data_offset - dll.header.size))
                data_refs.add(0)
                data_refs = list(data_refs)
                data_refs.sort()

                data_start = dll.header.data_offset

                for ref in data_refs:
                    absolute = data_start + ref - text_start
                    syms_file.write("_data_{:X} = 0x{:X}; # absolute: 0x{:X}\n"
                                    .format(ref, ref, absolute))
            
            if bss_size > 0:
                syms_file.write("\n.bss = 0x{:X}:\n".format(dll.get_bss_offset() - dll.header.size))
                bss_refs.add(0)
                bss_refs = list(bss_refs)
                bss_refs.sort()
                
                bss_start = dll.get_bss_offset()

                for ref in bss_refs:
                    absolute = bss_start + ref - text_start
                    syms_file.write("_bss_{:X} = 0x{:X}; # absolute: 0x{:X}\n"
                                    .format(ref, ref, absolute))

    def __extract_text_asm(self, 
                           nonmatching_dir: Path,
                           matching_dir: Path, 
                           dll: DLL, 
                           dll_functions: "list[DLLFunctionWithLateRodata]",
                           dll_symbols: DLLSymbols,
                           dll_data: bytearray,
                           nonmatching_funcs: "list[str] | None"):
        rodata: bytearray | None = None
        rodata_start: int | None = None
        
        for func_with_late_rodata in dll_functions:
            func = func_with_late_rodata.func
            late_rodata_start = func_with_late_rodata.late_rodata_start
            late_rodata_end = func_with_late_rodata.late_rodata_end

            # Filter matching functions
            dir = nonmatching_dir
            if nonmatching_funcs is not None and not func.symbol in nonmatching_funcs:
                if self.disassemble_all:
                    dir = matching_dir
                else:
                    continue

            s_path = dir.joinpath(f"{func.symbol}.s")
            with open(s_path, "w", encoding="utf-8") as s_file:
                s_file.write("/* Generated by dll_split.py (disassembly by rabbitizer {}) */\n\n".format(rabbitizer.__version__))

                # Emit .late_rodata
                has_late_rodata = late_rodata_start != 0 or late_rodata_end != 0
                if has_late_rodata:
                    s_file.write(".section .late_rodata\n")

                    if rodata == None:
                        rodata_start = dll.header.rodata_offset + dll.reloc_table.get_size() # exclude relocation tables
                        rodata_end = rodata_start + dll.get_rodata_size()
                        rodata = dll_data[rodata_start:rodata_end]

                    rodata_refs: set[int] | list[int] = set()
                    for ref in func.local_rodata_refs:
                        if ref >= late_rodata_start and ref < late_rodata_end:
                            rodata_refs.add(ref)
                    for ref in func.jump_table_refs:
                        if ref >= late_rodata_start and ref < late_rodata_end:
                            rodata_refs.add(ref)
                    rodata_refs.add(late_rodata_start)
                    rodata_refs.add(late_rodata_end)
                    rodata_refs = list(rodata_refs)
                    rodata_refs.sort()

                    for i in range(len(rodata_refs) - 1):
                        offset = rodata_refs[i]
                        size = rodata_refs[i + 1] - offset
                        if offset in func.jump_table_refs:
                            # Emit jump table
                            jump_table = func.jump_tables[offset]

                            sym_name = dll_symbols.get_jtable_name_or_default(jump_table.offset)
                            s_file.write("dlabel {}\n".format(sym_name))

                            rom_addr = jump_table.address + dll.header.size
                            ram_addr = jump_table.address
                            for entry in jump_table.entries:
                                value_bytes = struct.pack(">i", entry.value).hex().upper()
                                value_str = ".gpword .L{:X}".format(entry.target)

                                s_file.write("/* {:0>4X} {:0>6X} {} */ {}\n"
                                    .format(rom_addr, ram_addr, value_bytes, value_str))
                                
                                rom_addr += 4
                                ram_addr += 4
                            
                            jump_table_size = len(jump_table.entries) * 4
                            leftovers = size - jump_table_size
                            if leftovers > 0:
                                self.__write_asm_data_definitions(s_file, rodata, 
                                                                  data_rom_start=rodata_start,
                                                                  data_vram_start=rodata_start - dll.header.size,
                                                                  start=offset + jump_table_size,
                                                                  end=offset + size,
                                                                  is_leftovers=True)
                            s_file.write(".size {}, . - {}\n".format(sym_name, sym_name))
                        else:
                            # Emit other rodata
                            sym_name = dll_symbols.get_local_name_or_default(1, offset)
                            s_file.write("dlabel {}\n".format(sym_name))
                            self.__write_asm_data_definitions(s_file, rodata, 
                                                              data_rom_start=rodata_start,
                                                              data_vram_start=rodata_start - dll.header.size,
                                                              start=offset,
                                                              end=offset + size,
                                                              is_leftovers=False,
                                                              type_hint=func.local_rodata_ref_info[offset].load_type)
                            s_file.write(".size {}, . - {}\n".format(sym_name, sym_name))
                        s_file.write("\n")

                # Emit .text
                if has_late_rodata:
                    s_file.write("\n.section .text\n")
                s_file.write(f"glabel {func.symbol}\n")

                for i in func.insts:
                    inst_str, label = stringify_instruction(i, func)
                    if label != None:
                        s_file.write(f"{label}\n")
                    
                    rom_addr = i.i.vram + dll.header.size
                    ram_addr = i.i.vram
                    inst_bytes = i.i.getRaw().to_bytes(length=4, byteorder="big").hex().upper()

                    s_file.write("/* {:0>4X} {:0>6X} {} */ {}\n"
                        .format(rom_addr, ram_addr, inst_bytes, inst_str))
                
                s_file.write(".size {}, . - {}\n".format(func.symbol, func.symbol))

    def __write_asm_data_definitions(self, 
                                     output: TextIO, 
                                     data: bytes, 
                                     data_rom_start: int, 
                                     data_vram_start: int, 
                                     start: int, 
                                     end: int,
                                     is_leftovers: bool,
                                     type_hint: DLLLocalRefLoadType | None=None):
        left = end - start
        rom_addr = data_rom_start + start
        ram_addr = data_vram_start + start
        offset = start

        if type_hint == DLLLocalRefLoadType.LWC1 and left >= 4:
            value = struct.unpack_from(">f", data, offset)[0]
            value_bytes = data[offset:offset+4].hex().upper()

            output.write("/* {:0>4X} {:0>6X} {} */ .float {}\n"
                         .format(rom_addr, ram_addr, value_bytes, value))
            left -= 4
            offset += 4
            rom_addr += 4
            ram_addr += 4
            is_leftovers = True
        elif type_hint == DLLLocalRefLoadType.LDC1 and left >= 8:
            value = struct.unpack_from(">d", data, offset)[0]
            value_bytes = data[offset:offset+8].hex().upper()

            output.write("/* {:0>4X} {:0>6X} {} */ .double {}\n"
                         .format(rom_addr, ram_addr, value_bytes, value))
            left -= 8
            offset += 8
            rom_addr += 8
            ram_addr += 8
            is_leftovers = True
        
        if is_leftovers and end == len(data) and left <= 8:
            # If the end of rodata is all zeroes, it's probably just padding from section alignment.
            # Skip it since including these zeroes can cause matching issues.
            all_zero = True
            for i in range(left):
                if data[offset + i] != 0x00:
                    all_zero = False
                    break
            
            if all_zero:
                return

        while left > 0 and (left % 4) == 0:
            value = struct.unpack_from(">I", data, offset)[0]
            value_bytes = struct.pack(">I", value).hex().upper()

            output.write("/* {:0>4X} {:0>6X} {} */ .word 0x{:X}\n"
                         .format(rom_addr, ram_addr, value_bytes, value))

            left -= 4
            offset += 4
            rom_addr += 4
            ram_addr += 4
        
        while left > 0:
            value = struct.unpack_from(">B", data, offset)[0]
            value_bytes = struct.pack(">B", value).hex().upper()

            output.write("/* {:0>4X} {:0>6X} {} */ .byte 0x{:X}\n"
                         .format(rom_addr, ram_addr, value_bytes, value))

            left -= 1
            offset += 1
            rom_addr += 1
            ram_addr += 1

    def __migrate_late_rodata(self, funcs: list[DLLFunction], rodata_size: int) -> tuple[list[DLLFunctionWithLateRodata], int]:
        with_rodata = [DLLFunctionWithLateRodata(func, 0, 0) for func in funcs]
        late_rodata_start = rodata_size

        referenced_offsets: dict[int, set[int]] = {}
        combined_ref_info: dict[int, DLLLocalRefInfo] = {}
        for i in range(len(funcs)):
            func = funcs[i]
            assert(func.local_rodata_refs != None)
            assert(func.jump_table_refs != None)

            for ref in func.local_rodata_refs:
                if not ref in referenced_offsets:
                    referenced_offsets[ref] = set()
                referenced_offsets[ref].add(i)
            for ref in func.jump_table_refs:
                if not ref in referenced_offsets:
                    referenced_offsets[ref] = set()
                referenced_offsets[ref].add(i)
            for (ref, info) in func.local_rodata_ref_info.items():
                combined_ref_info[ref] = info
        
        if len(referenced_offsets) == 0:
            # Nothing to migrate
            return (with_rodata, late_rodata_start)
        
        referenced_offsets_sorted: list[tuple[int, set[int]]] = []
        for (offset, func_refs) in referenced_offsets.items():
            referenced_offsets_sorted.append((offset, func_refs))
        
        referenced_offsets_sorted.sort(key=lambda t: t[0])

        last_offset = rodata_size
        last_func = len(funcs)

        # starting from the end of rodata, iterate references backwards  <-
        #   stop if the next offset is ref'd by a function after the one we're on
        #   stop if the next offset has multiple func references
        #   stop if the next offset has no refs
        i = len(referenced_offsets_sorted) - 1
        while i >= 0:
            (offset, func_refs) = referenced_offsets_sorted[i]

            ref_info = combined_ref_info[offset]
            if ref_info.load_type == DLLLocalRefLoadType.UNKNOWN:
                break

            if len(func_refs) != 1:
                break

            func_ref = func_refs.pop()
            if func_ref > last_func:
                break

            if func_ref == last_func:
                with_rodata[last_func].late_rodata_start = offset
            else:
                last_func = func_ref
                with_rodata[last_func].late_rodata_start = offset
                with_rodata[last_func].late_rodata_end = last_offset
                
            last_offset = offset
            i -= 1
        
        late_rodata_start = last_offset

        return (with_rodata, late_rodata_start)

    def __get_nonmatching_functions_from_c(self, path: Path) -> "list[str]":
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
        
        with open(path, "r", encoding="utf-8") as syms_file:
            return DLLSymsTxt.parse(syms_file).to_absolute()

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
    parser.add_argument("--disassemble-all", dest="disassemble_all", action="store_true", help="Disasemble matched functions and migrated data.", default=False)
    parser.add_argument("dlls", nargs="*", action="extend", help="The numbers of each DLL to extract. Don't specify any to extract all that have src directories.")

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    # Extract DLLs
    start = timer()
    
    splitter = DLLSplitter(verbose=args.verbose, disassemble_all=args.disassemble_all)
    splitter.extract_dlls(args.dlls)
    
    end = timer()
    if not args.quiet:
        print("DLL extraction complete (took {:.3} seconds).".format(end - start))

if __name__ == "__main__":
    main()
