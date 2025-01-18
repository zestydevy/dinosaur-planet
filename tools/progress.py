#!/usr/bin/env python3
# Computes and displays progress for the decompilation project

import argparse
from git.repo import Repo
from glob import glob
from io import TextIOWrapper
import json
import os
from pathlib import Path
import re
import subprocess
import sys

from dino.dll import DLL, parse_dll_functions

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
ROOT_DIR = Path(os.path.abspath(os.path.join(SCRIPT_DIR, "..")))

ASM_PATH = ROOT_DIR.joinpath("asm")
BIN_PATH = ROOT_DIR.joinpath("bin")
BUILD_PATH = ROOT_DIR.joinpath("build")
SRC_PATH = ROOT_DIR.joinpath("src")

symbol_pattern = re.compile(r"(\S+)\s*=\s*(\S+);")

class DLLProgress:
    def __init__(self,
                 number: str,
                 total_bytes: int,
                 total_funcs: int,
                 matching_bytes: int,
                 matching_funcs: int) -> None:
        self.number = number
        self.total_bytes = total_bytes
        self.total_funcs = total_funcs
        self.matching_bytes = matching_bytes
        self.matching_funcs = matching_funcs

class CoreProgress:
    def __init__(self,
                 total_bytes: int,
                 total_funcs: int,
                 matching_bytes: int,
                 matching_funcs: int) -> None:
        self.total_bytes = total_bytes
        self.total_funcs = total_funcs
        self.matching_bytes = matching_bytes
        self.matching_funcs = matching_funcs

class OverallProgress:
    def __init__(self,
                 core: CoreProgress,
                 dlls: "list[DLLProgress]") -> None:
        self.core = core
        self.dlls = dlls

        # Compute total DLL progress
        self.dll_total_bytes = 0
        self.dll_total_funcs = 0
        self.dll_matching_bytes = 0
        self.dll_matching_funcs = 0

        for progress in dlls:
            self.dll_total_bytes += progress.total_bytes
            self.dll_total_funcs += progress.total_funcs
            self.dll_matching_bytes += progress.matching_bytes
            self.dll_matching_funcs += progress.matching_funcs

        # Compute overall progress
        self.total_bytes = core.total_bytes + self.dll_total_bytes
        self.total_funcs = core.total_funcs + self.dll_total_funcs
        self.matching_bytes = core.matching_bytes + self.dll_matching_bytes
        self.matching_funcs = core.matching_funcs + self.dll_matching_funcs

        # Compute ratios
        self.core_matching_funcs_ratio = core.matching_funcs / core.total_funcs
        self.core_matching_bytes_ratio = core.matching_bytes / core.total_bytes

        self.dll_matching_funcs_ratio = self.dll_matching_funcs / self.dll_total_funcs
        self.dll_matching_bytes_ratio = self.dll_matching_bytes / self.dll_total_bytes

        self.matching_funcs_ratio = self.matching_funcs / self.total_funcs
        self.matching_bytes_ratio = self.matching_bytes / self.total_bytes

def get_core_func_sizes(elf_path: Path) -> "tuple[dict[str, int], int]":
    # Get functions and their sizes from the given .elf
    try:
        result = subprocess.run(['mips-linux-gnu-readelf', '--symbols', elf_path], stdout=subprocess.PIPE)
        lines = result.stdout.decode().split("\n")
    except:
        print(f"Error: Could not run mips-linux-gnu-readelf on {elf_path} - make sure that the project is built")
        sys.exit(1)

    sizes = {}
    total = 0

    for line in [l for l in lines if "FUNC" in l]:
        components = line.split()
        size = int(components[2])
        name = components[7]
        # Include asm functions (which have a size of 0), 
        # but exclude branch labels (which also count as funcs and have a size of 0)
        if size > 0 or not name.startswith("L8"):
            total += size
            sizes[name] = size

    return sizes, total

def get_core_nonmatching_funcs() -> "set[str]":
    nonmatching_path = ASM_PATH.joinpath("nonmatchings")
    funcs = set()

    for asm_path in nonmatching_path.rglob("*.s"):
        # Skip DLL nonmatchings
        if asm_path.relative_to(nonmatching_path).parts[0] == "dlls":
            continue

        # Add
        funcs.add(asm_path.stem)

    return funcs

def get_core_progress() -> CoreProgress:
    # Get all core functions and their sizes from the final .elf
    dino_elf_path = BUILD_PATH.joinpath("dino.elf")
    func_sizes, total_bytes = get_core_func_sizes(dino_elf_path)
    all_funcs = set(func_sizes.keys())

    # Get nonmatching functions
    nonmatching_funcs = get_core_nonmatching_funcs()

    # Compute matching amount
    matching_funcs = all_funcs - nonmatching_funcs
    matching_bytes = 0
    for func in matching_funcs:
        matching_bytes += func_sizes[func]

    # Done
    return CoreProgress(
        total_bytes=total_bytes,
        total_funcs=len(all_funcs),
        matching_bytes=matching_bytes,
        matching_funcs=len(matching_funcs)
    )

def read_dll_symbols_txt(path: Path) -> "dict[int, str]":
    symbols: "dict[int, str]" = {}

    with open(path, "r", encoding="utf-8") as syms_file:
        for line in syms_file.readlines():
            pairs = symbol_pattern.findall(line.strip())
            for pair in pairs:
                addr_str: str = pair[1]
                if addr_str.lower().startswith("0x"):
                    addr = int(addr_str, base=16)
                else:
                    addr = int(addr_str)
                
                symbols[addr] = pair[0]

    return symbols

def get_dll_progress(dll_path: Path, number: str) -> DLLProgress:
    known_symbols: "dict[int, str]" = {}
    nonmatching_funcs: "set[str]" = set()
    has_src = False

    # To determine progress we need to check if the DLL has a src directory
    # If it does, we need its syms.txt and we need to check the respective asm/nonmatchings directory
    syms_path = SRC_PATH.joinpath(f"dlls/{number}/syms.txt")
    if syms_path.exists():
        has_src = True
        # Get a list of known symbols for the DLL (we need the function symbols)
        known_symbols = read_dll_symbols_txt(syms_path)
        # Get list of functions that aren't matching
        nonmatchings_dir = ASM_PATH.joinpath(f"nonmatchings/dlls/{number}")
        if nonmatchings_dir.exists():
            for asm_file in nonmatchings_dir.iterdir():
                if asm_file.name.endswith(".s"):
                    nonmatching_funcs.add(asm_file.name[:-2])
    
    # Get all DLL functions and their sizes
    with open(dll_path, "rb") as dll_file:
        dll_data = bytearray(dll_file.read())
        dll = DLL.parse(dll_data, number)
        dll_functions = parse_dll_functions(dll_data, dll, known_symbols=known_symbols)
    
    func_sizes: "dict[str, int]" = {}
    total_bytes = 0
    for func in dll_functions:
        size = len(func.insts) * 4
        func_sizes[func.symbol] = size
        total_bytes += size
    
    # Compute matching amounts
    if has_src:
        matching_funcs = set(func_sizes.keys()) - nonmatching_funcs
        matching_bytes = 0
        for func in matching_funcs:
            matching_bytes += func_sizes[func]
    else:
        matching_funcs = []
        matching_bytes = 0

    # Done
    return DLLProgress(
        number, 
        total_bytes=total_bytes, 
        total_funcs=len(func_sizes), 
        matching_bytes=matching_bytes,
        matching_funcs=len(matching_funcs)
    )

def get_all_dll_progress() -> "list[DLLProgress]":
    dlls_dir = BIN_PATH.joinpath("assets/dlls")
    progress: "list[DLLProgress]" = []

    # Get progress of each .dll asset
    for dll_path in [Path(p) for p in glob(f"{dlls_dir}/*.dll")]:
        number = dll_path.name.split(".")[0]
        progress.append(get_dll_progress(dll_path, number))
    
    return progress

def get_overall_progress() -> OverallProgress:
    # Get core progress
    core = get_core_progress()
    # Get DLL progress
    dlls = get_all_dll_progress()
    # Return overall
    return OverallProgress(core, dlls)

def output_json(p: OverallProgress, file: TextIOWrapper):
    # Get current commit info
    repo = Repo()
    git_head_obj = repo.head.object
    git_commit_hash = git_head_obj.hexsha
    git_commit_hash_short = repo.git.rev_parse(git_commit_hash, short=7)
    git_commit_timestamp = git_head_obj.committed_date

    # Build JSON data
    data = {
        "total": {
            "matching_ratio": p.matching_bytes_ratio,
            "matching_funcs": p.matching_funcs,
            "matching_bytes": p.matching_bytes,
            "total_funcs": p.total_funcs,
            "total_bytes": p.total_bytes,
        },
        "core": {
            "matching_ratio": p.core_matching_bytes_ratio,
            "matching_funcs": p.core.matching_funcs,
            "matching_bytes": p.core.matching_bytes,
            "total_funcs": p.core.total_funcs,
            "total_bytes": p.core.total_bytes,
        },
        "dll": {
            "matching_ratio": p.dll_matching_bytes_ratio,
            "matching_funcs": p.dll_matching_funcs,
            "matching_bytes": p.dll_matching_bytes,
            "total_funcs": p.dll_total_funcs,
            "total_bytes": p.dll_total_bytes,
        },
        "git": {
            "commit_hash": git_commit_hash,
            "commit_hash_short": git_commit_hash_short,
            "commit_timestamp": git_commit_timestamp
        },
    }

    # Output
    json.dump(data, file, indent=2)

def print_progress(p: OverallProgress):
    print(f"{p.core.matching_funcs} matched core functions / {p.core.total_funcs} total ({p.core_matching_funcs_ratio * 100:.2f}%)")
    print(f"{p.core.matching_bytes} matching core bytes / {p.core.total_bytes} total ({p.core_matching_bytes_ratio * 100:.2f}%)")
    print()
    print(f"{p.dll_matching_funcs} matched DLL functions / {p.dll_total_funcs} total ({p.dll_matching_funcs_ratio * 100:.2f}%)")
    print(f"{p.dll_matching_bytes} matching DLL bytes / {p.dll_total_bytes} total ({p.dll_matching_bytes_ratio * 100:.2f}%)")
    print()
    print(f"{p.matching_funcs} matched overall functions / {p.total_funcs} total ({p.matching_funcs_ratio * 100:.2f}%)")
    print(f"{p.matching_bytes} matching overall bytes / {p.total_bytes} total ({p.matching_bytes_ratio * 100:.2f}%)")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Computes and reports progress for the project.")
    parser.add_argument("-q", "--quiet", action="store_true", help="Don't print messages to stdout.", default=False)
    parser.add_argument("--json", type=argparse.FileType("w", encoding="utf-8"), help="File to write the current progress to as JSON.")

    args = parser.parse_args()
    
    # Compute progress
    if not args.quiet:
        print("Calculating progress...")

    progress = get_overall_progress()

    # Emit JSON
    if args.json:
        with args.json as json_file:
            output_json(progress, json_file)

    # Print progress
    if not args.quiet:
        print()
        print_progress(progress)
