#!/usr/bin/env python3
import argparse
from io import BufferedReader
import os
from pathlib import Path
import struct

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")

FILENAMES = [
    "AUDIO.tab", # 00
    "AUDIO.bin", # 01
    "SFX.tab", # 02
    "SFX.bin", # 03
    "AMBIENT.tab", # 04
    "AMBIENT.bin", # 05
    "MUSIC.tab", # 06
    "MUSIC.bin", # 07
    "MPEG.tab", # 08
    "MPEG.bin", # 09
    "MUSICACTIONS.bin", # 0A
    "CAMACTIONS.bin", # 0B
    "LACTIONS.bin", # 0C
    "ANIMCURVES.bin", # 0D
    "ANIMCURVES.tab", # 0E
    "OBJSEQ2CURVE.tab", # 0F
    "FONTS.bin", # 10
    "CACHEFON.bin", # 11
    "CACHEFON2.bin", # 12
    "GAMETEXT.bin", # 13
    "GAMETEXT.tab", # 14
    "GLOBALMAP.bin", # 15
    "TABLES.bin", # 16
    "TABLES.tab", # 17
    "SCREENS.bin", # 18
    "SCREENS.tab", # 19
    "VOXMAP.bin", # 1A
    "VOXMAP.tab", # 1B
    "TEXPRE.tab", # 1C
    "TEXPRE.bin", # 1D
    "WARPTAB.bin", # 1E
    "MAPS.bin", # 1F
    "MAPS.tab", # 20
    "MAPINFO.bin", # 21
    "MAPSETUP_IND", # 22
    "MAPSETUP.tab", # 23
    "TEX1.bin", # 24
    "TEX1.tab", # 25
    "TEXTABLE.bin", # 26
    "TEX0.bin", # 27
    "TEX0.tab", # 28
    "BLOCKS.bin", # 29
    "BLOCKS.tab", # 2A
    "TRKBLK.bin", # 2B
    "HITS.bin", # 2C
    "HITS.tab", # 2D
    "MODELS.tab", # 2E
    "MODELS.bin", # 2F
    "MODELIND.bin", # 30
    "MODANIM.tab", # 31
    "MODANIM.bin", # 32
    "ANIM.tab", # 33
    "ANIM.bin", # 34
    "AMAP.tab", # 35
    "AMAP.bin", # 36
    "BITTABLE.bin", # 37
    "WEAPONDATA.bin", # 38
    "VOXOBJ.tab", # 39
    "VOXOBJ.bin", # 3A
    "MODLINES.bin", # 3B
    "MODLINES.tab", # 3C
    "SAVEGAME.bin", # 3D
    "SAVEGAME.tab", # 3E
    "OBJSEQ.bin", # 3F
    "OBJSEQ.tab", # 40
    "OBJECTS.tab", # 41
    "OBJECTS.bin", # 42
    "OBJINDEX.bin", # 43
    "OBJEVENT.bin", # 44
    "OBJHITS.bin", # 45
    "DLLS.bin", # 46
    "DLLS.tab", # 47
    "DLLSIMPORTTAB.bin", # 48
    "ENVFXACT.bin", # 49
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
    parser.add_argument("-f", "--file", type=str, dest="file", help=f"The FST_tab.bin file to read, relative to the base directory (or an absolute path).", 
                        default=str(BIN_ASSETS_DIR.joinpath("FS_tab.bin")))

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    with open(args.file, "rb") as file:
        read_fst(file)

if __name__ == "__main__":
    main()