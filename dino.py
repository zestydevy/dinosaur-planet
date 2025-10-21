#!/usr/bin/env python3

import argparse
import glob
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tarfile
import urllib.request

from tools.dino.dlls_txt import DLLsTxt 

TARGET = "dino"
IDO_STATIC_RECOMP_VERSION = "v1.2"

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

DINO_DLL_PY = TOOLS_PATH.joinpath("dino_dll.py")
DLL_SPLIT_PY = TOOLS_PATH.joinpath("dll_split.py")
CONFIGURE_PY = TOOLS_PATH.joinpath("configure.py")
DIFF_PY = TOOLS_PATH.joinpath("asm_differ/diff.py")
M2CTX_PY = TOOLS_PATH.joinpath("m2ctx.py")
PERMUTER_IMPORT_PY = TOOLS_PATH.joinpath("decomp_permuter/import.py")
PERMUTER_PERMUTER_PY = TOOLS_PATH.joinpath("decomp_permuter/permuter.py")

GLOBAL_ASM_PATTERN = re.compile(r"#pragma GLOBAL_ASM\(\"(.+)/(.+)\.s\"\)")

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

    def clean_rebuild(self):
        self.clean()
        self.extract(core_only=False, use_cache=False, disassemble_all=False)
        self.build(configure=False, force=False, skip_expected=False, no_verify=False, target=None)

    def update_submodules(self):
        print("Updating Git submodules...")
        self.__run_cmd(["git", "submodule", "update", "--init", "--recursive"])

    def extract(self, core_only: bool, use_cache: bool, disassemble_all: bool):
        # Extract core/bin
        print("Extracting...")
        # If not using cache, clear existing extracted content
        if not use_cache:
            if ASM_PATH.exists():
                for path in ASM_PATH.glob("*"):
                    if path.name != "nonmatchings" and path.name != "matchings":
                        if path.is_dir():
                            if self.verbose:
                                print(f"rm {path}")
                            shutil.rmtree(path)
                        else:
                            if self.verbose:
                                print(f"rm {path}")
                            path.unlink()
                for dir in [ASM_PATH.joinpath("nonmatchings"), ASM_PATH.joinpath("matchings")]:
                    if dir.exists():
                        for path in dir.glob("*"):
                            if path.name != "dlls":
                                if self.verbose:
                                    print(f"rm {path}")
                                shutil.rmtree(path)
            if BIN_PATH.exists():
                if self.verbose:
                    print(f"rm {BIN_PATH}")
                shutil.rmtree(BIN_PATH)

        # Run splat
        args = [
            sys.executable, "-m", "splat", "split"
        ]

        if self.verbose:
            args.append("--verbose")
        if use_cache:
            args.append("--use-cache")
        if disassemble_all:
            args.append("--disassemble-all")
        
        args.append("splat.yaml")
        self.__run_cmd(args)
        
        # Unpack DLLs
        print()
        print("Unpacking DLLs...")
        self.__run_cmd([
            sys.executable, str(DINO_DLL_PY),
            "unpack",
            str(BIN_PATH.joinpath("assets/dlls")),
            str(BIN_PATH.joinpath("assets/DLLS.bin")),
            str(BIN_PATH.joinpath("assets/DLLS_tab.bin"))
        ])

        # Extract DLLs
        if not core_only:
            print()
            print("Extracting DLLs...")

            # If not using cache, clear existing DLL extracts
            if not use_cache:
                if ASM_PATH.exists():
                    for dir in [ASM_PATH.joinpath("nonmatchings"), ASM_PATH.joinpath("matchings")]:
                        if dir.exists():
                            for path in dir.glob("*"):
                                if path.name == "dlls":
                                    if self.verbose:
                                        print(f"rm {path}")
                                    shutil.rmtree(path)

            self.__extract_dlls(disassemble_all=disassemble_all)

        print()
        self.configure()

    def extract_dll(self, number: int, disassemble_all: bool):
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
        self.__extract_dlls([number], disassemble_all=disassemble_all)

        print()
        self.configure()

    def configure(self, non_matching: bool = False, non_equivalent: bool = False):
        print("Configuring build script...")

        self.__assert_project_built()

        cmd = [
            sys.executable, str(CONFIGURE_PY),
            "--base-dir", str(SCRIPT_DIR),
            "--target", TARGET
        ]

        if non_matching:
            cmd.append("--non-matching")
        if non_equivalent:
            cmd.append("--non-equivalent")

        self.__run_cmd(cmd)

    def build(self, 
              configure: bool, 
              force: bool, 
              skip_expected: bool, 
              no_verify: bool,
              target: "str | None"):
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
        if no_verify:
            print()
            print("Build successful.")
        else:
            print()
            self.verify()

        if not skip_expected and not no_verify:
            # If matching, update the 'expected' directory for diff
            self.create_expected_dir(already_verified=True, quiet=True)

    def check_ido_update(self):
        ido_dir = SCRIPT_DIR.joinpath("tools/ido_static_recomp")
        ido_version_file = ido_dir.joinpath("version.txt")

        version: str | None = None
        if ido_version_file.exists():
            version = ido_version_file.read_text()
        
        if version != IDO_STATIC_RECOMP_VERSION:
            print()
            print(
                f"Note: IDO static recomp is out-of-date. Please run '{self.__get_invoked_as()} update-ido' " +
                f"to update to {IDO_STATIC_RECOMP_VERSION}.")

    def update_ido(self, force: bool):
        # Check version
        ido_dir = SCRIPT_DIR.joinpath("tools/ido_static_recomp")
        ido_version_file = ido_dir.joinpath("version.txt")

        version: str | None = None
        if ido_version_file.exists():
            version = ido_version_file.read_text()
        
        if not force and version == IDO_STATIC_RECOMP_VERSION:
            print(f"IDO static recomp already up-to-date: {IDO_STATIC_RECOMP_VERSION}.")
            return
    
        def ido_download_url(ido_version: str):
            os_name: str
            if sys.platform == "win32":
                os_name = "windows"
            else:
                os_name = "linux"
            
            return f"https://github.com/decompals/ido-static-recomp/releases/download/{IDO_STATIC_RECOMP_VERSION}/ido-{ido_version}-recomp-{os_name}.tar.gz"
        
        def download_ido(ido_version: str, dir: Path):
            # Clear existing download
            if dir.exists():
                if self.verbose:
                    print(f"rm {dir}")
                shutil.rmtree(dir)
            if self.verbose:
                print(f"mkdir {dir}")
            dir.mkdir(exist_ok=True)

            # Download
            print(f"Downloading IDO {ido_version} to {dir.absolute().resolve()}...")
            filename = dir.joinpath(f"ido-{ido_version}-recomp.tar.gz")
            download_url = ido_download_url(ido_version)
            if self.verbose:
                print(f"Downloading '{download_url}' to '{filename.absolute().resolve()}'...")
            urllib.request.urlretrieve(download_url, filename)

            # Extract tar file
            if self.verbose:
                print(f"Extracting '{filename.absolute().resolve()}'...")
            with tarfile.open(filename, "r:gz") as tar:
                tar.extractall(dir, filter="data")
            
            # Clean up, remove tar file
            if self.verbose:
                print(f"rm {filename.absolute().resolve()}")
            filename.unlink()
        
        # Download both 5.3 and 7.1 since we need both
        ido_dir.mkdir(exist_ok=True)
            
        download_ido("5.3", ido_dir.joinpath("5.3"))
        download_ido("7.1", ido_dir.joinpath("7.1"))
        
        ido_version_file.write_text(IDO_STATIC_RECOMP_VERSION)
        print(f"Updated IDO static recomp to {IDO_STATIC_RECOMP_VERSION}.")

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
        file_paths = [Path(p) for p in glob.glob(f"{base_path}/**/*.o", recursive=True)]
        file_paths.append(base_path.joinpath(f"{TARGET}.z64"))
        file_paths.append(base_path.joinpath(f"{TARGET}.map"))

        to_create: "list[tuple[Path, Path]]" = []
        for in_path in file_paths:
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
        self.update_ido(force=False)
        print()
        self.extract(core_only=False, use_cache=False, disassemble_all=False)
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
            existing_path_absolute = SRC_PATH.joinpath(f"dlls/{existing_dll_dir}").relative_to(SCRIPT_DIR)
            print(f"An environment already exists at {existing_path_absolute}! " +
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

        self.__run_cmd([sys.executable, str(DIFF_PY)] + args)
    
    def objdiff(self, args: "list[str]"):
        self.__assert_project_built()

        self.__run_cmd(["objdiff-cli", "diff", "-p", SCRIPT_DIR] + args)
    
    def make_context(self, file: str):
        self.__run_cmd([sys.executable, str(M2CTX_PY), file])
        print(f"Created context file at {SCRIPT_DIR.joinpath('ctx.c')}")
    
    def permuter_import(self, args: "list[str]"):
        self.__run_cmd([sys.executable, str(PERMUTER_IMPORT_PY)] + args)
    
    def permuter(self, args: "list[str]"):
        self.__run_cmd([sys.executable, str(PERMUTER_PERMUTER_PY)] + args)

    def m2c(self, args: "list[str]"):
        self.__run_cmd(["m2c", "-t", "mips-ido-c"] + args)

    def decompile(self, file: str, func_name: str, keep_ctx: bool):
        filepath = Path(file)
        if not filepath.exists():
            print(f"File not found at: {filepath.absolute().resolve()}")
            sys.exit(1)

        # Find GLOBAL_ASM for function
        asmpath: Path | None = None
        with open(filepath, "r", encoding="utf-8") as c_file:
            for line in c_file.readlines():
                match = GLOBAL_ASM_PATTERN.match(line.strip())
                if match != None:
                    asmdir = match.group(1)
                    name = match.group(2)

                    if name == func_name:
                        asmpath = Path(asmdir).joinpath(f"{name}.s")
                        break

        if asmpath == None:
            print(f"Could not find GLOBAL_ASM pragma for function '{func_name}' in {filepath.absolute().resolve()}")
            sys.exit(1)

        # Re-create context for file
        if not keep_ctx:
            self.__run_cmd([sys.executable, str(M2CTX_PY), file])

        # Run m2c
        self.__run_cmd(["m2c", "-t", "mips-ido-c", "--context", "ctx.c", "-f", func_name, asmpath.as_posix()])
    
    def __assert_project_built(self):
        linker_script_path = SCRIPT_DIR.joinpath(f"{TARGET}.ld")
        if not linker_script_path.exists():
            print(f"Could not find linker script at {linker_script_path}. Is the ROM extracted?")
            sys.exit(1)

    def __run_cmd(self, args: "str | list[str]"):
        if self.verbose:
            print(">", " ".join(args))
        subprocess.check_call(args)

    def __extract_dlls(self, dlls: "list[str | int]"=[], quiet: bool=False, disassemble_all: bool=False):
        args = [
            sys.executable, str(DLL_SPLIT_PY),
            "--base-dir", str(SCRIPT_DIR),
        ]

        if self.verbose:
            args.append("--verbose")
        if not self.verbose and quiet:
            args.append("--quiet")
        if disassemble_all:
            args.append("--disassemble-all")

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
    
    extract_cmd = subparsers.add_parser("extract", help="Split ROM and extract DLLs.")
    extract_cmd.add_argument("--use-cache", action="store_true", dest="use_cache", help="Only split changed segments in splat config.", default=False)
    extract_cmd.add_argument("--disassemble-all", dest="disassemble_all", action="store_true", help="Disasemble matched functions and migrated data.", default=False)

    extract_core_cmd = subparsers.add_parser("extract-core", help="Split ROM (no DLLs).")
    extract_core_cmd.add_argument("--use-cache", action="store_true", dest="use_cache", help="Only split changed segments in splat config.", default=False)
    extract_core_cmd.add_argument("--disassemble-all", dest="disassemble_all", action="store_true", help="Disasemble matched functions and migrated data.", default=False)

    extract_dll_cmd = subparsers.add_parser("extract-dll", help="Extract a DLL.")
    extract_dll_cmd.add_argument("number", type=int, help="The number of the DLL.")
    extract_dll_cmd.add_argument("--disassemble-all", dest="disassemble_all", action="store_true", help="Disasemble matched functions and migrated data.", default=False)

    configure_cmd = subparsers.add_parser("configure", help="Re-configure the build script.")
    configure_cmd.add_argument("--non-matching", dest="non_matching", action="store_true", help="Define NON_MATCHING.", default=False)
    configure_cmd.add_argument("--non-equivalent", dest="non_equivalent", action="store_true", help="Define NON_EQUIVALENT.", default=False)

    build_cmd = subparsers.add_parser("build", help="Build ROM and verify that it matches.")
    build_cmd.add_argument("-c", "--configure", action="store_true", help="Re-configure the build script before building.", default=False)
    build_cmd.add_argument("-f", "--force", action="store_true", help="Force a full rebuild.", default=False)
    build_cmd.add_argument("--no-expected", dest="skip_expected", action="store_true", help="Don't update the 'expected' directory after a matching build.", default=False)
    build_cmd.add_argument("--no-verify", dest="no_verify", action="store_true", help="Don't verify the build ROM.", default=False)
    build_cmd.add_argument("target", nargs="?", help="The target to build. Don\'t specify to build the full ROM.")

    build_exp_cmd = subparsers.add_parser("build-expected", help="Update the 'expected' directory for diff. Requires a verified build.")
    build_exp_cmd.add_argument("-f", "--force", action="store_true", help="Fully recreate the directory instead of updating it.", default=False)

    subparsers.add_parser("verify", help="Verify that the re-built ROM matches the base ROM.")
    subparsers.add_parser("baseverify", help="Verify that the base ROM is correct.")
    subparsers.add_parser("clean", help="Remove extracted files, build artifacts, and build scripts.")
    subparsers.add_parser("clean-rebuild", help="Clean, re-extract, and re-build.")

    update_ido_cmd = subparsers.add_parser("update-ido", help="Download and update IDO static recomp to the version currently used by the decomp.")
    update_ido_cmd.add_argument("-f", "--force", action="store_true", help="Redownload even if IDO static recomp is already up-to-date.", default=False)

    subparsers.add_parser("submodules", help="Initialize and update Git submodules.")
    subparsers.add_parser("diff", help="Diff the re-rebuilt ROM with the original (redirects to asm-differ).", add_help=False)
    subparsers.add_parser("objdiff", help="Diff the re-rebuilt ROM with the original (redirects to objdiff-cli).", add_help=False)
    subparsers.add_parser("permuter", help="Randomly permute a C file to better match a target binary (redirects to decomp-permuter permuter.py).", add_help=False)
    subparsers.add_parser("permuter-import", help="Import a function for permuter (redirects to decomp-permuter import.py).", add_help=False)
    
    ctx_cmd = subparsers.add_parser("context", help="Create a context file that can be used for m2c/decomp.me.")
    ctx_cmd.add_argument("file", help="The C file to create context for.")

    subparsers.add_parser("m2c", help="Run m2c with the target (-t) argument already set (redirects to m2c).", add_help=False)

    decompile_cmd = subparsers.add_parser("decompile", help="Run m2c for a specific file/function. Automatically generates the context file.")
    decompile_cmd.add_argument("file", help="The C file to create context for. Must contain a GLOBAL_ASM pragma for the function to be decompiled.")
    decompile_cmd.add_argument("function", help="The name of the function to decompile. A GLOBAL_ASM pragma must exist with a path to the asm of the function.")
    decompile_cmd.add_argument("--keep-ctx", dest="keep_ctx", action="store_true", help="Don't regenerate ctx.c. Useful if you need to manually edit the context file.", default=False)

    args, _ = parser.parse_known_args()
    cmd = args.command

    try:
        runner = DinoCommandRunner(args.verbose)
        if cmd == "setup":
            runner.setup()
        elif cmd == "setup-dll":
            runner.setup_dll(number=args.number, dll_dir=args.dir)
        elif cmd == "extract":
            runner.extract(core_only=False, use_cache=args.use_cache, disassemble_all=args.disassemble_all)
        elif cmd == "extract-core":
            runner.extract(core_only=True, use_cache=args.use_cache, disassemble_all=args.disassemble_all)
        elif cmd == "extract-dll":
            runner.extract_dll(number=args.number, disassemble_all=args.disassemble_all)
        elif cmd == "build":
            runner.build(
                configure=args.configure, 
                force=args.force, 
                skip_expected=args.skip_expected,
                no_verify=args.no_verify,
                target=args.target
            )
        elif cmd == "build-expected":
            runner.create_expected_dir(force=args.force)
        elif cmd == "configure":
            runner.configure(
                non_matching=args.non_matching,
                non_equivalent=args.non_equivalent
            )
        elif cmd == "verify":
            runner.verify()
        elif cmd == "baseverify":
            runner.baseverify()
        elif cmd == "clean":
            runner.clean()
        elif cmd == "clean-rebuild":
            runner.clean_rebuild()
        elif cmd == "update-ido":
            runner.update_ido(args.force)
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
        elif cmd == "context":
            runner.make_context(args.file)
        elif cmd == "permuter":
            permuter_index = sys.argv.index("permuter")
            full_args = sys.argv[permuter_index + 1:]
            runner.permuter(args=full_args)
        elif cmd == "permuter-import":
            permuter_index = sys.argv.index("permuter-import")
            full_args = sys.argv[permuter_index + 1:]
            runner.permuter_import(args=full_args)
        elif cmd == "m2c":
            m2c_index = sys.argv.index("m2c")
            full_args = sys.argv[m2c_index + 1:]
            runner.m2c(args=full_args)
        elif cmd == "decompile":
            runner.decompile(args.file, args.function, args.keep_ctx)
        
        # Notify user if IDO recomp is out-of-date
        if cmd != "setup" and cmd != "update-ido":
            runner.check_ido_update()
    except subprocess.CalledProcessError:
        sys.exit(1)

if __name__ == "__main__":
    main()