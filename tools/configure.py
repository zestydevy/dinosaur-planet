#!/usr/bin/env python3
# Builds the build.ninja script for the Dinosaur Planet decomp

import argparse
from enum import Enum
from genericpath import isdir
import glob
import os
from pathlib import Path
from shutil import which
import sys
from ninja import ninja_syntax
import yaml

class BuildFileType(Enum):
    C = 1
    ASM = 2
    BIN = 3

class OptimizationFlags(Enum):
    O2g3 = "-O2 -g3"
    O2g0 = "-O2 -g0"
    O1 = "-O1" 

class BuildFile:
    def __init__(self, src_path: str, obj_path: str, type: BuildFileType, opt: "OptimizationFlags | None"=None):
        self.src_path = src_path
        self.obj_path = obj_path
        self.type = type
        self.opt = opt

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
                 link_script: "str | None"=None,
                 link_script_dll="src/dlls/dll.ld",
                 skip_dlls=False,
                 default_opt_flags=OptimizationFlags.O2g3):
        self.target = target
        self.build_dir = build_dir
        self.link_script = link_script or f"{target}.ld"
        self.link_script_dll = link_script_dll
        self.skip_dlls = skip_dlls
        self.default_opt_flags = default_opt_flags

class BuildNinjaWriter:
    def __init__(self, writer: ninja_syntax.Writer, input: BuildFiles, config: BuildConfig):
        self.writer = writer
        self.input = input
        self.config = config
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

        self.writer.variable("CC_DEFINES", " ".join([
            "-D_LANGUAGE_C", 
            "-D_FINALROM", 
            "-D_MIPS_SZLONG=32",
            "-DF3DEX_GBI_2", 
        ]))

        self.writer.variable("INCLUDES", " ".join([
            "-I include",
        ]))

        self.writer.variable("OPT_FLAGS", self.config.default_opt_flags.value)
        
        self.writer.variable("CC_FLAGS", " ".join([
            "$CC_DEFINES",
            "$INCLUDES",
            "-G 0",
            "-mips2",
            "-non_shared",
            "-Xfullwarn",
            "-Xcpluscomm",
            "-Wab,-r4300_mul",
        ]))

        self.writer.variable("CC_FLAGS_DLL", " ".join([
            "$CC_DEFINES",
            "$INCLUDES",
            "-mips2",
            "-KPIC",
            "-w",
            "-Xcpluscomm",
            "-Wab,-r4300_mul",
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

        self.writer.variable("GCC_FLAGS", " ".join([
            "$CC_DEFINES",
            "$INCLUDES",
            "-m32", 
            "-nostdinc", 
            "-std=gnu90",
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
            "-Wno-main",
        ]))

        self.writer.variable("GCC_FLAGS_DLL", " ".join([
            "$CC_DEFINES",
            "$INCLUDES",
            "-nostdinc",
            "-std=gnu90",
            #"-march=vr4300", # TODO: these were in the original Makefile but aren't real args for GCC
            #"-mtune=vr4300",
            #"-mfix4300",
            "-fPIC",
            "-fno-stack-protector",
            "-fno-builtin",
            "-fno-common",
            "-fsigned-char",
        ]))

        self.writer.variable("LD_FLAGS", " ".join([
            "-T undefined_syms.txt", 
            "-T undefined_funcs.txt", 
            "-T undefined_syms_auto.txt", 
            "-T $BUILD_DIR/$TARGET.ld", # pre-processed linker script
            "-Map $BUILD_DIR/$TARGET.map",
            "--no-check-sections",
        ]))

        self.writer.variable("LD_FLAGS_DLL", " ".join([
            "-nostartfiles",
            "-nodefaultlibs",
            "-r",
            "--emit-relocs",
        ]))

        self.writer.newline()

        # Write tools
        cross = self.__detect_cross()

        self.writer.comment("Tools")
        self.writer.variable("AS", f"{cross}as")
        self.writer.variable("LD", f"{cross}ld")
        self.writer.variable("OBJCOPY", f"{cross}objcopy")
        self.writer.variable("CC", "tools/ido_recomp/linux/5.3/cc")
        self.writer.variable("ASM_PROCESSOR", "python3 tools/asm_processor/build.py")
        self.writer.variable("CC_PREPROCESSED", "$ASM_PROCESSOR $CC -- $AS $AS_FLAGS --")
        self.writer.variable("CC_PREPROCESSED_DLL", "$ASM_PROCESSOR $CC -- $AS $AS_FLAGS_DLL --")
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
            "$GCC -MM -MF $out.d -MT $out $GCC_FLAGS_DLL $in && $CC_PREPROCESSED_DLL -c $CC_FLAGS_DLL $OPT_FLAGS -o $out $in", 
            "Compiling $in...",
            depfile="$out.d")
        self.writer.rule("as", "$AS $AS_FLAGS -o $out $in", "Assembling $in...")
        self.writer.rule("as_dll", "$AS $AS_FLAGS_DLL -o $out $in", "Assembling $in...")
        self.writer.rule("preprocess_linker_script", "cpp -P -DBUILD_DIR=$BUILD_DIR -o $out $in", "Pre-processing linker script...")
        self.writer.rule("ld", "$LD $LD_FLAGS -o $out", "Linking...")
        self.writer.rule("ld_dll", "$LD $LD_FLAGS_DLL -T $SYMS_TXT -T $LINK_SCRIPT_DLL $in -o $out", "Linking...")
        self.writer.rule("ld_bin", "$LD -r -b binary -o $out $in", "Linking binary $in...")
        self.writer.rule("to_bin", "$OBJCOPY $in $out -O binary", "Converting $in to $out...")
        self.writer.rule("file_copy", "cp $in $out", "Copying $in to $out...")
        # Note: for elf2dll, remove output file first since it might be a symlink and we don't
        #       want to write through the link to the original DLL
        self.writer.rule("elf2dll", "rm -f $out && $ELF2DLL $in $out", "Converting $in to DP DLL $out...")
        self.writer.rule("pack_dlls", "$DINODLL pack $DLLS_DIR $DLLS_BIN_OUT $DLLS_TAB_IN -q --tab_out $DLLS_TAB_OUT", "Packing DLLs...")
        self.writer.rule("sym_link", "ln -s -f -r $in $out", "Symbolic linking $in to $out...")

        self.writer.newline()

    def __write_file_builds(self):
        self.writer.comment("Source compilation")

        for file in self.input.files:
            # Determine variables
            variables: dict[str, str] = {}
            opt = file.opt
            if opt is not None and opt != self.config.default_opt_flags:
                variables["OPT_FLAGS"] = opt.value

            # Determine command
            command: str
            if file.type == BuildFileType.C:
                command = "cc"
            elif file.type == BuildFileType.ASM:
                command = "as"
            elif file.type == BuildFileType.BIN:
                command = "ld_bin"
            else:
                raise NotImplementedError()
            
            # Write command
            self.writer.build(file.obj_path, command, file.src_path, variables=variables)
            self.link_deps.append(file.obj_path)

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
                # Determine command
                command: str
                if file.type == BuildFileType.C:
                    command = "cc_dll"
                elif file.type == BuildFileType.ASM:
                    command = "as_dll"
                elif file.type == BuildFileType.BIN:
                    command = "ld_bin"
                else:
                    raise NotImplementedError()
                
                # Write command
                self.writer.build(file.obj_path, command, file.src_path)
                dll_link_deps.append(file.obj_path)
            
            # Link
            elf_path = f"{obj_dir}/{dll.number}.elf"
            syms_txt_path = f"{dll.dir}/syms.txt"
            custom_link_script = Path(f"{dll.dir}/{dll.number}.ld")

            if custom_link_script.exists():
                # Use DLL's custom link script
                # Note: Assume custom script lists all inputs
                implicit_deps = [str(custom_link_script), syms_txt_path]
                implicit_deps.extend(dll_link_deps)
                self.writer.build(elf_path, "ld_dll", [], 
                    implicit=implicit_deps,
                    variables={
                        "SYMS_TXT": syms_txt_path,
                        "LINK_SCRIPT_DLL": str(custom_link_script)
                    })
            else:
                # Use default DLL link script
                self.writer.build(elf_path, "ld_dll", dll_link_deps, 
                    implicit=["$LINK_SCRIPT_DLL", syms_txt_path],
                    variables={"SYMS_TXT": syms_txt_path})

            # Convert ELF to Dinosaur Planet DLL
            dll_asset_path = f"$BUILD_DIR/bin/assets/dlls/{dll.number}.dll"
            self.writer.build(dll_asset_path, "elf2dll", elf_path)
            pack_deps.append(dll_asset_path)
        
        any_dlls = len(pack_deps) > 0

        # Leftovers
        if any_dlls:
            self.writer.comment("Leftover DLLs that haven't been decompiled yet")
            for dll in self.input.leftover_dlls:
                self.writer.build(dll.obj_path, "sym_link", dll.src_path)
                pack_deps.append(dll.obj_path)

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

        # Preprocess linker script
        # TODO: it looks like splat's linker script output doesn't need preprocessing anymore
        self.writer.build("$BUILD_DIR/$TARGET.ld", "preprocess_linker_script", "$LINK_SCRIPT")

        # Link
        self.link_deps.append("$BUILD_DIR/$TARGET.ld")
        self.writer.build("$BUILD_DIR/$TARGET.elf", "ld", self.link_deps)

        # Convert .elf to .bin
        self.writer.build("$BUILD_DIR/$TARGET.bin", "to_bin", "$BUILD_DIR/$TARGET.elf")

        # Copy .bin to .z64
        # TODO: change to rename?
        self.writer.build("$BUILD_DIR/$TARGET.z64", "file_copy", "$BUILD_DIR/$TARGET.bin")
        
    def __detect_cross(self) -> str:
        if which("mips-n64-ld") is not None:
            return "mips-n64-" # N64 dev tools
        elif which("mips64-linux-gnu-ld") is not None:
            return "mips64-linux-gnu-"
        elif which("mips64-elf-ld") is not None:
            return "mips64-elf-"
        else:
            return "mips-linux-gnu-"

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
        paths = [Path(path) for path in glob.glob("src/**/*.c", recursive=True) if not path.startswith("src/dlls")]
        for src_path in paths:
            obj_path = self.__make_obj_path(src_path)
            opt = self.__get_optimization_level(src_path)
            self.files.append(BuildFile(str(src_path), obj_path, BuildFileType.C, opt))

    def __scan_asm_files(self):
        # Exclude splat nonmatchings, those are compiled in with their respective C file
        paths = [Path(path) for path in glob.glob("asm/**/*.s", recursive=True) if not path.startswith("asm/nonmatchings")]
        for src_path in paths:
            obj_path = self.__make_obj_path(src_path)
            self.files.append(BuildFile(str(src_path), obj_path, BuildFileType.ASM))

    def __scan_bin_files(self):
        # Exclude DLLS.bin and DLLS_tab.bin, we will be handling those uniquely
        paths = [Path(path) for path in glob.glob("bin/**/*.bin", recursive=True) if not path.endswith("DLLS.bin") and not path.endswith("DLLS_tab.bin")]
        for src_path in paths:
            obj_path = self.__make_obj_path(src_path)
            self.files.append(BuildFile(str(src_path), obj_path, BuildFileType.BIN))

    def __scan_dlls(self):
        # Scan DLLs separately since we need to build them as their own thing
        dll_dirs = [dir for dir in glob.glob("src/dlls/*") if isdir(dir)]
        to_compile: "set[str]" = set()
        for dir in dll_dirs:
            dir_parts = dir.split("/")
            number = dir_parts[-1]

            # Skip if this DLL is configured to use the original DLL instead of recompiling
            if not self.__should_compile_dll(Path(dir), number):
                continue

            c_paths = [Path(path) for path in glob.glob(f"{dir}/**/*.c", recursive=True)]
            asm_paths = [Path(path) for path in glob.glob(f"{dir}/**/*.s", recursive=True)]
            asm_paths.extend([Path(path) for path in glob.glob(f"asm/nonmatchings/dlls/{number}/data/*.s")])
            files: "list[BuildFile]" = []

            for src_path in c_paths:
                obj_path = self.__make_obj_path(src_path)
                opt = self.__get_optimization_level(src_path)
                files.append(BuildFile(str(src_path), obj_path, BuildFileType.C, opt))
            
            for src_path in asm_paths:
                obj_path = self.__make_obj_path(src_path)
                files.append(BuildFile(str(src_path), obj_path, BuildFileType.ASM))
            
            self.dlls.append(DLL(number, dir, files))
            to_compile.add(number)
        
        # Scan for leftover DLLs that haven't been decompiled yet
        paths = [Path(path) for path in glob.glob("bin/assets/dlls/*.dll", recursive=True)]
        for src_path in paths:
            number = src_path.name.split(".")[0]
            if number in to_compile:
                continue

            obj_path = f"$BUILD_DIR/{src_path.with_suffix('.dll')}"
            self.leftover_dlls.append(BuildFile(str(src_path), obj_path, BuildFileType.BIN))

    def __make_obj_path(self, path: Path) -> str:
        return f"$BUILD_DIR/{path.with_suffix('.o')}"
    
    def __get_optimization_level(self, path: Path) -> OptimizationFlags:
        if len(path.parts) >= 2:
            direct_parent = path.parts[-2]
            if direct_parent == "O1":
                return OptimizationFlags.O1
            elif direct_parent == "g0":
                return OptimizationFlags.O2g0
        
        return self.config.default_opt_flags
    
    def __should_compile_dll(self, dll_dir: Path, number: str) -> bool:
        yaml_path = dll_dir.joinpath(f"{number}.yaml")
        if not yaml_path.exists():
            print(f"WARN: Missing {yaml_path}!")
            return True
        
        dll_config = self.__parse_dll_yaml(yaml_path)
        return "compile" in dll_config and dll_config["compile"]
    
    def __parse_dll_yaml(self, path: Path):
        with open(path, "r") as file:
            return yaml.safe_load(file)
    
def main():
    parser = argparse.ArgumentParser(description="Creates the Ninja build script for the Dinosaur Planet decompilation project.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project (default=..).", default="..")
    parser.add_argument("--target", type=str, help="The filename of the ROM to create (excluding extension suffix, default=dino).", default="dino")
    parser.add_argument("--skip-dlls", dest="skip_dlls", action="store_true", help="Don't recompile DLLs (use original).")
    
    args = parser.parse_args()

    # Create config
    config = BuildConfig(
        target=args.target,
        skip_dlls=args.skip_dlls or False
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

if __name__ == "__main__":
    main()
