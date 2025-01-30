#!/usr/bin/env python3
import argparse
from io import BufferedReader
import os
from pathlib import Path
import struct

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")

class BitTableEntry:
    def __init__(self, 
                 start_bit: int,
                 length: int,
                 is_task: bool,
                 bitstring: int,
                 task: int) -> None:
        self.start_bit = start_bit
        self.length = length
        self.is_task = is_task
        self.bitstring = bitstring
        self.task = task

def read_bittable(file: BufferedReader,
                  only_tasks: bool,
                  only_entries: "set[int]"):
    entries: "list[BitTableEntry]" = []
    while True:
        data = file.read(4)
        if len(data) < 4:
            break

        entry_tuple = struct.unpack_from(">HBB", data)

        entries.append(BitTableEntry(
            start_bit=entry_tuple[0],
            length=entry_tuple[1] & 0b1_1111,
            is_task=(entry_tuple[1] & 0b10_0000) != 0,
            bitstring=entry_tuple[1] >> 6,
            task=entry_tuple[2] 
        ))
    
    print("{:<4} {:<3} {:<8} Task".format("Idx", "Sec", "Bits"))
    for i, entry in enumerate(entries):
        if only_tasks and not entry.is_task and entry.task == 0:
            continue
        if len(only_entries) > 0 and i not in only_entries:
            continue
            
        print("{:<4} {:<3} {:<8} {}, {}".format(
            i, 
            entry.bitstring,
            f"{entry.start_bit}[{entry.length + 1}]",
            entry.is_task,
            entry.task
        ))

def main():
    parser = argparse.ArgumentParser(description="Queries BITTABLE.bin.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("--tasks", action="store_true", help="Only show entries with tasks.", default=False)
    parser.add_argument("-i", "--index", nargs="*", action="extend", help="Only show these entries.", default=[])

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    path = BIN_ASSETS_DIR.joinpath("BITTABLE.bin")
    with open(path, "rb") as file:
        read_bittable(file,
                      only_tasks=args.tasks,
                      only_entries=set([int(i) for i in args.index]))

if __name__ == "__main__":
    main()