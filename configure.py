#!/usr/bin/env python3

from enum import Enum
from genericpath import isdir
import glob
import os
from pathlib import Path
from typing import TypedDict
from ninja import ninja_syntax

class BuildFileType(Enum):
    C = 1
    ASM = 2
    BIN = 3

class OptimizationLevel(Enum):
    O2 = "-O2"
    O1 = "-O1" 
    O2g0 = "-O2 -g0"

class BuildFile(TypedDict):
    src_path: str
    obj_path: str
    type: BuildFileType
    opt: "OptimizationLevel | None"

class DLL(TypedDict):
    number: str
    dir: str
    files: "list[BuildFile]"

class BuildFiles(TypedDict):
    files: "list[BuildFile]" # excludes DLLs
    dlls: "list[DLL]"
    leftover_dlls: "list[BuildFile]" # Uncompiled DLLs

class BuildNinjaWriter:
    def __init__(self, writer: ninja_syntax.Writer, input: BuildFiles):
        self.writer = writer
        self.input = input
        self.link_deps: "list[str]" = []

    def write(self):
        # Write prelude (variables, rules)
        self.__write_prelude()

        # Write builds for source compilation
        self.__write_file_builds()
        
        # Write DLL builds/linking
        self.__write_dll_builds()

        # Write main linker step
        self.__write_linking()

        # Done
        self.writer.close()
    
    def __write_prelude(self):
        # Write flag variables
        self.writer.comment("Flags")

        self.writer.variable("CC_DEFINES", " ".join([
            "-D_LANGUAGE_C", 
            "-D_FINALROM", 
            "-DF3DEX_GBI_2", 
            "-D_MIPS_SZLONG=32"
        ]))

        self.writer.variable("CC_INCLUDES", " ".join([
            "-I .",
            "-I include"
        ]))

        self.writer.variable("OPT_FLAGS", " ".join([
            "-O2",
            "-g3"
        ]))
        
        self.writer.variable("CC_FLAGS", " ".join([
            "-G 0",
            "-mips2",
            "-non_shared",
            "-Xfullwarn",
            "-Xcpluscomm",
            "-Wab,-r4300_mul",
            "$CC_DEFINES",
            "$CC_INCLUDES"
        ]))

        self.writer.variable("CC_FLAGS_DLL", " ".join([
            "-mips2",
            "-KPIC",
            "-w",
            "-Xcpluscomm",
            "-Wab,-r4300_mul",
            "$CC_DEFINES",
            "$CC_INCLUDES"
        ]))

        self.writer.variable("AS_FLAGS", " ".join([
            "-EB",
            "-mtune=vr4300",
            "-march=vr4300",
            "-Iinclude", # use variable here?
            "-modd-spreg"
        ]))

        self.writer.variable("GCC_FLAGS", " ".join([
            "-m32", 
            "-nostdinc", 
            "-std=gnu90",
            "$CC_DEFINES",
            "$CC_INCLUDES",
            "-fsyntax-only", 
            "-fno-builtin", 
            "-fsigned-char", 
            "-fno-PIC",
            "-fno-zero-initialized-in-bss",
            "-fno-toplevel-reorder",
            "-Wall",
            "-Wextra",
            "-Wno-missing-braces",
            "-Wno-unknown-pragmas"
            "-Wno-format-security",
            "-Wno-main"
        ]))

        self.writer.variable("GCC_FLAGS_DLL", " ".join([
            "-nostdinc",
            "-std=gnu90",
            "$CC_DEFINES",
            "$CC_INCLUDES",
            #"-march=vr4300",
            #"-mtune=vr4300",
            #"-mfix4300",
            "-fPIC",
            "-fno-stack-protector",
            "-fno-builtin",
            "-fno-common",
            "-fsigned-char"
        ]))

        self.writer.variable("LD_FLAGS", " ".join([
            "-T undefined_syms.txt", 
            "-T undefined_funcs.txt", 
            "-T undefined_syms_auto.txt", 
            "-T build/dino.ld",
            "-Map build/dino.map",
            "--no-check-sections"
        ]))

        self.writer.variable("LD_FLAGS_DLL", " ".join([
            "-nostartfiles",
            "-nodefaultlibs",
            "-r",
            "-T src/dlls/dll.ld",
            "--emit-relocs"
        ]))

        self.writer.newline()

        # Write tools
        cross = "mips-linux-gnu-"

        self.writer.comment("Tools")
        self.writer.variable("AS", f"{cross}as")
        self.writer.variable("LD", f"{cross}ld")
        self.writer.variable("OBJCOPY", f"{cross}objcopy")
        self.writer.variable("CC", "tools/ido_recomp/linux/5.3/cc")
        self.writer.variable("ASM_PROCESSOR", "python3 tools/asm_processor/build.py")
        self.writer.variable("CC_PREPROCESSED", "$ASM_PROCESSOR $CC -- $AS $AS_FLAGS --")
        self.writer.variable("GCC", "gcc")
        self.writer.variable("ELF2DLL", "tools/elf2dll")
        self.writer.variable("DINODLL", "python3 tools/dino_dll.py")
        
        self.writer.newline()

        # Write rules
        self.writer.comment("Rules")
        self.writer.rule("cc", 
            "$GCC -MM -MF $out.d -MT $out $GCC_FLAGS $in && $CC_PREPROCESSED -c $CC_FLAGS $OPT_FLAGS -o $out $in", 
            "Compiling $in...",
            depfile="$out.d")
        self.writer.rule("cc_dll", 
            "$GCC -MM -MF $out.d -MT $out $GCC_FLAGS_DLL $in && $CC_PREPROCESSED -c $CC_FLAGS_DLL $OPT_FLAGS -o $out $in", 
            "Compiling $in...",
            depfile="$out.d")
        self.writer.rule("as", "$AS $AS_FLAGS -o $out $in", "Assembling $in...")
        self.writer.rule("preprocess_linker_script", "cpp -P -DBUILD_DIR=build -o $out $in", "Pre-processing linker script...")
        self.writer.rule("ld", "$LD $LD_FLAGS -o $out", "Linking...")
        self.writer.rule("ld_dll", "$LD $LD_FLAGS_DLL $in -o $out", "Linking...")
        self.writer.rule("ld_bin", "$LD -r -b binary -o $out $in", "Linking binary $in...")
        self.writer.rule("binary_copy", "$OBJCOPY $in $out -O binary", "Copying binary $in to $out...")
        self.writer.rule("file_copy", "cp $in $out", "Copying $in to $out...")
        self.writer.rule("elf2dll", "$ELF2DLL $in $out", "Converting $in to DP DLL $out...")
        self.writer.rule("pack_dlls", "$DINODLL pack build/bin/assets/dlls $out bin/assets/DLLS_tab.bin ", "Packing DLLs...")

        self.writer.newline()

    def __write_file_builds(self):
        self.writer.comment("Source compilation")

        for file in self.input["files"]:
            # Determine variables
            variables: dict[str, str] = {}
            opt = file["opt"]
            if opt != None and opt != OptimizationLevel.O2:
                variables["OPT_FLAGS"] = opt.value

            # Determine command
            type = file["type"]
            command: str
            if type == BuildFileType.C:
                command = "cc"
            elif type == BuildFileType.ASM:
                command = "as"
            elif type == BuildFileType.BIN:
                command = "ld_bin"
            else:
                raise NotImplementedError()
            
            # Write command
            self.writer.build(file["obj_path"], command, file["src_path"], variables=variables)
            self.link_deps.append(file["obj_path"])

        self.writer.newline()
    
    def __write_dll_builds(self):
        self.writer.comment("DLL compilation")

        pack_deps: "list[str]" = []
        for dll in self.input["dlls"]:
            self.writer.comment(f"DLL {dll['number']}")
            obj_dir = f"build/{dll['dir']}"

            # Compile DLL sources
            dll_link_deps: "list[str]" = []
            for file in dll["files"]:
                # Determine command
                type = file["type"]
                command: str
                if type == BuildFileType.C:
                    command = "cc_dll"
                elif type == BuildFileType.ASM:
                    command = "as"
                elif type == BuildFileType.BIN:
                    command = "ld_bin"
                else:
                    raise NotImplementedError()
                
                # Write command
                self.writer.build(file["obj_path"], command, file["src_path"])
                dll_link_deps.append(file["obj_path"])
            
            # Link
            elf_path = f"{obj_dir}/{dll['number']}.elf"
            self.writer.build(elf_path, "ld_dll", dll_link_deps)

            # TODO: remove?
            # Copy .elf to .bin
            self.writer.build(f"{obj_dir}/{dll['number']}.bin", "binary_copy", elf_path)

            # Convert ELF to Dinosaur Planet DLL
            dll_asset_path = f"build/bin/assets/dlls/{dll['number']}.dll"
            self.writer.build(dll_asset_path, "elf2dll", elf_path)
            pack_deps.append(dll_asset_path)
        
        any_dlls = len(pack_deps) > 0

        # Leftovers
        if any_dlls:
            self.writer.comment("Leftover DLLs that haven't been decompiled yet")
            for dll in self.input["leftover_dlls"]:
                self.writer.build(dll["obj_path"], "file_copy", dll["src_path"])

        self.writer.newline()
        self.writer.comment("DLL packing")

        # Pack DLLs
        if any_dlls:
            self.writer.build("build/bin/assets/DLLS.bin", "pack_dlls", pack_deps)
            self.writer.build("build/bin/assets/DLLS.o", "ld_bin", "build/bin/assets/DLLS.bin")
            self.link_deps.append("build/bin/assets/DLLS.o")
        else:
            self.writer.comment("WARN: No DLLs to compile, using original DLLS.bin")
            self.writer.build("build/bin/assets/DLLS.o", "ld_bin", "bin/assets/DLLS.bin")
            self.link_deps.append("build/bin/assets/DLLS.o")

        self.writer.newline()

    def __write_linking(self):
        self.writer.comment("Linking")

        # Preprocess linker script
        self.writer.build("build/dino.ld", "preprocess_linker_script", "dino.ld")

        # Link
        self.link_deps.append("build/dino.ld")
        self.writer.build("build/dino.elf", "ld", self.link_deps)

        # TODO: whats with the copies?
        # Copy .elf to .bin
        self.writer.build("build/dino.bin", "binary_copy", "build/dino.elf")

        # Copy .bin to .z64
        self.writer.build("build/dino.z64", "file_copy", "build/dino.bin")
        
class InputScanner:
    def scan(self) -> BuildFiles:
        self.files: "list[BuildFile]" = []
        self.dlls: "list[DLL]" = []
        self.leftover_dlls: "list[BuildFile]" = []

        self.__scan_c_files()
        self.__scan_asm_files()
        self.__scan_bin_files()
        self.__scan_dlls()

        return { "files": self.files, "dlls": self.dlls, "leftover_dlls": self.leftover_dlls }
        

    def __scan_c_files(self):
        # Exclude DLLs here, that's done separately
        paths = [Path(path) for path in glob.glob("src/**/*.c", recursive=True) if not path.startswith("src/dlls")]
        for src_path in paths:
            obj_path = self.__make_obj_path(src_path)
            opt = self.__get_optimization_level(src_path)
            self.files.append(self.__new_build_file(str(src_path), obj_path, BuildFileType.C, opt))

    def __scan_asm_files(self):
        # Exclude splat nonmatchings, those are compiled in with their respective C file
        paths = [Path(path) for path in glob.glob("asm/**/*.s", recursive=True) if not path.startswith("asm/nonmatchings")]
        for src_path in paths:
            obj_path = self.__make_obj_path(src_path)
            self.files.append(self.__new_build_file(str(src_path), obj_path, BuildFileType.ASM))

    def __scan_bin_files(self):
        # Exclude DLLS.bin, we will be building our own
        paths = [Path(path) for path in glob.glob("bin/**/*.bin", recursive=True) if not path.endswith("DLLS.bin")]
        for src_path in paths:
            obj_path = self.__make_obj_path(src_path)
            self.files.append(self.__new_build_file(str(src_path), obj_path, BuildFileType.BIN))

    def __scan_dlls(self):
        # Scan DLLs separately since we need to build them as their own thing
        dll_dirs = [dir for dir in glob.glob("src/dlls/*") if isdir(dir)]
        to_compile: "set[str]" = set()
        for dir in dll_dirs:
            dir_parts = dir.split("/")
            number = dir_parts[-1]
            c_paths = [Path(path) for path in glob.glob(f"{dir}/**/*.c", recursive=True)]
            asm_paths = [Path(path) for path in glob.glob(f"{dir}/**/*.s", recursive=True)]
            files: "list[BuildFile]" = []

            for src_path in c_paths:
                obj_path = self.__make_obj_path(src_path)
                opt = self.__get_optimization_level(src_path)
                files.append(self.__new_build_file(str(src_path), obj_path, BuildFileType.C, opt))
            
            for src_path in asm_paths:
                obj_path = self.__make_obj_path(src_path)
                files.append(self.__new_build_file(str(src_path), obj_path, BuildFileType.ASM))
            
            self.dlls.append({ "number": number, "dir": dir, "files": files })
            to_compile.add(number)
        
        # Scan for leftover DLLs that haven't been decompiled yet
        paths = [Path(path) for path in glob.glob("bin/assets/dlls/*.dll", recursive=True)]
        for src_path in paths:
            number = src_path.name.split(".")[0]
            if number in to_compile:
                continue

            obj_path = f"build/{src_path.with_suffix('.dll')}"
            self.leftover_dlls.append(self.__new_build_file(str(src_path), obj_path, BuildFileType.BIN))


    def __new_build_file(self, src_path: str, obj_path: str, type: BuildFileType, opt: "OptimizationLevel | None"=None) -> BuildFile:
        return {
            "src_path": src_path,
            "obj_path": obj_path,
            "type": type,
            "opt": opt
        }

    def __make_obj_path(self, path: Path) -> str:
        return f"build/{path.with_suffix('.o')}"
    
    def __get_optimization_level(self, path: Path) -> OptimizationLevel:
        if len(path.parts) < 2:
            return OptimizationLevel.O2
        
        direct_parent = path.parts[-2]
        if direct_parent == "O1":
            return OptimizationLevel.O1
        elif direct_parent == "g0":
            return OptimizationLevel.O2g0
        else:
            return OptimizationLevel.O2


    
def main():
    # Create ninja build file
    ninja_file = open("build.ninja", "w")

    # Gather input files
    scanner = InputScanner()
    input = scanner.scan()

    # Write
    writer = BuildNinjaWriter(ninja_syntax.Writer(ninja_file), input)
    writer.write()

if __name__ == "__main__":
    main()
