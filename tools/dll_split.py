#!/usr/bin/env python3
# A mini version of splat specifically for Dinosaur Planet DLLs
# dll_split.py is responsible for:
# - Extracting assembly and data from Dinosaur Planet DLLs
# - Extracting in such a way that is ready for the decomp project's build system
# - Creating .c, exports.s, and syms.txt stubs

import argparse
import glob
import os
import sys
import spimdisasm
from pathlib import Path
import rabbitizer
import re
from timeit import default_timer as timer
from typing import TextIO

from dino.dll import DLL
from dino.dll_analysis import DLL_VRAM_BASE, AnalyzedDLL, analyze_dll
from dino.dll_build_config import DLLBuildConfig
from dino.dll_tab import DLLTab
from dino.dlls_txt import DLLsTxt
from dino.dll_syms_txt import DLLSymsTxt

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
ASM_PATH = Path("asm")
BIN_PATH = Path("bin")
SRC_PATH = Path("src")

global_asm_pattern = re.compile(r"#pragma GLOBAL_ASM\(\"asm\/nonmatchings\/dlls\/\S+\/(.+)\.s\"\)")

class DLLSplitterException(Exception):
    pass

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
        
        core_export_syms = self.__read_symbols_file(Path("export_symbol_addrs.txt"))
        assert core_export_syms, "Missing export_symbol_addrs.txt"
        
        # Extract each DLL that has a src directory
        dlls_src_path = SRC_PATH.joinpath("dlls")
        dlls_txt_path = dlls_src_path.joinpath("dlls.txt")
        assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
        
        with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
            dlls_txt = DLLsTxt.parse(dlls_txt_file)

        i = 0
        count = 796 if len(only_dlls) == 0 else len(only_dlls)
        for (number, dll_dir) in dlls_txt.path_map.items():
            # Skip DLL if not in list
            if len(only_dlls) > 0 and not str(number) in only_dlls:
                continue

            if not self.verbose:
                if sys.stdout.isatty():
                    print("\033[K", end="\r")
                    print("[{}/{}] DLL {}: {}".format(i + 1, count, number, dll_dir), end="")
                else:
                    print("[{}/{}] DLL {}: {}".format(i + 1, count, number, dll_dir))

            i += 1

            dir = dlls_src_path.joinpath(dll_dir)

            if not dir.exists():
                raise DLLSplitterException(f"No such DLL src directory {dir}!")

            # Check DLL path
            dll_path = BIN_PATH.joinpath(f"assets/dlls/{number}.dll")
            if not dll_path.exists():
                raise DLLSplitterException(f"No such DLL {dll_path}!")

            # Load known symbols for DLL
            symbol_files: list[DLLSymsTxt] = [core_export_syms]
            dll_syms = self.__read_symbols_file(dir.joinpath("syms.txt"))
            if dll_syms != None:
                symbol_files.append(dll_syms)

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
                
                # Analyze DLL
                analyzed_dll = analyze_dll(dll, number, data, symbol_files, bss_size)
                if dll_syms == None:
                    self.__default_symbol_names(analyzed_dll)
                
                end = timer()
                if self.verbose:
                    print("[{}] Parsing complete (took {:.3} seconds).".format(number, end - start))
            
                # Create DLL config if it doesn't exist
                dll_config_path = dir.joinpath("dll.yaml")
                if not dll_config_path.exists():
                    dll_config = DLLBuildConfig(compile=True)
                    with open(dll_config_path, "w") as file:
                        dll_config.save(file)

                # Extract DLL
                if self.verbose:
                    print("[{}] Extracting...".format(number))
                start = timer()

                self.extract_dll(analyzed_dll, dll_dir)

                end = timer()
                if self.verbose:
                    print("[{}] Extracting complete (took {:.3} seconds).".format(number, end - start))
        
        # Extract DLLs *without* a src directory as an asm stub
        for number in range(796):
            number += 1

            # Skip if DLL has a src directory
            if number in dlls_txt.path_map:
                continue

            # Skip DLL if not in list
            if len(only_dlls) > 0 and not str(number) in only_dlls:
                continue

            if not self.verbose:
                if sys.stdout.isatty():
                    print("\033[K", end="\r")
                    print("[{}/{}] DLL {}: ASM".format(i + 1, count, number), end="")
                else:
                    print("[{}/{}] DLL {}: ASM".format(i + 1, count, number))
            i += 1

            # Check DLL path
            dll_path = BIN_PATH.joinpath(f"assets/dlls/{number}.dll")
            if not dll_path.exists():
                raise DLLSplitterException(f"No such DLL {dll_path}!")

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
                
                # Analyze DLL
                analyzed_dll = analyze_dll(dll, number, data, [core_export_syms], bss_size)
                self.__default_symbol_names(analyzed_dll)
                
                end = timer()
                if self.verbose:
                    print("[{}] Parsing complete (took {:.3} seconds).".format(number, end - start))

                # Extract DLL
                if self.verbose:
                    print("[{}] Extracting...".format(number))
                start = timer()

                self.extract_asm_dll(analyzed_dll)

                end = timer()
                if self.verbose:
                    print("[{}] Extracting complete (took {:.3} seconds).".format(number, end - start))

        if not self.verbose and sys.stdout.isatty():
            print("\033[K", end="\r")

    def __default_symbol_names(self, dll: AnalyzedDLL):
        if dll.number >= 210:
            # Object DLL
            OBJECT_INTERFACE = [
                "create",
                "update",
                None,
                "draw",
                "destroy",
                "get_model_flags",
                "get_state_size"
            ]

            for i, export in enumerate(dll.meta.header.export_offsets):
                if i >= len(OBJECT_INTERFACE):
                    break
                name = OBJECT_INTERFACE[i]
                if name == None:
                    continue
                sym = dll.context.globalSegment.addSymbol(DLL_VRAM_BASE + export,
                                                          sectionType=spimdisasm.common.FileSectionType.Text,
                                                          isAutogenerated=True)
                sym.name = f"dll_{dll.number}_{name}"

    def extract_dll(self, 
                    dll: AnalyzedDLL, 
                    dll_dir: str):
        # Determine paths
        src_path = SRC_PATH.joinpath(f"dlls/{dll_dir}")
        nonmatching_asm_path = ASM_PATH.joinpath(f"nonmatchings/dlls/{dll_dir}")
        matching_asm_path = ASM_PATH.joinpath(f"matchings/dlls/{dll_dir}")

        # Determine what needs to be extracted
        nonmatching_funcs: "list[str] | None" = None
        for c_file_path in [Path(path) for path in glob.glob(f"{src_path}/*.c", recursive=True)]:
            if nonmatching_funcs == None:
                nonmatching_funcs = []
            nonmatching_funcs.extend(self.__get_nonmatching_functions_from_c(c_file_path))

        # Create directories if necessary
        if nonmatching_funcs == None or len(nonmatching_funcs) > 0:
            os.makedirs(nonmatching_asm_path, exist_ok=True)
        if self.disassemble_all and (nonmatching_funcs == None or len(dll.functions) != len(nonmatching_funcs)):
            os.makedirs(matching_asm_path, exist_ok=True)

        # Extract .text
        if nonmatching_funcs == None or len(nonmatching_funcs) > 0 or self.disassemble_all:
            self.__extract_text_asm(nonmatching_asm_path, matching_asm_path, dll, nonmatching_funcs)

        # Create exports.s if it doesn't exist
        exports_s_path = src_path.joinpath("exports.s")
        if not exports_s_path.exists():
            self.__create_exports_s(exports_s_path, dll)

        # Create syms.txt if it doens't exist
        syms_txt_path = src_path.joinpath("syms.txt")
        if not syms_txt_path.exists():
            self.__create_syms_txt(syms_txt_path, dll)

        # Create <dll>.c stub if no .c files exist
        if nonmatching_funcs == None:
            c_file_path = src_path.joinpath(f"{dll.number}.c")
            self.__create_c_stub(c_file_path, nonmatching_asm_path, dll)

    def extract_asm_dll(self, dll: AnalyzedDLL):
        # Determine path
        s_path = ASM_PATH.joinpath(f"nonmatchings/dlls/_asm/{dll.number}.s")
        
        # Create directory if necessary
        os.makedirs(s_path.parent, exist_ok=True)

        # Create single ASM file for DLL
        with open(s_path, "w", encoding="utf-8") as dll_s:
            # Prelude
            dll_s.write(".include \"macro.inc\"\n")
            dll_s.write("\n")
            dll_s.write(".set noat\n")
            dll_s.write(".set noreorder\n")
            dll_s.write(".set gp=64\n")
            dll_s.write(".set fp=64\n")
            dll_s.write("\n")
            dll_s.write(".option pic2\n")
            dll_s.write("\n")

            # Exports
            funcs_by_address: "dict[int, str]" = {}
            for func in dll.functions:
                funcs_by_address[func.vram - DLL_VRAM_BASE] = func.getName()
            
            dll_s.write(".section \".exports\"\n")
            dll_s.write("\n")
            dll_s.write(f".dword {funcs_by_address[dll.meta.header.ctor_offset]}\n")
            dll_s.write(f".dword {funcs_by_address[dll.meta.header.dtor_offset]}\n")
            for offset in dll.meta.header.export_offsets:
                func_symbol = funcs_by_address[offset]
                dll_s.write(f".dword {func_symbol}\n")

            # .rodata
            if dll.rodata != None:
                dll_s.write("\n")
                dll_s.write(".section .rodata, \"a\"\n")
                dll_s.write("\n")
                dll_s.write(dll.rodata.disassemble())
            
            # .data
            if dll.data != None:
                dll_s.write("\n")
                dll_s.write(".section .data, \"wa\"\n")
                dll_s.write("\n")
                dll_s.write(dll.data.disassemble())
            
            # .bss
            if dll.bss != None:
                dll_s.write("\n")
                dll_s.write(".section .bss, \"wa\"\n")
                dll_s.write("\n")
                dll_s.write(dll.bss.disassemble())
            
            # .text
            dll_s.write("\n")
            dll_s.write(".section .text, \"ax\"\n")
            for func in dll.functions:
                dll_s.write("\n")
                dll_s.write(func.disassemble())

        # Undefined symbols
        if len(dll.oob_syms) > 0:
            with open(s_path.with_suffix(".undefined_syms.ld"), "w", encoding="utf-8") as undef_syms_s:
                undef_syms_s.write("SECTIONS\n")
                undef_syms_s.write("{\n")
                undef_syms_s.write(f"    {dll.last_section.getSectionName()} : {{\n")
                for sym in dll.oob_syms:
                    undef_syms_s.write(f"        {sym.getName()} = 0x{(sym.vram - dll.last_section.vram):X};\n")
                undef_syms_s.write("    }\n")
                undef_syms_s.write("}\n")

    def __create_exports_s(self, path: Path, dll: AnalyzedDLL):
        funcs_by_address: "dict[int, str]" = {}
        for func in dll.functions:
            funcs_by_address[func.vram - DLL_VRAM_BASE] = func.getName()

        with open(path, "w", encoding="utf-8") as exports_s:
            exports_s.write(".option pic2\n")
            exports_s.write(".section \".exports\"\n")
            exports_s.write(".global _exports\n")
            exports_s.write("_exports:\n")
            exports_s.write("\n")

            exports_s.write(f"# ctor/dtor\n")
            exports_s.write(f".dword {funcs_by_address[dll.meta.header.ctor_offset]}\n")
            exports_s.write(f".dword {funcs_by_address[dll.meta.header.dtor_offset]}\n")
            exports_s.write("\n")

            exports_s.write(f"# export table\n")
            for i, offset in enumerate(dll.meta.header.export_offsets):
                func_symbol = funcs_by_address[offset]
                exports_s.write(f"/*{i}*/ .dword {func_symbol}\n")

    def __create_c_stub(self, 
                        c_path: Path, 
                        asm_path: Path, 
                        dll: AnalyzedDLL):
        # Map exports
        exports: "dict[int, int]" = {}
        for i, offset in enumerate(dll.meta.header.export_offsets):
            exports[offset] = i
        
        extern_offsets = dll.meta.reloc_table.global_offset_table[4:]

        # If an object DLL, try to figure out the state size for auto decomp
        obj_state_size: int | None = None
        if dll.number >= 210:
            obj_state_size = self.__try_find_object_state_size(dll, exports)
        
        # Write
        with open(c_path, "w", encoding="utf-8") as c_file:
            # Includes
            c_file.write("#include \"common.h\"\n")
            if dll.number >= 210:
                if obj_state_size != None and obj_state_size != 0:
                    c_file.write("\ntypedef struct {\n")
                    c_file.write(f"    u8 _unk0[0x{obj_state_size:X}];\n")
                    c_file.write(f"}} DLL{dll.number}_State;\n")
            
            # .data stubs
            if dll.data != None:
                data_start = dll.meta.header.data_offset - dll.meta.header.size
                data_relocs = set(dll.meta.reloc_table.data_relocations)

                c_file.write("\n")
                for sym in dll.data.symbolList:
                    assert(isinstance(sym, spimdisasm.mips.symbols.SymbolData))
                    address = sym.vram - DLL_VRAM_BASE
                    offset = address - data_start
                    static = not address in extern_offsets

                    self.__write_c_stub_data(
                        c_file, 
                        sym.words, 
                        "_data_", 
                        const=False,
                        static=static,
                        field_offset=offset,
                        access_type=sym.contextSym.accessType,
                        relocs=data_relocs)
            
            # .bss stubs
            if dll.bss != None:
                bss_start = dll.meta.get_bss_offset() - dll.meta.header.size

                c_file.write("\n")
                for sym in dll.bss.symbolList:
                    assert(isinstance(sym, spimdisasm.mips.symbols.SymbolBss))
                    address = sym.vram - DLL_VRAM_BASE
                    offset = address - bss_start
                    static = not address in extern_offsets
                    size_left = sym.spaceSize

                    first = True
                    while size_left > 0:
                        size = size_left

                        offset_alignment = self.__determine_alignment(offset)
                        if offset_alignment < 16:
                            # If the size we're trying to stub is bigger than the offset's alignment, IDO will
                            # add padding before the stubbed variable and mess everything up. So we need
                            # to split up these into additional "fake symbols".
                            max_size = offset_alignment
                            if max_size < size:
                                size = max_size
                        
                        if first:
                            first = False
                            c_file.write("/*0x{:X}*/ {}u8 {}[{:#x}];\n"
                                .format(offset, "static " if static else "", sym.getName(), size))
                        else:
                            c_file.write("/*0x{:X}*/ static u8 _bss_{:X}[{:#x}];\n"
                                .format(offset, offset, size))
                            
                        offset += size
                        size_left -= size
            
            # function stubs and .rodata
            rodata_start = dll.meta.header.rodata_offset - dll.meta.header.size \
                + dll.meta.reloc_table.get_size() # exclude relocation tables
            writing_rodata: bool | None = None
            func_num = 0
            for func_rodata in dll.function_rodata:
                if func_rodata.function == None:
                    if writing_rodata != True:
                        c_file.write("\n")
                        writing_rodata = True
                    for sym in func_rodata.iterRodataSyms():
                        assert(isinstance(sym, spimdisasm.mips.symbols.SymbolRodata))
                        address = sym.vram - DLL_VRAM_BASE
                        offset = address - rodata_start
                        static = not address in extern_offsets

                        if sym.isString():
                            # Attempt to decode strings like spimdisasm does
                            # This roughly follows the logic of spimdisasm's SymbolBase.getNthWordAsString function
                            buffer = spimdisasm.common.Utils.wordsToBytes(sym.words)

                            i = 0
                            while i < sym.sizew:
                                decodedStrings, rawStringSize = spimdisasm.common.Utils.decodeBytesToStrings(
                                    buffer, 4 * i, sym.stringEncoding)
                                
                                if rawStringSize < 0:
                                    skip = 0

                                    c_file.write("// Failed to decode string at .rodata+0x{:X}\n".format(offset + (i * 4)))
                                    self.__write_c_stub_data(
                                        c_file, 
                                        sym.words[i:i+1], 
                                        "_rodata_", 
                                        const=True,
                                        static=static,
                                        field_offset=offset + (i * 4))
                                else:
                                    skip = rawStringSize // 4

                                    if rawStringSize == 0:
                                        decodedStrings.append("")

                                    c_file.write("/*0x{:X}*/ static const char str_{:X}[] = \"{}\";\n"
                                        .format(offset + (i * 4), offset + (i * 4), decodedStrings[-1]))

                                i += skip
                                i += 1
                        else:
                            self.__write_c_stub_data(
                                c_file, 
                                sym.words, 
                                "_rodata{}_".format("" if not sym.isJumpTable() else "_jtbl"), 
                                const=True,
                                static=static,
                                field_offset=offset,
                                access_type=sym.contextSym.accessType)
                else:
                    writing_rodata = False

                    func = func_rodata.function

                    address = func.vram - DLL_VRAM_BASE
                    
                    metadata = ["offset: 0x{:X}".format(address)]
                    
                    ctor_dtor = False
                    if address == dll.meta.header.ctor_offset:
                        metadata.append("ctor")
                        ctor_dtor = True
                    elif address == dll.meta.header.dtor_offset:
                        metadata.append("dtor")
                        ctor_dtor = True
                    else:
                        metadata.append(f"func: {func_num}")
                        func_num += 1

                    export_idx = exports.get(address, None)
                    if export_idx != None:
                        metadata.append(f"export: {export_idx}")

                    c_file.write("\n")
                    c_file.write(f'// {" | ".join(metadata)}\n')
                    if not self.__try_auto_decomp(dll, func, ctor_dtor, export_idx, obj_state_size, c_file):
                        c_file.write(f'#pragma GLOBAL_ASM("{asm_path.as_posix()}/{func.getName()}.s")\n')

    EMPTY_CTOR_DTOR = [0xAFA40000, 0x03E00008, 0x00000000]
    DEFAULT_OBJ_CREATE = [0xAFA40000, 0xAFA50004,  0xAFA60008, 0x03E00008, 0x000000]
    DEFAULT_OBJ_UPDATE = [0xAFA40000, 0x03E00008, 0x00000000]
    DEFAULT_OBJ_FUNC3 = [0xAFA40000, 0x03E00008, 0x00000000]
    DEFAULT_OBJ_DRAW = [0xAFA40000, 0xAFA50004,  0xAFA60008, 0xAFA7000C, 0x03E00008, 0x000000]
    DEFAULT_OBJ_DESTROY = [0xAFA40000, 0xAFA50004, 0x03E00008, 0x000000]
    DEFAULT_OBJ_GET_MODEL_FLAGS = [0xAFA40000, 0x00001025, 0x03E00008, 0x000000]

    def __instructions_equal(self, func: spimdisasm.mips.symbols.SymbolFunction, inst_bytes: list[int]):
        if len(func.instructions) == len(inst_bytes):
            return [i.getRaw() for i in func.instructions] == inst_bytes
        else:
            return False

    def __try_auto_decomp(self, 
                          dll: AnalyzedDLL,
                          func: spimdisasm.mips.symbols.SymbolFunction, 
                          ctor_dtor: bool,
                          export_idx: int | None,
                          obj_state_size: int | None,
                          c_file: TextIO) -> bool:
        if ctor_dtor:
            # Check for empty constructor/destructor
            if self.__instructions_equal(func, self.EMPTY_CTOR_DTOR):
                c_file.write(f'void {func.getName()}(void *dll) {{ }}\n')
                return True
            else:
                c_file.write(f'void {func.getName()}(void *dll);\n')
            return False
        
        if dll.number >= 210:
            # Object DLL, check for function defaults
            if export_idx == 0:
                if self.__instructions_equal(func, self.DEFAULT_OBJ_CREATE):
                    c_file.write(f'void {func.getName()}(Object *self, ObjCreateInfo *createInfo, s32 arg2) {{ }}\n')
                    return True
                else:
                    c_file.write(f'void {func.getName()}(Object *self, ObjCreateInfo *createInfo, s32 arg2);\n')
            elif export_idx == 1:
                if self.__instructions_equal(func, self.DEFAULT_OBJ_UPDATE):
                    c_file.write(f'void {func.getName()}(Object *self) {{ }}\n')
                    return True
                else:
                    c_file.write(f'void {func.getName()}(Object *self);\n')
            elif export_idx == 2:
                if self.__instructions_equal(func, self.DEFAULT_OBJ_FUNC3):
                    c_file.write(f'void {func.getName()}(Object *self) {{ }}\n')
                    return True
                else:
                    c_file.write(f'void {func.getName()}(Object *self);\n')
            elif export_idx == 3:
                if self.__instructions_equal(func, self.DEFAULT_OBJ_DRAW):
                    c_file.write(f'void {func.getName()}(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {{ }}\n')
                    return True
                else:
                    c_file.write(f'void {func.getName()}(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);\n')
            elif export_idx == 4:
                if self.__instructions_equal(func, self.DEFAULT_OBJ_DESTROY):
                    c_file.write(f'void {func.getName()}(Object *self, s32 a1) {{ }}\n')
                    return True
                else:
                    c_file.write(f'void {func.getName()}(Object *self, s32 a1);\n')
            elif export_idx == 5:
                if self.__instructions_equal(func, self.DEFAULT_OBJ_GET_MODEL_FLAGS):
                    c_file.write(f'u32 {func.getName()}(Object *self) {{\n')
                    c_file.write(f'    return MODFLAGS_NONE;\n')
                    c_file.write('}\n')
                    return True
                else:
                    c_file.write(f'u32 {func.getName()}(Object *self);\n')
            elif export_idx == 6:
                if obj_state_size != None:
                    c_file.write(f'u32 {func.getName()}(Object *self, u32 a1) {{\n')
                    if obj_state_size == 0:
                        c_file.write(f'    return 0;\n')
                    else:
                        c_file.write(f'    return sizeof(DLL{dll.number}_State);\n')
                    c_file.write('}\n')
                    return True
                else:
                    c_file.write(f'u32 {func.getName()}(Object *self, u32 a1);\n')
        
        return False

    def __try_find_object_state_size(self, dll: AnalyzedDLL, exports: dict[int, int]):
        if dll.number < 210:
            return None

        for func_rodata in dll.function_rodata:
            if func_rodata.function != None:
                func = func_rodata.function
                address = func.vram - DLL_VRAM_BASE
                export_idx = exports.get(address, None)

                if export_idx == 6:
                    if len(func.instructions) == 5:
                        if func.instructions[0].getRaw() == 0xAFA40000 and func.instructions[1].getRaw() == 0xAFA50004 and \
                           func.instructions[3].getRaw() == 0x03E00008 and func.instructions[4].getRaw() == 0x00000000:
                            inst = func.instructions[2].getRaw()
                            if (inst >> 0x10) == 0x2402:
                                # li instruction
                                state_size = inst & 0xFFFF
                                return state_size
                            elif inst == 0x00001025:
                                # or $v0, $zero, $zero
                                return 0
                    return None

        return None

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
                            words: list[int], 
                            prefix: str, 
                            const: bool, 
                            static: bool, 
                            field_offset: int, 
                            access_type: rabbitizer.Enum | None=None,
                            relocs: set[int] | None=None):
        name = "{}{:X}".format(prefix, field_offset)

        data_type = "u32"
        # Note: .rodata doesn't seem to match correctly when collapsing arrays.
        #       When this happens, it seems to mainly be short strings that weren't detected correctly.
        array = len(words) > 1 or const
        if access_type == rabbitizer.AccessType.FLOAT:
            data_type = "f32"

        if array:
            c_file.write("/*0x{:X}*/ {}{}{} {}[] = {{\n".format(
                field_offset,
                "static " if static else "", 
                "const " if const else "", 
                data_type,
                name))
        else:
            c_file.write("/*0x{:X}*/ {}{}{} {} = ".format(
                field_offset,
                "static " if static else "", 
                "const " if const else "", 
                data_type,
                name))
            
        if access_type == rabbitizer.AccessType.FLOAT:
            for i, word in enumerate(words):
                if array:
                    if i > 0:
                        c_file.write(", ")
                        if (i % 8) == 0:
                            c_file.write("\n    ")
                    else:
                        c_file.write("    ")
                assert(relocs == None or not (field_offset + (i * 4)) in relocs)
                c_file.write(str(spimdisasm.common.Utils.wordToFloat(word)))
        else:
            for i, word in enumerate(words):
                if array:
                    if i > 0:
                        c_file.write(", ")
                        if (i % 8) == 0:
                            c_file.write("\n    ")
                    else:
                        c_file.write("    ")
                if relocs != None and (field_offset + (i * 4)) in relocs:
                    # Assumes reloc'd pointers always point to the same section
                    c_file.write("(u32)&{}{:X}".format(prefix, word))
                else:
                    c_file.write("{:0=#10x}".format(word))

        if array:
            c_file.write("\n};\n")
        else:
            c_file.write(";\n")     

    def __create_syms_txt(self, 
                          syms_path: Path, 
                          dll: AnalyzedDLL):
        with open(syms_path, "w", encoding="utf-8") as syms_file:
            # .text
            syms_file.write(".text = 0x0:\n")
            for sym in dll.text.symbolList:
                syms_file.write("{} = 0x{:X};\n".format(sym.getName(), sym.vram - dll.text.vram))

            # .rodata
            if dll.rodata != None:
                rodata_start = dll.meta.header.rodata_offset + dll.meta.reloc_table.get_size() \
                    - dll.meta.header.size
                syms_file.write("\n.rodata = 0x{:X}:\n".format(rodata_start))

                for entry in dll.function_rodata:
                    # Write symbols for orphaned rodata
                    if entry.function == None:
                        for sym in entry.iterRodataSyms():
                            offset = sym.vram - dll.rodata.vram
                            absolute = rodata_start + offset
                            syms_file.write("{} = 0x{:X}; // absolute:0x{:X}\n"
                                            .format(sym.getName(), offset, absolute))
            
            # .data
            if dll.data != None:
                data_start = dll.meta.header.data_offset - dll.meta.header.size
                syms_file.write("\n.data = 0x{:X}:\n".format(data_start))

                for sym in dll.context.globalSegment.symbols.values():
                    if sym.sectionType == spimdisasm.common.FileSectionType.Data:
                        offset = sym.vram - dll.data.vram
                        absolute = data_start + offset
                        syms_file.write("{} = 0x{:X}; // absolute:0x{:X}\n"
                                        .format(sym.getName(), offset, absolute))
            
            # .bss
            if dll.bss != None:
                bss_start = dll.meta.get_bss_offset() - dll.meta.header.size
                syms_file.write("\n.bss = 0x{:X}:\n".format(bss_start))

                for sym in dll.context.globalSegment.symbols.values():
                    if sym.sectionType == spimdisasm.common.FileSectionType.Bss:
                        offset = sym.vram - dll.bss.vram
                        absolute = bss_start + offset
                        syms_file.write("{} = 0x{:X}; // absolute:0x{:X}\n"
                                        .format(sym.getName(), offset, absolute))
            
            # oob syms
            if len(dll.oob_syms) > 0:
                last_section_start = dll.last_section.vromStart - dll.meta.header.size
                for sym in dll.oob_syms:
                    offset = sym.vram - dll.last_section.vram
                    absolute = last_section_start + offset
                    syms_file.write("{} = 0x{:X}; // absolute:0x{:X}\n"
                                    .format(sym.getName(), offset, absolute))

    def __extract_text_asm(self, 
                           nonmatching_dir: Path,
                           matching_dir: Path, 
                           dll: AnalyzedDLL,
                           nonmatching_funcs: "list[str] | None"):
        for func in dll.function_rodata:
            # Filter matching functions
            dir = nonmatching_dir
            if nonmatching_funcs is not None and not func.getName() in nonmatching_funcs:
                if self.disassemble_all:
                    dir = matching_dir
                else:
                    continue

            s_path = dir.joinpath(f"{func.getName()}.s")
            
            with open(s_path, "w", encoding="utf-8") as s_file:
                func.writeToFile(s_file)

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

    def __read_symbols_file(self, path: Path) -> DLLSymsTxt | None:
        if not path.exists():
            return None
        
        with open(path, "r", encoding="utf-8") as syms_file:
            return DLLSymsTxt.parse(syms_file)

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
