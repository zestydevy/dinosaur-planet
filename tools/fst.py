#!/usr/bin/env python3
import argparse
from io import BufferedReader
import os
from pathlib import Path
import struct

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")

FILENAMES = [
    "AUDIO_TAB", # 00
    "AUDIO_BIN", # 01
    "SFX_TAB", # 02
    "SFX_BIN", # 03
    "AMBIENT_TAB", # 04
    "AMBIENT_BIN", # 05
    "MUSIC_TAB", # 06
    "MUSIC_BIN", # 07
    "MPEG_TAB", # 08
    "MPEG_BIN", # 09
    "MUSICACTIONS_BIN", # 0A
    "CAMACTIONS_BIN", # 0B
    "LACTIONS_BIN", # 0C
    "ANIMCURVES_BIN", # 0D
    "ANIMCURVES_TAB", # 0E
    "OBJSEQ2CURVE_TAB", # 0F
    "FONTS_BIN", # 10
    "CACHEFON_BIN", # 11
    "CACHEFON2_BIN", # 12
    "GAMETEXT_BIN", # 13
    "GAMETEXT_TAB", # 14
    "GLOBALMAP_BIN", # 15
    "TABLES_BIN", # 16
    "TABLES_TAB", # 17
    "SCREENS_BIN", # 18
    "SCREENS_TAB", # 19
    "VOXMAP_BIN", # 1A
    "VOXMAP_TAB", # 1B
    "TEXPRE_TAB", # 1C
    "TEXPRE_BIN", # 1D
    "WARPTAB_BIN", # 1E
    "MAPS_BIN", # 1F
    "MAPS_TAB", # 20
    "MAPINFO_BIN", # 21
    "MAPSETUP_IND", # 22
    "MAPSETUP_TAB", # 23
    "TEX1_BIN", # 24
    "TEX1_TAB", # 25
    "TEXTABLE_BIN", # 26
    "TEX0_BIN", # 27
    "TEX0_TAB", # 28
    "BLOCKS_BIN", # 29
    "BLOCKS_TAB", # 2A
    "TRKBLK_BIN", # 2B
    "HITS_BIN", # 2C
    "HITS_TAB", # 2D
    "MODELS_TAB", # 2E
    "MODELS_BIN", # 2F
    "MODELIND_BIN", # 30
    "MODANIM_TAB", # 31
    "MODANIM_BIN", # 32
    "ANIM_TAB", # 33
    "ANIM_BIN", # 34
    "AMAP_TAB", # 35
    "AMAP_BIN", # 36
    "BITTABLE_BIN", # 37
    "WEAPONDATA_BIN", # 38
    "VOXOBJ_TAB", # 39
    "VOXOBJ_BIN", # 3A
    "MODLINES_BIN", # 3B
    "MODLINES_TAB", # 3C
    "SAVEGAME_BIN", # 3D
    "SAVEGAME_TAB", # 3E
    "OBJSEQ_BIN", # 3F
    "OBJSEQ_TAB", # 40
    "OBJECTS_TAB", # 41
    "OBJECTS_BIN", # 42
    "OBJINDEX_BIN", # 43
    "OBJEVENT_BIN", # 44
    "OBJHITS_BIN", # 45
    "DLLS_BIN", # 46
    "DLLS_TAB", # 47
    "DLLSIMPORTTAB_BIN", # 48
    "ENVFXACT_BIN", # 49
]

def read_fst(file: BufferedReader):
    file_count = struct.unpack(">I", file.read(4))[0]
    offsets: list[int] = []
    for i in range(file_count + 1):
        offsets.append(struct.unpack(">I", file.read(4))[0])
    
    print("{:<4} {:<8} {:<8} Name".format("Idx", "ROM", "Size"))
    for i in range(file_count):
        rom = 0xA4AA0 + offsets[i]
        size = offsets[i + 1] - offsets[i]
        print("{:<4X} {:<8X} {:<8X} {}".format(i, rom, size, FILENAMES[i]))

def main():
    parser = argparse.ArgumentParser(description="Queries FS_tab.bin.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    path = BIN_ASSETS_DIR.joinpath("FS_tab.bin")
    with open(path, "rb") as file:
        read_fst(file)

if __name__ == "__main__":
    main()