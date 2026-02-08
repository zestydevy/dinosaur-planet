#!/usr/bin/env python3
import argparse
from io import BufferedReader
import os
from pathlib import Path
import struct

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")

def player_name(playerNo: int):
    if playerNo == 0:
        return "Ambience 1"
    elif playerNo == 1:
        return "Ambience 2"
    elif playerNo == 2:
        return "Music 1"
    elif playerNo == 3:
        return "Music 2"
    else:
        return str(playerNo)

def list_actions(reader: BufferedReader, id_filter: int | None, player_filter: int | None, seq_filter: int | None):
    reader.seek(0, os.SEEK_END)
    total = reader.tell() // 0x20
    reader.seek(0, os.SEEK_SET)

    for i in range(total):
        unk0, unk1, distFalloffExp, unk3, distFalloffStart, distFalloffEnd, unk8, unkC, playerNo, \
            seqID, volume, bpm, unk14, fadeTimeDs, unk16, unk18, unk1A, unk1C \
            = struct.unpack_from(">BBBBHHIIBBBBBBHHHI", reader.read(0x20))
        
        id = i + 1
        if id_filter != None and id_filter != id:
            continue
        if player_filter != None and player_filter != playerNo:
            continue
        if seq_filter != None and seq_filter != seqID:
            continue

        print(f"{id} (0x{id:X})")
        print(f"  unk0: {unk0}, unk1: {unk1}, unk2: {distFalloffExp}, unk3: {unk3}")
        print(f"  distFalloffStart: {distFalloffStart}, distFalloffEnd: {distFalloffEnd}")
        print(f"  unk8: {unk8}, unkC: {unkC}")
        print(f"  playerNo: {player_name(playerNo)} ({playerNo}), seqID: {seqID}, volume: {volume}")
        print(f"  bpm: {bpm}, unk14: {unk14}, fadeTimeDs: {fadeTimeDs}")
        print(f"  unk16: 0x{unk16:X}, unk18: 0x{unk18:X}")
        print(f"  unk1A: 0x{unk1A:X}, unk1C: {unk1C}")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("--file", type=str, help="Path to MUSICACTIONS.bin. (optional)")
    parser.add_argument("--id", type=str, help="Filter by action ID.")
    parser.add_argument("--player", type=str, help="Filter by sequence player.")
    parser.add_argument("--seq", type=str, help="Filter by sequence ID.")

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    if args.file != None:
        path = args.file
    else:
        path = BIN_ASSETS_DIR.joinpath("MUSICACTIONS.bin")

    with open(path, "rb") as musicactions_bin:
        list_actions(musicactions_bin, 
                     int(args.id, base=0) if args.id else None, 
                     int(args.player, base=0) if args.player else None, 
                     int(args.seq, base=0) if args.seq else None)

if __name__ == "__main__":
    main()