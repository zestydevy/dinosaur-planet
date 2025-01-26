#!/usr/bin/env python3

import argparse
import glob
import os
from pathlib import Path
import shutil
import subprocess
import sys

from tools.dino.dlls_txt import DLLsTxt 

TARGET = "dino"

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
ASM_PATH = SCRIPT_DIR.joinpath("asm/")
BIN_PATH = SCRIPT_DIR.joinpath("bin/")
SRC_PATH = SCRIPT_DIR.joinpath("src/")
BUILD_PATH = SCRIPT_DIR.joinpath("build/")
EXPECTED_PATH = SCRIPT_DIR.joinpath("expected/")
TOOLS_PATH = SCRIPT_DIR.joinpath("tools/")
PERMUTER_ARTIFACT_PATH = SCRIPT_DIR.joinpath("nonmatchings/")

BUILD_SCRIPT_PATH = SCRIPT_DIR.joinpath("build.ninja")

# Paths to directories/files to delete when cleaning
CLEAN_PATHS = [
    ASM_PATH,
    BIN_PATH,
    BUILD_PATH,
    BUILD_SCRIPT_PATH,
    EXPECTED_PATH,
    PERMUTER_ARTIFACT_PATH,
    SCRIPT_DIR.joinpath(".ninja_log"),
    SCRIPT_DIR.joinpath(".splat_cache"),
    SCRIPT_DIR.joinpath(f"{TARGET}.ld"),
    SCRIPT_DIR.joinpath("undefined_funcs_auto.txt"),
    SCRIPT_DIR.joinpath("undefined_syms_auto.txt"),
]

BUILD_ARTIFACTS = [
    BUILD_PATH,
    SCRIPT_DIR.joinpath(".ninja_log"),
]

SPLIT_PY = TOOLS_PATH.joinpath("splat/split.py")
DINO_DLL_PY = TOOLS_PATH.joinpath("dino_dll.py")
DLL_SPLIT_PY = TOOLS_PATH.joinpath("dll_split.py")
CONFIGURE_PY = TOOLS_PATH.joinpath("configure.py")
DIFF_PY = TOOLS_PATH.joinpath("asm_differ/diff.py")
M2CTX_PY = TOOLS_PATH.joinpath("m2ctx.py")
PERMUTER_IMPORT_PY = TOOLS_PATH.joinpath("decomp_permuter/import.py")
PERMUTER_PERMUTER_PY = TOOLS_PATH.joinpath("decomp_permuter/permuter.py")

class DinoCommandRunner:
    def __init__(self, verbose: bool) -> None:
        self.verbose = verbose

    def clean(self):
        print("Cleaning...")
        for path in CLEAN_PATHS:
            if not path.exists():
                continue
            
            if self.verbose:
                print(f"  rm {path.relative_to(SCRIPT_DIR)}")
            
            if path.is_dir():
                shutil.rmtree(path)
            else:
                path.unlink()

    def update_submodules(self):
        print("Updating Git submodules...")
        self.__run_cmd(["git", "submodule", "update", "--init", "--recursive"])

    def extract(self, use_cache: bool):
        print("Extracting...")

        # If not using cache, clear existing extracted content
        if not use_cache:
            if ASM_PATH.exists():
                if self.verbose:
                    print(f"rm {ASM_PATH}")
                shutil.rmtree(ASM_PATH)
            if BIN_PATH.exists():
                if self.verbose:
                    print(f"rm {BIN_PATH}")
                shutil.rmtree(BIN_PATH)

        # Run splat
        args = [
            "python3", str(SPLIT_PY), 
            "--target", "baserom.z64", 
            "--basedir", str(SCRIPT_DIR),
        ]

        if self.verbose:
            args.append("--verbose")
        if use_cache:
            args.append("--use-cache")
        
        args.append("splat.yaml")
        self.__run_cmd(args)
        
        # Unpack DLLs
        print()
        print("Unpacking DLLs...")
        self.__run_cmd([
            "python3", str(DINO_DLL_PY), 
            "unpack",
            str(BIN_PATH.joinpath("assets/dlls")),
            str(BIN_PATH.joinpath("assets/DLLS.bin")),
            str(BIN_PATH.joinpath("assets/DLLS_tab.bin"))
        ])

        # Extract DLLs
        print()
        print("Extracting DLLs...")
        self.__extract_dlls()

        print()
        self.configure()

    def extract_dll(self, number: int):
        print(f"Extracting DLL {number}...")

        dlls_txt_path = SRC_PATH.joinpath("dlls/dlls.txt")
        assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
        
        with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
            dlls_txt = DLLsTxt.parse(dlls_txt_file)

        # Clear extracted asm for DLL
        dll_dir = dlls_txt.path_map.get(number, None)
        if dll_dir != None:
            asm_dir = ASM_PATH.joinpath(f"nonmatchings/dlls/{dll_dir}")
            if asm_dir.exists():
                if self.verbose:
                    print(f"rm {asm_dir}")
                shutil.rmtree(asm_dir)
        
        # Extract DLL
        self.__extract_dlls([number])

        print()
        self.configure()

    def configure(self):
        print("Configuring build script...")

        self.__assert_project_built()

        self.__run_cmd([
            "python3", str(CONFIGURE_PY), 
            "--base-dir", str(SCRIPT_DIR),
            "--target", TARGET
        ])

    def build(self, configure: bool, force: bool, skip_expected: bool, target: "str | None"):
        # Configure build script if it's missing
        if configure or not BUILD_SCRIPT_PATH.exists():
            self.configure()
            print()
        
        # If force is given, delete build artifacts first
        if force:
            for path in BUILD_ARTIFACTS:
                if not path.exists():
                    continue

                if self.verbose:
                    print(f"rm {path}")
                
                if path.is_dir():
                    shutil.rmtree(path)
                else:
                    path.unlink()

        # Build
        if target is None:
            print("Building ROM...")
        else:
            print(f"Building {target}...")
        
        args = ["ninja"]
        if SCRIPT_DIR != Path.cwd():
            args.extend(["-C", str(SCRIPT_DIR)])
        if self.verbose:
            args.append("-v")
        
        if target is not None:
            args.append(target)
        
        self.__run_cmd(args)

        # Stop here if the full ROM isn't being built
        if target is not None and target != str(BUILD_PATH.joinpath(f"{TARGET}.z64").relative_to(SCRIPT_DIR)):
            return
        
        # Verify
        print()
        self.verify()

        if not skip_expected:
            # If matching, update the 'expected' directory for diff
            self.create_expected_dir(already_verified=True, quiet=True)

    def create_expected_dir(self, already_verified=False, force=False, quiet=False):
        # Ensure the build matches
        if not already_verified:
            try:
                self.verify()

                if not quiet:
                    print()
            except subprocess.CalledProcessError:
                print()
                print("The 'expected' output directory can only be created from a matching build!")
                sys.exit(1)
        
        # If force is given, remove any existing files
        if force:
            if self.verbose:
                print(f"rm {EXPECTED_PATH}")
            shutil.rmtree(EXPECTED_PATH)

        # Determine which files need to be copied
        base_path = BUILD_PATH.relative_to(SCRIPT_DIR)
        obj_paths = [Path(p) for p in glob.glob(f"{base_path}/**/*.o", recursive=True)]

        to_create: "list[tuple[Path, Path]]" = []
        for in_path in obj_paths:
            out_path = EXPECTED_PATH.joinpath(in_path)
            if not os.path.exists(out_path) or os.path.getmtime(in_path) > os.path.getmtime(out_path):
                to_create.append((in_path, out_path))

        if len(to_create) == 0:
            # Nothing to do
            if not quiet:
                print("The 'expected' output directory is already up to date.")
            return
        
        # Update directory
        if not quiet:
            print("Updating 'expected' output directory for diff...")

        dirs = {str(pair[1].parent) for pair in to_create}

        for dir in dirs:
            if self.verbose:
                print(f"mkdir {Path(dir).relative_to(SCRIPT_DIR)}")
            os.makedirs(dir, exist_ok=True)

        for pair in to_create:
            if self.verbose:
                print(f"cp {pair[0]} {Path(pair[1]).relative_to(SCRIPT_DIR)}")
            shutil.copyfile(pair[0], pair[1])

    def baseverify(self):
        print("Verifying base ROM...")

        baserom_path = SCRIPT_DIR.joinpath("baserom.z64")
        if not baserom_path.exists():
            print(f"Missing base ROM! Ensure the real Dinosaur Planet ROM exists at {baserom_path.relative_to(SCRIPT_DIR)}.")
            sys.exit(1)
        
        self.__run_cmd(["md5sum", "-c", "baserom.md5"])

    def verify(self):
        print("Verifying re-built ROM...")

        rom_path = SCRIPT_DIR.joinpath(f"build/{TARGET}.z64")
        if not rom_path.exists():
            print(f"Missing re-built ROM! Ensure the project has been built and that {rom_path.relative_to(SCRIPT_DIR)} exists.")
            sys.exit(1)
        
        self.__run_cmd(["md5sum", "-c", "checksum.md5"])

    def setup(self):
        print("Welcome to the Dinosaur Planet decompilation project!")
        print()
        self.update_submodules()
        print()
        self.baseverify()
        print()
        self.extract(use_cache=False)
        print()
        print(f"Done! Run '{self.__get_invoked_as()} build' to build the ROM.")

    def setup_dll(self, number: int, dll_dir: str):
        dlls_txt_path = SRC_PATH.joinpath("dlls/dlls.txt")
        assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
        
        with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
            dlls_txt = DLLsTxt.parse(dlls_txt_file)

        # Ensure the DLL isn't already set up
        existing_dll_dir = dlls_txt.path_map.get(number, None)
        src_dir = SRC_PATH.joinpath(f"dlls/{dll_dir}")

        if existing_dll_dir == None and src_dir.exists():
            existing_dll_dir = dll_dir

        if existing_dll_dir != None:
            print(f"An environment already exists at {SRC_PATH.joinpath(f"dlls/{existing_dll_dir}").relative_to(SCRIPT_DIR)}! " +
                  f"Try '{self.__get_invoked_as()} extract-dll {number}' instead to re-extract.")
            sys.exit(1)
        
        print(f"Creating environment for DLL {number}...")

        # Add dlls.txt entry
        with open(dlls_txt_path, "w", encoding="utf-8") as dlls_txt_file:
            dlls_txt.set(number, dll_dir)
            dlls_txt.save(dlls_txt_file)
            dlls_txt_file.flush()
        
        # Create directory
        os.makedirs(src_dir)
        
        # Extract DLL
        print("Extracting DLL...")
        self.__extract_dlls([number], quiet=True)

        # Re-configure build script
        self.configure()

        # Done
        print()
        print(f"Done! Environment created at {src_dir.relative_to(SCRIPT_DIR)}.")

    def diff(self, args: "list[str]"):
        self.__assert_project_built()

        # Need to run diff from the project root where diff_settings.py is
        os.chdir(SCRIPT_DIR)

        self.__run_cmd(["python3", str(DIFF_PY)] + args)
    
    def objdiff(self, args: "list[str]"):
        self.__assert_project_built()

        self.__run_cmd(["objdiff-cli", "diff", "-p", SCRIPT_DIR] + args)
    
    def make_context(self, file: str):
        self.__run_cmd(["python3", str(M2CTX_PY), file])
        print(f"Created context file at {SCRIPT_DIR.joinpath('ctx.c')}")
    
    def permuter_import(self, args: "list[str]"):
        self.__run_cmd(["python3", str(PERMUTER_IMPORT_PY)] + args)
    
    def permuter(self, args: "list[str]"):
        self.__run_cmd(["python3", str(PERMUTER_PERMUTER_PY)] + args)
    
    def __assert_project_built(self):
        linker_script_path = SCRIPT_DIR.joinpath(f"{TARGET}.ld")
        if not linker_script_path.exists():
            print(f"Could not find linker script at {linker_script_path}. Is the ROM extracted?")
            sys.exit(1)

    def __run_cmd(self, args: "str | list[str]"):
        if self.verbose:
            print(">", " ".join(args))
        subprocess.check_call(args)

    def __extract_dlls(self, dlls: "list[str | int]"=[], quiet: bool=False):
        args = [
            "python3", str(DLL_SPLIT_PY),
            "--base-dir", str(SCRIPT_DIR),
        ]

        if self.verbose:
            args.append("--verbose")
        if not self.verbose and quiet:
            args.append("--quiet")

        args.extend([str(dll) for dll in dlls])

        self.__run_cmd(args)
    
    def __get_invoked_as(self):
        invoked_as = sys.argv[0]
        if not invoked_as.endswith(".py"):
            # Assume the script was invoked via something like a path symlink,
            # just print the name instead of the potentially full path
            invoked_as = Path(invoked_as).name
        return invoked_as

def main():
    parser = argparse.ArgumentParser(description="Quick commands for working on the Dinosaur Planet decompilation.")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable debug logging.", default=False)

    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("setup", help="Initialize/update Git submodules, verify the base ROM, and extract the ROM.")

    setup_dll_cmd = subparsers.add_parser("setup-dll", help="Set up a new environment for decomping a DLL.")
    setup_dll_cmd.add_argument("number", type=int, help="The number of the DLL.")
    setup_dll_cmd.add_argument("dir", type=str, help="Directory name to set up the DLL under.")
    
    extract_cmd = subparsers.add_parser("extract", help="Split ROM and unpack DLLs.")
    extract_cmd.add_argument("--use-cache", action="store_true", dest="use_cache", help="Only split changed segments in splat config.", default=False)

    extract_dll_cmd = subparsers.add_parser("extract-dll", help="Split and extract DLL.")
    extract_dll_cmd.add_argument("number", type=int, help="The number of the DLL.")

    subparsers.add_parser("configure", help="Re-configure the build script.")
    
    build_cmd = subparsers.add_parser("build", help="Build ROM and verify that it matches.")
    build_cmd.add_argument("-c", "--configure", action="store_true", help="Re-configure the build script before building.", default=False)
    build_cmd.add_argument("-f", "--force", action="store_true", help="Force a full rebuild.", default=False)
    build_cmd.add_argument("--no-expected", dest="skip_expected", action="store_true", help="Don't update the 'expected' directory after a matching build.", default=False)
    build_cmd.add_argument("target", nargs="?", help="The target to build. Don\'t specify to build the full ROM.")

    build_exp_cmd = subparsers.add_parser("build-expected", help="Update the 'expected' directory for diff. Requires a verified build.")
    build_exp_cmd.add_argument("-f", "--force", action="store_true", help="Fully recreate the directory instead of updating it.", default=False)
    
    subparsers.add_parser("verify", help="Verify that the re-built ROM matches the base ROM.")
    subparsers.add_parser("baseverify", help="Verify that the base ROM is correct.")
    subparsers.add_parser("clean", help="Remove extracted files, build artifacts, and build scripts.")
    subparsers.add_parser("submodules", help="Initialize and update Git submodules.")
    subparsers.add_parser("diff", help="Diff the re-rebuilt ROM with the original (redirects to asm-differ).", add_help=False)
    subparsers.add_parser("objdiff", help="Diff the re-rebuilt ROM with the original (redirects to objdiff-cli).", add_help=False)
    subparsers.add_parser("permuter", help="Randomly permute a C file to better match a target binary (redirects to decomp-permuter permuter.py).", add_help=False)
    subparsers.add_parser("permuter-import", help="Import a function for permuter (redirects to decomp-permuter import.py).", add_help=False)
    
    ctx_cmd = subparsers.add_parser("context", help="Create a context file that can be used for mips2c/decomp.me.")
    ctx_cmd.add_argument("file", help="The C file to create context for.")

    args, _ = parser.parse_known_args()
    cmd = args.command

    try:
        runner = DinoCommandRunner(args.verbose)
        if cmd == "setup":
            runner.setup()
        elif cmd == "setup-dll":
            runner.setup_dll(number=args.number, dll_dir=args.dir)
        elif cmd == "extract":
            runner.extract(use_cache=args.use_cache)
        elif cmd == "extract-dll":
            runner.extract_dll(number=args.number)
        elif cmd == "build":
            runner.build(
                configure=args.configure, 
                force=args.force, 
                skip_expected=args.skip_expected,
                target=args.target
            )
        elif cmd == "build-expected":
            runner.create_expected_dir(force=args.force)
        elif cmd == "configure":
            runner.configure()
        elif cmd == "verify":
            runner.verify()
        elif cmd == "baseverify":
            runner.baseverify()
        elif cmd == "clean":
            runner.clean()
        elif cmd == "submodules":
            runner.update_submodules()
        elif cmd == "diff":
            diff_index = sys.argv.index("diff")
            full_args = sys.argv[diff_index + 1:]
            runner.diff(args=full_args)
        elif cmd == "objdiff":
            diff_index = sys.argv.index("objdiff")
            full_args = sys.argv[diff_index + 1:]
            runner.objdiff(args=full_args)
        elif cmd =="context":
            runner.make_context(args.file)
        elif cmd == "permuter":
            permuter_index = sys.argv.index("permuter")
            full_args = sys.argv[permuter_index + 1:]
            runner.permuter(args=full_args)
        elif cmd == "permuter-import":
            permuter_index = sys.argv.index("permuter-import")
            full_args = sys.argv[permuter_index + 1:]
            runner.permuter_import(args=full_args)
    except subprocess.CalledProcessError:
        sys.exit(1)

if __name__ == "__main__":
    main()
