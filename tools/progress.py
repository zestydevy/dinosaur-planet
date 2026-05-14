#!/usr/bin/env python3
# Computes and displays progress for the decompilation project

import argparse
from typing import TextIO
from git.repo import Repo
from io import TextIOWrapper
import json
import os
from pathlib import Path

from dino.dlls_txt import DLLsTxt

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
ROOT_DIR = Path(os.path.abspath(os.path.join(SCRIPT_DIR, "..")))

ASM_PATH = ROOT_DIR.joinpath("asm")
SRC_DLLS_PATH = ROOT_DIR.joinpath("src/dlls")

class DLLProgress:
    def __init__(self,
                 number: int,
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

class ProgressCategory:
    def __init__(self) -> None:
        self.total_bytes = 0
        self.total_funcs = 0
        self.matching_bytes = 0
        self.matching_funcs = 0
        self.matching_funcs_ratio = 0
        self.matching_bytes_ratio = 0
    
    def compute_ratios(self):
        if self.total_funcs != 0:
            self.matching_funcs_ratio = self.matching_funcs / self.total_funcs
        else:
            self.matching_funcs_ratio = 0
        if self.total_bytes != 0:
            self.matching_bytes_ratio = self.matching_bytes / self.total_bytes
        else:
            self.matching_bytes_ratio = 0

class OverallProgress:
    def __init__(self,
                 core: CoreProgress,
                 dlls: "list[DLLProgress]") -> None:
        # Compute core progress
        self.core = ProgressCategory()

        self.core.total_bytes = core.total_bytes
        self.core.total_funcs = core.total_funcs
        self.core.matching_bytes = core.matching_bytes
        self.core.matching_funcs = core.matching_funcs

        # Compute total DLL progress
        self.dll = ProgressCategory()
        self.dll_engine = ProgressCategory()
        self.dll_modgfx = ProgressCategory()
        self.dll_projgfx = ProgressCategory()
        self.dll_objects = ProgressCategory()

        for progress in dlls:
            if progress.number >= 210:
                subcategory = self.dll_objects
            elif progress.number >= (185 + 1):
                subcategory = self.dll_projgfx
            elif progress.number >= (103 + 1):
                subcategory = self.dll_modgfx
            else:
                subcategory = self.dll_engine

            self.dll.total_bytes += progress.total_bytes
            self.dll.total_funcs += progress.total_funcs
            self.dll.matching_bytes += progress.matching_bytes
            self.dll.matching_funcs += progress.matching_funcs

            subcategory.total_bytes += progress.total_bytes
            subcategory.total_funcs += progress.total_funcs
            subcategory.matching_bytes += progress.matching_bytes
            subcategory.matching_funcs += progress.matching_funcs

        # Compute overall progress
        self.total = ProgressCategory()
        self.total.total_bytes = core.total_bytes + self.dll.total_bytes
        self.total.total_funcs = core.total_funcs + self.dll.total_funcs
        self.total.matching_bytes = core.matching_bytes + self.dll.matching_bytes
        self.total.matching_funcs = core.matching_funcs + self.dll.matching_funcs

        # Compute ratios
        self.core.compute_ratios()
        self.dll.compute_ratios()
        self.dll_engine.compute_ratios()
        self.dll_modgfx.compute_ratios()
        self.dll_projgfx.compute_ratios()
        self.dll_objects.compute_ratios()
        self.total.compute_ratios()

def add_funcs_from_asm(file: TextIO, funcs: set[str]):
    in_text: bool | None = None
    total_bytes = 0
    for line in file.readlines():
        if line.startswith(".section"):
            section = line.split(" ")[1][:-1]
            if section == ".text":
                in_text = True
            else:
                if in_text:
                    # Left .text, we can exit early
                    break
                in_text = False
        elif in_text != False and line.startswith("nonmatching"):
            _, name, size = line.split(" ")
            name = name[:-1]
            size = int(size, base=0)
            total_bytes += size
            funcs.add(name)
    
    return total_bytes

def get_core_progress() -> CoreProgress:
    data_path = ASM_PATH.joinpath("data")
    dlls_path = ASM_PATH.joinpath("dlls")
    nonmatchings_path = ASM_PATH.joinpath("nonmatchings")
    nonmatchings_dlls_path = ASM_PATH.joinpath("nonmatchings/dlls")

    nonmatching_funcs: set[str] = set()
    nonmatching_bytes = 0
    all_funcs: set[str] = set()
    total_bytes = 0

    for path in ASM_PATH.glob("**/*.s"):
        if path.is_relative_to(data_path) or path.is_relative_to(dlls_path) or path.is_relative_to(nonmatchings_dlls_path):
            continue

        with open(path, "r", encoding="utf-8") as file:
            if path.is_relative_to(nonmatchings_path):
                nonmatching_bytes += add_funcs_from_asm(file, nonmatching_funcs)
            else:
                total_bytes += add_funcs_from_asm(file, all_funcs)
    
    matching_bytes = total_bytes - nonmatching_bytes
    matching_funcs = all_funcs - nonmatching_funcs

    # Done
    return CoreProgress(
        total_bytes=total_bytes,
        total_funcs=len(all_funcs),
        matching_bytes=matching_bytes,
        matching_funcs=len(matching_funcs)
    )

def get_dll_progress(number: int, src_dir_name: str | None) -> DLLProgress:
    all_funcs: set[str] = set()
    with open(ASM_PATH.joinpath(f"dlls/{number}.s"), "r", encoding="utf-8") as file:
        total_bytes = add_funcs_from_asm(file, all_funcs)
    
    if src_dir_name != None:
        nonmatching_funcs: set[str] = set()
        nonmatching_bytes = 0
        dll_nonmatchings_path = ASM_PATH.joinpath("nonmatchings/dlls").joinpath(src_dir_name)
        if dll_nonmatchings_path.exists():
            for path in dll_nonmatchings_path.glob("**/*.s"):
                if path.stem == "_orig_got":
                    continue
                with open(path, "r", encoding="utf-8") as file:
                    nonmatching_bytes += add_funcs_from_asm(file, nonmatching_funcs)
        
        matching_funcs = all_funcs - nonmatching_funcs
        matching_bytes = total_bytes - nonmatching_bytes
    else:
        matching_funcs: set[str] = set()
        matching_bytes = 0
    
    return DLLProgress(
        number, 
        total_bytes=total_bytes, 
        total_funcs=len(all_funcs), 
        matching_bytes=matching_bytes,
        matching_funcs=len(matching_funcs)
    )

def get_all_dll_progress() -> "list[DLLProgress]":
    progress: "list[DLLProgress]" = []

    # Get custom src dir for each DLL
    dlls_txt_path = SRC_DLLS_PATH.joinpath("dlls.txt")
    assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
    
    with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
        dlls_txt = DLLsTxt.parse(dlls_txt_file)

    # Get progress of each DLL
    for i in range(796):
        number = i + 1
        dir_name = dlls_txt.path_map.get(number, None)
        progress.append(get_dll_progress(number, dir_name))
    
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
            "matching_ratio": p.total.matching_bytes_ratio,
            "matching_funcs": p.total.matching_funcs,
            "matching_bytes": p.total.matching_bytes,
            "total_funcs": p.total.total_funcs,
            "total_bytes": p.total.total_bytes,
        },
        "core": {
            "matching_ratio": p.core.matching_bytes_ratio,
            "matching_funcs": p.core.matching_funcs,
            "matching_bytes": p.core.matching_bytes,
            "total_funcs": p.core.total_funcs,
            "total_bytes": p.core.total_bytes,
        },
        "dll": {
            "matching_ratio": p.dll.matching_bytes_ratio,
            "matching_funcs": p.dll.matching_funcs,
            "matching_bytes": p.dll.matching_bytes,
            "total_funcs": p.dll.total_funcs,
            "total_bytes": p.dll.total_bytes,
            "engine": {
                "matching_ratio": p.dll_engine.matching_bytes_ratio,
                "matching_funcs": p.dll_engine.matching_funcs,
                "matching_bytes": p.dll_engine.matching_bytes,
                "total_funcs": p.dll_engine.total_funcs,
                "total_bytes": p.dll_engine.total_bytes,
            },
            "modgfx": {
                "matching_ratio": p.dll_modgfx.matching_bytes_ratio,
                "matching_funcs": p.dll_modgfx.matching_funcs,
                "matching_bytes": p.dll_modgfx.matching_bytes,
                "total_funcs": p.dll_modgfx.total_funcs,
                "total_bytes": p.dll_modgfx.total_bytes,
            },
            "projgfx": {
                "matching_ratio": p.dll_projgfx.matching_bytes_ratio,
                "matching_funcs": p.dll_projgfx.matching_funcs,
                "matching_bytes": p.dll_projgfx.matching_bytes,
                "total_funcs": p.dll_projgfx.total_funcs,
                "total_bytes": p.dll_projgfx.total_bytes,
            },
            "objects": {
                "matching_ratio": p.dll_objects.matching_bytes_ratio,
                "matching_funcs": p.dll_objects.matching_funcs,
                "matching_bytes": p.dll_objects.matching_bytes,
                "total_funcs": p.dll_objects.total_funcs,
                "total_bytes": p.dll_objects.total_bytes,
            }
        },
        "git": {
            "commit_hash": git_commit_hash,
            "commit_hash_short": git_commit_hash_short,
            "commit_timestamp": git_commit_timestamp
        },
    }

    # Output
    json.dump(data, file, indent=2)

def print_progress(p: OverallProgress, show_banks: bool):
    print(f"{p.total.matching_funcs} matched overall functions / {p.total.total_funcs} total ({p.total.matching_funcs_ratio * 100:.2f}%)")
    print(f"{p.total.matching_bytes} matching overall bytes / {p.total.total_bytes} total ({p.total.matching_bytes_ratio * 100:.2f}%)")
    print()
    print(f"{p.core.matching_funcs} matched core functions / {p.core.total_funcs} total ({p.core.matching_funcs_ratio * 100:.2f}%)")
    print(f"{p.core.matching_bytes} matching core bytes / {p.core.total_bytes} total ({p.core.matching_bytes_ratio * 100:.2f}%)")
    print()
    print(f"{p.dll.matching_funcs} matched DLL functions / {p.dll.total_funcs} total ({p.dll.matching_funcs_ratio * 100:.2f}%)")
    print(f"{p.dll.matching_bytes} matching DLL bytes / {p.dll.total_bytes} total ({p.dll.matching_bytes_ratio * 100:.2f}%)")
    if show_banks:
        print()
        print("DLL Banks:")
        print(f"    {p.dll_engine.matching_funcs} matched engine DLL functions / {p.dll_engine.total_funcs} total ({p.dll_engine.matching_funcs_ratio * 100:.2f}%)")
        print(f"    {p.dll_engine.matching_bytes} matching engine DLL bytes / {p.dll_engine.total_bytes} total ({p.dll_engine.matching_bytes_ratio * 100:.2f}%)")
        print()
        print(f"    {p.dll_modgfx.matching_funcs} matched modgfx DLL functions / {p.dll_modgfx.total_funcs} total ({p.dll_modgfx.matching_funcs_ratio * 100:.2f}%)")
        print(f"    {p.dll_modgfx.matching_bytes} matching modgfx DLL bytes / {p.dll_modgfx.total_bytes} total ({p.dll_modgfx.matching_bytes_ratio * 100:.2f}%)")
        print()
        print(f"    {p.dll_projgfx.matching_funcs} matched projgfx DLL functions / {p.dll_projgfx.total_funcs} total ({p.dll_projgfx.matching_funcs_ratio * 100:.2f}%)")
        print(f"    {p.dll_projgfx.matching_bytes} matching projgfx DLL bytes / {p.dll_projgfx.total_bytes} total ({p.dll_projgfx.matching_bytes_ratio * 100:.2f}%)")
        print()
        print(f"    {p.dll_objects.matching_funcs} matched object DLL functions / {p.dll_objects.total_funcs} total ({p.dll_objects.matching_funcs_ratio * 100:.2f}%)")
        print(f"    {p.dll_objects.matching_bytes} matching object DLL bytes / {p.dll_objects.total_bytes} total ({p.dll_objects.matching_bytes_ratio * 100:.2f}%)") 

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Computes and reports progress for the project.")
    parser.add_argument("-q", "--quiet", action="store_true", help="Don't print messages to stdout.", default=False)
    parser.add_argument("--json", type=str, help="File to write the current progress to as JSON.")
    parser.add_argument("--dll-banks", dest="dll_banks", action="store_true", help="Display progress per DLL bank.", default=False)

    args = parser.parse_args()
    
    # Compute progress
    if not args.quiet:
        print("Calculating progress...")

    progress = get_overall_progress()

    # Emit JSON
    if args.json:
        with open(args.json, "w", encoding="utf-8") as json_file:
            output_json(progress, json_file)

    # Print progress
    if not args.quiet:
        print()
        print_progress(progress, args.dll_banks)
