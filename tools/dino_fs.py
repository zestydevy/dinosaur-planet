#!/usr/bin/env python3
import argparse
from pathlib import Path
import struct

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
    "FILE_1A.bin", # 1A
    "FILE_1B.tab", # 1B
    "VOXMAP.tab", # 1C
    "VOXMAP.bin", # 1D
    "WARPTAB.bin", # 1E
    "MAPS.bin", # 1F
    "MAPS.tab", # 20
    "MAPINFO.bin", # 21
    "MAPSETUP.ind", # 22
    "MAPSETUP.tab", # 23
    "TEX1.bin", # 24
    "TEX1.tab", # 25
    "TEXTABLE.bin", # 26
    "TEX0.bin", # 27
    "TEX0.tab", # 28
    "BLOCKS.bin", # 29
    "BLOCKS.tab", # 2A
    "TRKBLK.tab", # 2B
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
    "SAVEGAMES.bin", # 3D
    "SAVEGAMES.tab", # 3E
    "OBJSEQ.bin", # 3F
    "OBJSEQ.tab", # 40
    "OBJECTS.tab", # 41
    "OBJECTS.bin", # 42
    "OBJINDEX.bin", # 43
    "OBJEVENTS.bin", # 44
    "OBJHITS.bin", # 45
    "DLLS.bin", # 46
    "DLLS.tab", # 47
    "DLLSIMPORTTAB.bin", # 48
    "ENVFXACTIONS.bin", # 49
]

def unpack(args):
    assets_tab_path = args.tab
    assets_bin_path = args.bin
    output_dir = Path(args.output)

    output_dir.mkdir(parents=True, exist_ok=True)

    # Read assets table
    with open(assets_tab_path, "rb") as assets_tab:
        file_count = struct.unpack(">I", assets_tab.read(4))[0]
        offsets: list[int] = []
        for i in range(file_count + 1):
            offsets.append(struct.unpack(">I", assets_tab.read(4))[0])

    # Extract files
    with open(assets_bin_path, "rb") as assets_bin:
        for i in range(file_count):
            offset = offsets[i]
            size = offsets[i + 1] - offsets[i]

            assets_bin.seek(offset)
            filename = FILENAMES[i]

            with open(output_dir.joinpath(filename), "wb") as file:
                file.write(assets_bin.read(size))

def pack(args):
    assets_tab_path = args.tab
    assets_bin_path = args.bin
    assets: list[str] = args.asset

    with open(assets_tab_path, "wb") as assets_tab, \
         open(assets_bin_path, "wb") as assets_bin:
        # Write file count
        assets_tab.write(struct.pack(">I", len(assets)))

        # Write files
        offset = 0
        for asset_path in assets:
            assets_tab.write(struct.pack(">I", offset))
            
            with open(asset_path, "rb") as asset:
                assets_bin.write(asset.read())
                offset += asset.tell()
        assets_tab.write(struct.pack(">I", offset))

def main():
    parser = argparse.ArgumentParser(description="Packs/unpacks the Dinosaur Planet ROM filesystem.")
    subparsers = parser.add_subparsers(required=True)

    unpack_parser = subparsers.add_parser("unpack")
    unpack_parser.add_argument("--tab", type=str, help="Path to assets.tab.", required=True)
    unpack_parser.add_argument("--bin", type=str, help="Path to assets.bin.", required=True)
    unpack_parser.add_argument("-o", "--output", type=str, help="Output directory to write extracted files to.", required=True)
    unpack_parser.set_defaults(func=unpack)

    pack_parser = subparsers.add_parser("pack")
    pack_parser.add_argument("--tab", type=str, help="Output path to assets.tab.", required=True)
    pack_parser.add_argument("--bin", type=str, help="Output path to assets.bin.", required=True)
    pack_parser.add_argument("asset", type=str, nargs="+", help="Asset files to repack.")
    pack_parser.set_defaults(func=pack)

    args = parser.parse_args()
    args.func(args)

if __name__ == "__main__":
    main()
