#!/usr/bin/env python3
import argparse
from io import BufferedReader
import os
from pathlib import Path
import struct

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")
MP3_DIR = Path("bin/mp3")

def read_mpeg(mpeg: BufferedReader, mpeg_tab: BufferedReader):
    MP3_DIR.mkdir(exist_ok=True)
    
    offsets: list[int] = []
    while True:
        offset_bytes = mpeg_tab.read(4)
        if len(offset_bytes) == 0:
            break
        offsets.append(struct.unpack(">I", offset_bytes)[0])

    for i in range(len(offsets) - 1):
        offset = offsets[i]
        size = offsets[i + 1] - offset

        mpeg.seek(offset, os.SEEK_SET)
        mpeg_bytes = mpeg.read(size)

        with open(MP3_DIR.joinpath(f"{i}.mp3"), "wb") as mpeg_file:
            mpeg_file.write(mpeg_bytes)
    
    print(f"Wrote {len(offsets) - 1} files to {MP3_DIR.absolute()}")

def main():
    parser = argparse.ArgumentParser(description="Extracts MPEG.bin.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    with open(BIN_ASSETS_DIR.joinpath("MPEG.bin"), "rb") as mpeg, \
         open(BIN_ASSETS_DIR.joinpath("MPEG_tab.bin"), "rb") as mpeg_tab:
        read_mpeg(mpeg, mpeg_tab)

if __name__ == "__main__":
    main()