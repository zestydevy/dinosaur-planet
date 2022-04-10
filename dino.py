#!/usr/bin/env python3

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys

TARGET = "dino"

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
ASM_PATH = SCRIPT_DIR.joinpath("asm/")
BIN_PATH = SCRIPT_DIR.joinpath("bin/")
BUILD_PATH = SCRIPT_DIR.joinpath("build/")
TOOLS_PATH = SCRIPT_DIR.joinpath("tools/")

BUILD_SCRIPT_PATH = SCRIPT_DIR.joinpath("build.ninja")

# Paths to directories/files to delete when cleaning
CLEAN_PATHS = [
    ASM_PATH,
    BIN_PATH,
    BUILD_PATH,
    BUILD_SCRIPT_PATH,
    SCRIPT_DIR.joinpath(".ninja_log"),
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
CONFIGURE_PY = TOOLS_PATH.joinpath("configure.py")

def clean():
    print("Cleaning...")
    for path in CLEAN_PATHS:
        if not path.exists():
            continue
        
        print(f"  Removing {path.relative_to(SCRIPT_DIR)}...")
        if path.is_dir():
            shutil.rmtree(path)
        else:
            path.unlink()
    

def init_submodules():
    print("Updating Git submodules...")
    subprocess.check_call(["git", "submodule", "update", "--init", "--recursive"])

def extract():
    print("Extracting...")

    if ASM_PATH.exists():
        shutil.rmtree(ASM_PATH)
    if BIN_PATH.exists():
        shutil.rmtree(BIN_PATH)
    
    subprocess.check_call([
        "python3", str(SPLIT_PY), 
        "--target", "baserom.z64", 
        "--basedir", str(SCRIPT_DIR), 
        "splat.yaml"
    ])
    
    subprocess.check_call([
        "python3", str(DINO_DLL_PY), 
        "unpack",
        BIN_PATH.joinpath("assets/dlls"),
        BIN_PATH.joinpath("assets/DLLS.bin"),
        BIN_PATH.joinpath("assets/DLLS_tab.bin")
    ])

def configure(skip_dlls: bool):
    print("Configuring build script...")

    linker_script_path = SCRIPT_DIR.joinpath(f"{TARGET}.ld")
    if not linker_script_path.exists():
        print(f"Could not find linker script at {linker_script_path}. Is the ROM extracted?")
        sys.exit(1)
    
    args = [
        "python3", str(CONFIGURE_PY), 
        "--base-dir", str(SCRIPT_DIR),
        "--target", TARGET
    ]
    if skip_dlls:
        args.append("--skip-dlls")

    subprocess.check_call(args)

def build(reconfigure: bool, verbose: bool, force: bool):
    if reconfigure or not BUILD_SCRIPT_PATH.exists():
        configure(False) # TODO: DLL skipping should probably be done via a YAML config instead of command line
        print()
    
    if force:
        for path in BUILD_ARTIFACTS:
            if not path.exists():
                continue
            
            if path.is_dir():
                shutil.rmtree(path)
            else:
                path.unlink()

    print("Building ROM...")
    
    args = ["ninja"]
    if SCRIPT_DIR != Path.cwd():
        args.extend(["-C", str(SCRIPT_DIR)])
    if verbose:
        args.append("-v")
    
    subprocess.check_call(args)

    print()
    verify()

def baseverify():
    print("Verifying base ROM...")

    baserom_path = SCRIPT_DIR.joinpath("baserom.z64")
    if not baserom_path.exists():
        print(f"Missing base ROM! Ensure the real Dinosaur Planet ROM exists at {baserom_path.relative_to(SCRIPT_DIR)}.")
        sys.exit(1)
    
    subprocess.check_call(["md5sum", "-c", "baserom.md5"])

def verify():
    print("Verifying re-built ROM...")

    rom_path = SCRIPT_DIR.joinpath(f"build/{TARGET}.z64")
    if not rom_path.exists():
        print(f"Missing re-built ROM! Ensure the project has been built and that {rom_path.relative_to(SCRIPT_DIR)} exists.")
        sys.exit(1)
    
    subprocess.check_call(["md5sum", "-c", "checksum.md5"])

def setup():
    print("Welcome to the Dinosaur Planet decompilation project!")
    print()
    init_submodules()
    print()
    baseverify()
    print()
    extract()
    print()
    print("Done! Run './dino.py build' to build the ROM.")

def main():
    parser = argparse.ArgumentParser(description="Quick commands for working on the Dinosuar Planet decompilation.")
    subparsers = parser.add_subparsers(dest="command", required=True)
    
    # Commands:
    # setup - init submodules, verify baserom, extract
    # extract - split ROM and unpack DLLs
    # build - configure (if file doesn't exist), run ninja, verify
    # configure - re-configure build script
    # verify - verify that the built ROM matches the baserom
    # baseverify - verify that the baserom is correct
    # clean - delete extracted code, build artifacts, and build scripts
    # diff - run asm-differ (implement -m with this script and call ninja instead)
    # context - create ctx.c from the given file (or from all headers if no file is given)
    # TODO: dll tools? extract, dump?

    subparsers.add_parser("setup", help="Initialize tools, verify the base ROM, and extract the ROM.")
    subparsers.add_parser("extract", help="Split ROM and unpack DLLs.")
    build_cmd = subparsers.add_parser("build", help="Re-build ROM and verify that it matches.")
    build_cmd.add_argument("-c", "--configure", action="store_true", help="Re-configure the build script before building.", default=False)
    build_cmd.add_argument("-v", "--verbose", action="store_true", help="Show actual ninja commands while building.", default=False)
    build_cmd.add_argument("-f", "--force", action="store_true", help="Force a full rebuild.", default=False)
    configure_cmd = subparsers.add_parser("configure", help="Re-configure the build script.")
    configure_cmd.add_argument("--skip-dlls", dest="skip_dlls", action="store_true", help="Don't recopile DLLs (use original)", default=False)
    subparsers.add_parser("verify", help="Verify that the re-built ROM matches the base ROM.")
    subparsers.add_parser("baseverify", help="Verify that the base ROM is correct.")
    subparsers.add_parser("clean", help="Remove extracted files, build artifacts, and build scripts.")

    args = parser.parse_args()
    cmd = args.command

    try:
        if cmd == "setup":
            setup()
        elif cmd == "extract":
            extract()
        elif cmd == "build":
            build(reconfigure=args.configure, verbose=args.verbose, force=args.force)
        elif cmd == "configure":
            configure(skip_dlls=args.skip_dlls)
        elif cmd == "verify":
            verify()
        elif cmd == "baseverify":
            baseverify()
        elif cmd == "clean":
            clean()
    except subprocess.CalledProcessError:
        pass

if __name__ == "__main__":
    main()
