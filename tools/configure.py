#!/usr/bin/env python3
# Builds the build.ninja script for the Dinosaur Planet decomp

import argparse
from enum import Enum
from genericpath import isdir
import glob
import json
import os
from pathlib import Path
import re
from shutil import which
import sys
from typing import TextIO
import ninja_syntax

from dino.dll_build_config import DLLBuildConfig
from dino.dlls_txt import DLLsTxt

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))

opt_flags_pattern = re.compile(r"@DECOMP_OPT_FLAGS\s*=\s*(.*)\s*")
ido_version_pattern = re.compile(r"@DECOMP_IDO_VERSION\s*=\s*(.*)\s*")
mips_iset_pattern = re.compile(r"@DECOMP_MIPS_ISET\s*=\s*(.*)\s*")

IDO_VAR_MAP = {
    "5.3": "$IDO_53",
    "7.1": "$IDO_71"
}

class BuildFileConfig:
    def __init__(self, opt: "str | None", ido_version: "str | None", mips_iset: "str | None", has_global_asm: bool):
        self.opt = opt
        self.ido_version = ido_version
        self.mips_iset = mips_iset
        self.has_global_asm = has_global_asm

class BuildFileType(Enum):
    C = 1
    ASM = 2
    BIN = 3

class BuildFile:
    def __init__(self, src_path: str, obj_path: str, type: BuildFileType, config: BuildFileConfig | None=None):
        self.src_path = src_path
        self.obj_path = obj_path
        self.type = type
        self.config = config

class DLL:
    def __init__(self, number: str, dir: str, files: "list[BuildFile]"):
        self.number = number
        self.dir = dir
        self.files = files

class BuildFiles:
    def __init__(self, files: "list[BuildFile]", dlls: "list[DLL]", leftover_dlls: "list[BuildFile]"):
        self.files = files # excludes DLLs
        self.dlls = dlls
        self.leftover_dlls = leftover_dlls # Uncompiled DLLs

class BuildConfig:
    def __init__(self,
                 target: str,
                 build_dir="build",
                 expected_build_dir="expected/build",
                 link_script: "str | None"=None,
                 link_script_dll="src/dlls/dll.ld",
                 skip_dlls=False,
                 non_matching=False,
                 non_equivalent=False,
                 default_opt_flags="-O2 -g3"):
        self.target = target
        self.build_dir = build_dir
        self.expected_build_dir = expected_build_dir
        self.link_script = link_script or f"{target}.ld"
        self.link_script_dll = link_script_dll
        self.skip_dlls = skip_dlls
        self.non_matching = non_matching
        self.non_equivalent = non_equivalent
        self.default_opt_flags = default_opt_flags

class BuildNinjaWriter:
    def __init__(self, writer: ninja_syntax.Writer, input: BuildFiles, config: BuildConfig):
        self.writer = writer
        self.input = input
        self.config = config
        self.link_deps: "list[str]" = [
            "undefined_syms.txt", 
            "undefined_funcs.txt", 
            "undefined_syms_auto.txt", 
        ]

    def write(self):
        # Write prelude (variables, rules)
        self.__write_prelude()

        # Write builds for source compilation
        self.__write_file_builds()
        
        # Write DLL builds/linking
        self.__write_dll_builds()

        # Write main linker step
        self.__write_linking()

        # Write default target
        self.writer.default(["$BUILD_DIR/$TARGET.z64"])
    
    def close(self):
        self.writer.close()

    def __write_prelude(self):
        # Write variables
        self.writer.comment("Variables")

        self.writer.variable("BUILD_DIR", self.config.build_dir)
        self.writer.variable("TARGET", self.config.target)
        self.writer.variable("LINK_SCRIPT", self.config.link_script)
        self.writer.variable("LINK_SCRIPT_DLL", self.config.link_script_dll)

        self.writer.variable("LD_EMULATION", "elf32btsmip")

        cc_defines = [
            "-D_LANGUAGE_C", 
            #"-D_FINALROM", 
            "-D_MIPS_SZLONG=32",
            "-DF3DEX_GBI_2", 
        ]

        if self.config.non_matching:
            cc_defines.append("-DNON_MATCHING")
        if self.config.non_equivalent:
            cc_defines.append("-DNON_EQUIVALENT")

        self.writer.variable("CC_DEFINES", " ".join(cc_defines))

        self.writer.variable("INCLUDES", " ".join([
            "-I include",
        ]))

        self.writer.variable("OPT_FLAGS", self.config.default_opt_flags)
        self.writer.variable("MIPS_ISET", "-mips2")
        
        cc_ignore_warnings = [
            "838", # Microsoft extension
            "649", # Missing member name in structure / union
        ]

        cc_flags = [
            "$CC_DEFINES",
            "$INCLUDES",
            "-G 0",
            "-non_shared",
            "-Xfullwarn",
            "-Xcpluscomm",
            "-Wab,-r4300_mul",
            "-woff", ",".join(cc_ignore_warnings),
        ]

        self.writer.variable("CC_FLAGS", " ".join(cc_flags))

        cc_flags_O3 = cc_flags.copy()
        cc_flags_O3.remove("-Xfullwarn") # -O3 compilation doesn't support -Xfullwarn
        self.writer.variable("CC_FLAGS_O3", " ".join(cc_flags_O3))

        self.writer.variable("CC_FLAGS_DLL", " ".join([
            "$CC_DEFINES",
            "$INCLUDES",
            "-KPIC",
            "-Xfullwarn",
            "-Xcpluscomm",
            "-Wab,-r4300_mul",
            "-woff", ",".join(cc_ignore_warnings),
        ]))

        self.writer.variable("AS_FLAGS", " ".join([
            "$INCLUDES",
            "-EB",
            "-mtune=vr4300",
            "-march=vr4300",
            "-modd-spreg",
            "-mips3",
            "-mabi=32",
        ]))

        self.writer.variable("AS_FLAGS_DLL", " ".join([
            "$INCLUDES",
            "-EB",
            "-mtune=vr4300",
            "-march=vr4300",
            "-modd-spreg",
            "-KPIC",
        ]))

        self.writer.variable("LD_FLAGS", " ".join([
            "-T undefined_syms.txt", 
            "-T undefined_funcs.txt", 
            "-T undefined_syms_auto.txt", 
            "-T $LINK_SCRIPT",
            "-Map $BUILD_DIR/$TARGET.map",
            "--no-check-sections",
            "-m $LD_EMULATION",
        ]))

        self.writer.variable("LD_FLAGS_DLL", " ".join([
            "-T export_symbol_addrs.txt", 
            "-r",
            "--emit-relocs",
            "-m $LD_EMULATION",
        ]))

        self.writer.newline()

        # Write tools
        cross = self.__detect_cross()
        exe_suffix = ".exe" if sys.platform == "win32" else ""

        self.writer.comment("Tools")
        self.writer.variable("AS", f"{cross}as{exe_suffix}")
        self.writer.variable("LD", f"{cross}ld{exe_suffix}")
        self.writer.variable("OBJCOPY", f"{cross}objcopy{exe_suffix}")
        self.writer.variable("IDO_53", f"tools/ido_static_recomp/5.3/cc{exe_suffix}")
        self.writer.variable("IDO_71", f"tools/ido_static_recomp/7.1/cc{exe_suffix}")
        self.writer.variable("CC", "$IDO_53")
        self.writer.variable("ASM_PROCESSOR", f"{sys.executable} tools/asm_processor/build.py")
        self.writer.variable("HEADER_DEPS", f"{sys.executable} tools/header_deps.py")
        self.writer.variable("ELF2DLL", f"{sys.executable} tools/elf2dll.py")
        self.writer.variable("DINODLL", f"{sys.executable} tools/dino_dll.py")
        self.writer.variable("DLL_ASMPROC_FIXUP", f"{sys.executable} tools/dll_asmproc_fixup.py")
        self.writer.variable("DLLSYMS2LD", f"{sys.executable} tools/dllsyms2ld.py")
        
        self.writer.newline()

        # Write rules
        self.writer.comment("Rules")
        self.writer.rule("cc", 
            "$HEADER_DEPS $ASM_PROCESSOR $CC -- $AS $AS_FLAGS -- -c $CC_FLAGS $OPT_FLAGS $MIPS_ISET -o $out $in", 
            "Compiling $in...",
            depfile="$out.d")
        self.writer.rule("cc_noasmproc", 
            "$HEADER_DEPS $CC -c $CC_FLAGS $OPT_FLAGS $MIPS_ISET -o $out $in", 
            "Compiling $in...",
            depfile="$out.d")
        self.writer.rule("cc_dll", 
            "$HEADER_DEPS $DLL_ASMPROC_FIXUP $EXPORTS_S $ASM_PROCESSOR $CC -- $AS $AS_FLAGS_DLL -- -c $CC_FLAGS_DLL $OPT_FLAGS $MIPS_ISET -o $out $in", 
            "Compiling $in...",
            depfile="$out.d")
        self.writer.rule("cc_dll_noasmproc", 
            "$HEADER_DEPS $CC -c $CC_FLAGS_DLL $OPT_FLAGS $MIPS_ISET -o $out $in", 
            "Compiling $in...",
            depfile="$out.d")
        self.writer.rule("as", "$AS $AS_FLAGS -o $out $in", "Assembling $in...")
        self.writer.rule("as_dll", "$AS $AS_FLAGS_DLL -o $out $in", "Assembling $in...")
        self.writer.rule("ld", "$LD $LD_FLAGS -o $out", "Linking...")
        self.writer.rule("ld_dll", "$LD $LD_FLAGS_DLL -T $SYMS_LD -T $LINK_SCRIPT_DLL $in -o $out", "Linking DLL...")
        self.writer.rule("ld_bin", "$LD -m $LD_EMULATION -r -b binary -o $out $in", "Linking binary $in...")
        self.writer.rule("to_bin", "$OBJCOPY $in $out -O binary", "Converting $in to $out...")
        self.writer.rule("file_copy", "cp $in $out", "Copying $in to $out...")
        self.writer.rule("dllsyms2ld", "$DLLSYMS2LD -o $out $in", "Converting $in to $out...")
        if sys.platform == "win32":
            self.writer.rule("elf2dll", "$ELF2DLL -o $out -b $DLL_BSS_TXT $in", "Converting $in to DP DLL $out...")
        else:
            # Note: for elf2dll, remove output file first since it might be a symlink and we don't want 
            # to write through the link to the original DLL (not relevant on Windows, we don't symlink there)
            self.writer.rule("elf2dll", f"rm -f $out && $ELF2DLL -o $out -b $DLL_BSS_TXT $in", "Converting $in to DP DLL $out...")
        self.writer.rule("pack_dlls", "$DINODLL pack $DLLS_DIR $DLLS_BIN_OUT $DLLS_TAB_IN -q --tab_out $DLLS_TAB_OUT", "Packing DLLs...")
        if sys.platform != "win32":
            self.writer.rule("sym_link", f"ln -s -f -r $in $out", "Symbolic linking $in to $out...")

        self.writer.newline()

    def __write_file_builds(self):
        self.writer.comment("Source compilation")

        for file in self.input.files:
            # Determine variables
            variables: dict[str, str] = self.__file_config_to_variables(file.config)

            # -O3 builds use a custom flags list
            if file.config != None and file.config.opt != None and "-O3" in file.config.opt:
                variables["CC_FLAGS"] = "$CC_FLAGS_O3"

            # Determine command
            command: str
            if file.type == BuildFileType.C:
                with_asmproc = file.config.has_global_asm
                if with_asmproc and file.config != None:
                    if file.config.opt != None and "-O3" in file.config.opt:
                        # Can't run asm processor with -O3 compilation
                        with_asmproc = False
                        print(f"WARN: File {file.src_path} has GLOBAL_ASM but cannot be ran through asm-processor because this file is compiled with -O3!")
                    elif file.config.opt != None and file.config.mips_iset != None and \
                         "-O0" in file.config.opt and "-mips1" in file.config.mips_iset:
                        # Can't run asm processor with -O0 -mips1
                        with_asmproc = False
                        print(f"WARN: File {file.src_path} has GLOBAL_ASM but cannot be ran through asm-processor because this file is compiled with -O0 -mips1!")

                if with_asmproc:
                    command = "cc"
                else:
                    command = "cc_noasmproc"
            elif file.type == BuildFileType.ASM:
                command = "as"
            elif file.type == BuildFileType.BIN:
                command = "ld_bin"
            else:
                raise NotImplementedError()
            
            # Write command
            obj_build_path = f"$BUILD_DIR/{Path(file.obj_path).as_posix()}"
            src_build_path = Path(file.src_path).as_posix()
            self.writer.build(obj_build_path, command, src_build_path, variables=variables)
            self.link_deps.append(obj_build_path)

        self.writer.newline()
    
    def __write_dll_builds(self):
        self.writer.comment("DLL compilation")

        pack_deps: "list[str]" = []
        for dll in self.input.dlls:
            self.writer.comment(f"DLL {dll.number}")
            obj_dir = f"$BUILD_DIR/{dll.dir}"

            # Compile DLL sources
            dll_link_deps: "list[str]" = []
            for file in dll.files:
                # Determine variables
                variables: dict[str, str] = self.__file_config_to_variables(file.config)

                # Determine command
                command: str
                if file.type == BuildFileType.C:
                    if file.config.has_global_asm:
                        command = "cc_dll"
                        variables["EXPORTS_S"] = f"{dll.dir}/exports.s" # for dll_asmproc_fixup.py
                    else:
                        command = "cc_dll_noasmproc"
                elif file.type == BuildFileType.ASM:
                    command = "as_dll"
                elif file.type == BuildFileType.BIN:
                    command = "ld_bin"
                else:
                    raise NotImplementedError()
                
                # Write command
                obj_build_path = f"$BUILD_DIR/{Path(file.obj_path).as_posix()}"
                src_build_path = Path(file.src_path).as_posix()
                self.writer.build(obj_build_path, command, src_build_path, variables=variables)
                dll_link_deps.append(obj_build_path)
            
            # Convert syms.txt to linker script
            syms_txt_path = f"{dll.dir}/syms.txt"
            syms_ld_path = f"$BUILD_DIR/{dll.dir}/syms.ld"
            self.writer.build(syms_ld_path, "dllsyms2ld", syms_txt_path)

            # Link
            elf_path = f"{obj_dir}/{dll.number}.elf"
            custom_link_script = Path(f"{dll.dir}/dll.ld")

            if custom_link_script.exists():
                # Use DLL's custom link script
                # Note: Assume custom script lists all inputs
                implicit_deps = [str(custom_link_script), syms_ld_path]
                implicit_deps.extend(dll_link_deps)
                self.writer.build(elf_path, "ld_dll", [], 
                    implicit=implicit_deps,
                    variables={
                        "SYMS_LD": syms_ld_path,
                        "LINK_SCRIPT_DLL": str(custom_link_script)
                    })
            else:
                # Use default DLL link script
                self.writer.build(elf_path, "ld_dll", dll_link_deps, 
                    implicit=["$LINK_SCRIPT_DLL", syms_ld_path, "export_symbol_addrs.txt"],
                    variables={"SYMS_LD": syms_ld_path})

            # Convert ELF to Dinosaur Planet DLL
            dll_asset_path = f"$BUILD_DIR/bin/assets/dlls/{dll.number}.dll"
            dll_bss_asset_path = f"$BUILD_DIR/bin/assets/dlls/{dll.number}.dll.bss.txt"
            self.writer.build(dll_asset_path, "elf2dll", elf_path,
                variables={
                    "DLL_BSS_TXT": dll_bss_asset_path
                },
                implicit_outputs=[dll_bss_asset_path])
            pack_deps.append(dll_asset_path)
            pack_deps.append(dll_bss_asset_path)
        
        any_dlls = len(pack_deps) > 0

        # Leftovers
        if any_dlls:
            self.writer.comment("Leftover DLLs that haven't been decompiled yet")
            for dll in self.input.leftover_dlls:
                # Note: Don't do sym linking on Windows
                dll_obj_build_path = f"$BUILD_DIR/{dll.obj_path}"
                self.writer.build(dll_obj_build_path, "file_copy" if sys.platform == "win32" else "sym_link", dll.src_path)
                pack_deps.append(dll_obj_build_path)

        self.writer.newline()
        self.writer.comment("DLL packing")

        # Pack DLLs
        if any_dlls:
            # Pack
            self.writer.build(
                outputs=["$BUILD_DIR/bin/assets/DLLS.bin", "$BUILD_DIR/bin/assets/DLLS_tab.bin"], 
                rule="pack_dlls", 
                inputs=pack_deps, 
                variables={
                    "DLLS_DIR": "$BUILD_DIR/bin/assets/dlls",
                    "DLLS_BIN_OUT": "$BUILD_DIR/bin/assets/DLLS.bin",
                    "DLLS_TAB_IN": "bin/assets/DLLS_tab.bin",
                    "DLLS_TAB_OUT": "$BUILD_DIR/bin/assets/DLLS_tab.bin"
                })
            # Link
            self.writer.build("$BUILD_DIR/bin/assets/DLLS.o", "ld_bin", "$BUILD_DIR/bin/assets/DLLS.bin")
            self.link_deps.append("$BUILD_DIR/bin/assets/DLLS.o")
            self.writer.build("$BUILD_DIR/bin/assets/DLLS_tab.o", "ld_bin", "$BUILD_DIR/bin/assets/DLLS_tab.bin")
            self.link_deps.append("$BUILD_DIR/bin/assets/DLLS_tab.o")
        else:
            # Nothing to pack, just link original DLLs.bin
            self.writer.comment("WARN: No DLLs to compile, using original DLLS.bin")
            self.writer.build("$BUILD_DIR/bin/assets/DLLS.o", "ld_bin", "bin/assets/DLLS.bin")
            self.link_deps.append("$BUILD_DIR/bin/assets/DLLS.o")
            self.writer.build("$BUILD_DIR/bin/assets/DLLS_tab.o", "ld_bin", "bin/assets/DLLS_tab.bin")
            self.link_deps.append("$BUILD_DIR/bin/assets/DLLS_tab.o")

        self.writer.newline()

    def __write_linking(self):
        self.writer.comment("Linking")

        # Link
        self.link_deps.append("$LINK_SCRIPT")
        self.writer.build("$BUILD_DIR/$TARGET.elf", "ld", [], implicit=self.link_deps)

        # Convert .elf to .z64
        self.writer.build("$BUILD_DIR/$TARGET.z64", "to_bin", "$BUILD_DIR/$TARGET.elf")
        
    def __detect_cross(self) -> str:
        # Ordered by preference
        prefixes = [
            "mips-linux-gnu-",
            "mips64-linux-gnu-",
            "mips64-elf-"
        ]

        for prefix in prefixes:
            if which(f"{prefix}ld") is not None:
                return prefix
  
        tried = ", ".join([f"{prefix}ld" for prefix in prefixes])
        print(f"Could not find MIPS binutils. Please make sure you have an appropriate version installed. Searched for: {tried}")
        sys.exit(1)

    def __file_config_to_variables(self, file_config: BuildFileConfig | None):
        variables: dict[str, str] = {}
        
        if file_config != None:
            if file_config.opt != None:
                variables["OPT_FLAGS"] = file_config.opt
            if file_config.ido_version != None:
                variables["CC"] = IDO_VAR_MAP[file_config.ido_version]
            if file_config.mips_iset != None:
                variables["MIPS_ISET"] = file_config.mips_iset
        
        return variables

class ObjDiffConfigWriter:
    def __init__(self, output_file: TextIO, input: BuildFiles, config: BuildConfig):
        self.output_file = output_file
        self.input = input
        self.config = config
    
    def write(self):
        config = {}

        config["custom_make"] = "ninja"
        config["build_target"] = False # We don't have build commands for the expected directory
        config["build_base"] = True
        config["progress_categories"] = [
            { "id": "core", "name": "Core Code" },
            { "id": "dll", "name": "DLL Code" },
        ]

        units = []
        config["units"] = units

        for file in self.input.files:
            if file.type == BuildFileType.C:
                units.append({
                    "name": file.obj_path,
                    "target_path": f"{self.config.expected_build_dir}/{file.obj_path}",
                    "base_path": f"{self.config.build_dir}/{file.obj_path}",
                    "metadata": {
                        "source_path": file.src_path,
                        "progress_categories": ["core"]
                    }
                })
        
        for dll in self.input.dlls:
            for file in dll.files:
                if file.type == BuildFileType.C:
                    units.append({
                        "name": file.obj_path,
                        "target_path": f"{self.config.expected_build_dir}/{file.obj_path}",
                        "base_path": f"{self.config.build_dir}/{file.obj_path}",
                        "metadata": {
                            "source_path": file.src_path,
                            "progress_categories": ["dll"]
                        }
                    })
        
        json.dump(config, self.output_file, indent=2)

class InputScanner:
    def __init__(self, config: BuildConfig):
        self.config = config

    def scan(self) -> BuildFiles:
        self.files: "list[BuildFile]" = []
        self.dlls: "list[DLL]" = []
        self.leftover_dlls: "list[BuildFile]" = []

        self.__scan_c_files()
        self.__scan_asm_files()
        self.__scan_bin_files()

        if not self.config.skip_dlls:
            self.__scan_dlls()

        return BuildFiles(self.files, self.dlls, self.leftover_dlls)
        
    def __scan_c_files(self):
        # Exclude DLLs here, that's done separately
        paths = [Path(path) for path in glob.glob("src/**/*.c", recursive=True) if not Path(path).is_relative_to(Path("src/dlls"))]
        for src_path in paths:
            obj_path = self.__make_obj_path(src_path)
            file_config = self.__get_file_config(src_path)
            self.files.append(BuildFile(str(src_path), str(obj_path), BuildFileType.C, file_config))

    def __scan_asm_files(self):
        # Exclude splat nonmatchings, those are compiled in with their respective C file
        paths = [Path(path) for path in glob.glob("asm/**/*.s", recursive=True) if not Path(path).is_relative_to(Path("asm/nonmatchings")) and not Path(path).is_relative_to(Path("asm/matchings"))]
        for src_path in paths:
            obj_path = self.__make_obj_path(src_path)
            self.files.append(BuildFile(str(src_path), str(obj_path), BuildFileType.ASM))

    def __scan_bin_files(self):
        # Exclude DLLS.bin and DLLS_tab.bin, we will be handling those uniquely
        paths = [Path(path) for path in glob.glob("bin/**/*.bin", recursive=True)]
        for src_path in paths:
            if src_path.name == "DLLS.bin" or src_path.name == "DLLS_tab.bin":
                continue
            obj_path = self.__make_obj_path(src_path)
            self.files.append(BuildFile(str(src_path), str(obj_path), BuildFileType.BIN))

    def __scan_dlls(self):
        # Scan DLLs separately since we need to build them as their own thing
        src_dlls_path = Path("src/dlls")

        dlls_txt_path = src_dlls_path.joinpath("dlls.txt")
        assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
        
        with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
            dlls_txt = DLLsTxt.parse(dlls_txt_file)
        
        dll_dirs = [(n, src_dlls_path.joinpath(path)) for (n, path) in dlls_txt.path_map.items()]

        to_compile: "set[str]" = set()
        for (number, dir) in dll_dirs:
            dll_config = self.__get_dll_config(dir, number)
            if dll_config == None:
                continue

            # Skip if this DLL is configured to use the original DLL instead of recompiling
            if not dll_config.compile:
                continue

            c_paths = [Path(path) for path in glob.glob(f"{dir}/**/*.c", recursive=True)]
            asm_paths = [Path(path) for path in glob.glob(f"{dir}/**/*.s", recursive=True)]

            files: "list[BuildFile]" = []

            for src_path in c_paths:
                obj_path = self.__make_obj_path(src_path)
                file_config = self.__get_file_config(src_path)
                files.append(BuildFile(str(src_path), str(obj_path), BuildFileType.C, file_config))
            
            for src_path in asm_paths:
                obj_path = self.__make_obj_path(src_path)
                files.append(BuildFile(str(src_path), str(obj_path), BuildFileType.ASM))
            
            self.dlls.append(DLL(str(number), dir, files))
            to_compile.add(str(number))
        
        # Scan for leftover DLLs that haven't been decompiled yet
        paths = [Path(path) for path in glob.glob("bin/assets/dlls/*.dll", recursive=True)]
        for src_path in paths:
            number = src_path.name.split(".")[0]
            if number in to_compile:
                continue

            obj_path = src_path.with_suffix('.dll')
            self.leftover_dlls.append(BuildFile(str(src_path), str(obj_path), BuildFileType.BIN))

    def __make_obj_path(self, path: Path) -> str:
        return path.with_suffix('.o')
    
    def __get_file_config(self, path: Path) -> BuildFileConfig:
        opt_flags: "str | None" = None
        ido_version: "str | None" = None
        mips_iset: "str | None" = None
        has_global_asm: bool = False

        with open(path, "r", encoding="utf-8") as file:
            while True:
                line = file.readline()
                if line == "":
                    break # EOF
                line = line.strip()
                if line.startswith("//"):
                    opt_match = opt_flags_pattern.search(line)
                    if opt_match != None:
                        opt_flags = opt_match.group(1)
                        continue
                    ido_match = ido_version_pattern.search(line)
                    if ido_match != None:
                        ido_version = ido_match.group(1)
                        continue
                    mips_iset_match = mips_iset_pattern.search(line)
                    if mips_iset_match != None:
                        mips_iset = mips_iset_match.group(1)
                        continue
                elif line.startswith("#pragma GLOBAL_ASM"):
                    has_global_asm = True
        
        return BuildFileConfig(
            opt=opt_flags, 
            ido_version=ido_version,
            mips_iset=mips_iset,
            has_global_asm=has_global_asm)
    
    def __get_dll_config(self, dll_dir: Path, number: int) -> DLLBuildConfig | None:
        yaml_path = dll_dir.joinpath("dll.yaml")
        if not yaml_path.exists():
            print(f"WARN: Missing {yaml_path}, skipping DLL {number}!")
            return None
        
        with open(yaml_path, "r") as file:
            return DLLBuildConfig.parse(file)
    
def main():
    parser = argparse.ArgumentParser(description="Creates the Ninja build script for the Dinosaur Planet decompilation project.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("--target", type=str, help="The filename of the ROM to create (excluding extension suffix, default=dino).", default="dino")
    parser.add_argument("--skip-dlls", dest="skip_dlls", action="store_true", help="Don't recompile DLLs (use original).")
    parser.add_argument("--non-matching", dest="non_matching", action="store_true", help="Define NON_MATCHING.")
    parser.add_argument("--non-equivalent", dest="non_equivalent", action="store_true", help="Define NON_EQUIVALENT.")
    
    args = parser.parse_args()

    # Create config
    config = BuildConfig(
        target=args.target,
        skip_dlls=args.skip_dlls or False,
        non_matching=args.non_matching or False,
        non_equivalent=args.non_equivalent or False
    )

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    # Verify this is the correct directory
    expected_link_script_path = Path.cwd().joinpath(config.link_script).absolute()
    if not expected_link_script_path.exists():
        print(f"Could not find linker script at {expected_link_script_path}. Is the base directory '{args.base_dir}' correct?")
        sys.exit(1)

    # Gather input files
    scanner = InputScanner(config)
    input = scanner.scan()

    # Create ninja build file
    ninja_file = open("build.ninja", "w")

    # Write
    writer = BuildNinjaWriter(ninja_syntax.Writer(ninja_file), input, config)
    writer.write()
    writer.close()

    # Write config for objdiff
    with open("objdiff.json", "w") as objdiff_config_file:
        objdiff_config_writer = ObjDiffConfigWriter(objdiff_config_file, input, config)
        objdiff_config_writer.write()

if __name__ == "__main__":
    main()