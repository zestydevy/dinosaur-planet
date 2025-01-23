# Dinosaur Planet DLL packer
# by nuggs#1832 / nuggslet

import glob
from pathlib import Path
import os, sys
import struct

class dino_dll():
    def pack(self, bin, tab, tab_out, dir):
        paths = [Path(path) for path in glob.glob(f"{Path(dir)}/*.dll")]
        paths = sorted(paths, key=lambda x: int(x.name.split('.')[0]))

        ftab = bytearray(open(tab, "rb").read())
        fbin = open(bin, "wb")

        pos = 0
        index = 0

        for path in paths:
            size = os.path.getsize(path)

            with open(path, "rb") as file:
                fbin.write(file.read())

            offset = (index * 8) + (4 * 4) # (index * entry_size) + tab_header_size

            # Use .bss size from elf2dll if available
            bss_path = path.with_name(f"{path.name}.bss.txt")
            if bss_path.exists():
                with open(bss_path, "r", encoding="utf-8") as bss_file:
                    bss = int(bss_file.read(), base=0)
            else:
                bss = struct.unpack_from(">2I", ftab, offset)[1]

            struct.pack_into(">2I", ftab, offset, pos, bss)
            pos += size

            index += 1

        # Write "fake" final entry
        offset = (index * 8) + (4 * 4)
        struct.pack_into(">2I", ftab, offset, pos, 0x0)

        open(tab_out, "wb").write(ftab)
        fbin.close()

    def unpack(self, bin, tab, dir):
        tab = open(tab, "rb").read()
        tab = tab[16:]

        bin = open(bin, "rb").read()

        if not os.path.exists(dir):
            os.makedirs(dir)

        i = 1
        while tab:
            data = struct.unpack_from(">3I", tab)
            tab = tab[8:]

            offset = data[0]
            next = data[2]
            size = next - offset

            # Note: final entry before 0xFFFFFFFF is not an actual DLL
            if next == 0xFFFFFFFF: break

            name = "%d.dll" % i
            name = os.path.join(dir, name)

            data = bin[offset:next]
            open(name, "wb").write(data)
            codeStart = struct.unpack_from(">3I", bin, offset)[0]
            codeEnd = struct.unpack_from(">3I", bin, offset)[2]
            codeLen = codeEnd - codeStart
            mainOffset = 58922956 + offset
            #print("    - [" + str(hex(mainOffset + codeStart)) + ", c, dlls2/dll" + str(i) + "]")
            #if codeEnd != 0xFFFFFFFF:
            #    print("    - [" + str(hex(mainOffset + codeEnd)) + ", bin, dll" + str(i) + "_data]")
            i += 1

def main():
    import argparse

    parser = argparse.ArgumentParser(description="Dinosaur Planet DLL packer by nuggs")
    parser.add_argument("mode", type=str, help="Mode", nargs="?", choices=["pack", "unpack"], default="unpack")
    parser.add_argument("dir", type=str, help="DLL directory")
    parser.add_argument("bin", type=str, help="DLLS.bin")
    parser.add_argument("tab", type=str, help="DLLS.tab")
    parser.add_argument("--tab_out", type=str, help="DLLS.tab output (packing, defaults to tab)")
    parser.add_argument("-q", "--quiet", help="Pass to suppress logging.", action="store_true")

    if len(sys.argv) == 1:
        parser.print_help(sys.stderr)
        sys.exit(1)

    args = parser.parse_args()
    dll = dino_dll()

    if args.mode == "pack":
        tab_out = args.tab_out
        if tab_out == None:
            tab_out = args.tab
        dll.pack(args.bin, args.tab, tab_out, args.dir)
        if not args.quiet:
            print("DLL packing complete.")
    if args.mode == "unpack":
        dll.unpack(args.bin, args.tab, args.dir)
        if not args.quiet:
            print("DLL unpacking complete.")

if __name__ == "__main__":
    main()
