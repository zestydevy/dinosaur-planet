# Dinosaur Planet DLL packer
# by nuggs#1832 / nuggslet

import os, sys
import struct

class dino_dll():
    def pack(self, bin, tab, dir):
        names = os.listdir(dir)
        names = sorted(names, key=lambda x: int(x.split('.')[0]))

        ftab = bytearray(open(tab, "rb").read())
        fbin = open(bin, "wb")

        pos = 0
        index = 0

        for name in names:
            name = os.path.join(dir, name)
            size = os.path.getsize(name)

            with open(name, "rb") as file:
                data = bytearray(file.read())
                bss = struct.unpack_from(">I", data, 0x18)[0]
                struct.pack_into(">I", data, 0x18, 0)
                fbin.write(data)

            offset = (index * 8) + (4 * 4)
            if bss == 0: bss = struct.unpack_from(">2I", ftab, offset)[1]
            if bss == 0xFFFFFFFF: bss = 0

            struct.pack_into(">2I", ftab, offset, pos, bss)
            pos += size

            index += 1

        offset = (index * 8) + (4 * 4)
        struct.pack_into(">2I", ftab, offset, pos, bss)

        open(tab, "wb").write(ftab)
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

            if next == 0xFFFFFFFF: break

            name = "%d.dll" % i
            name = os.path.join(dir, name)

            data = bin[offset:next]
            open(name, "wb").write(data)
            i += 1

def main():
    import argparse

    parser = argparse.ArgumentParser(description="Dinosaur Planet DLL packer by nuggs")
    parser.add_argument("mode", type=str, help="Mode", nargs="?", choices=["pack", "unpack"], default="unpack")
    parser.add_argument("dir", type=str, help="DLL directory")
    parser.add_argument("bin", type=str, help="DLLS.bin")
    parser.add_argument("tab", type=str, help="DLLS.tab")

    if len(sys.argv) == 1:
        parser.print_help(sys.stderr)
        sys.exit(1)

    args = parser.parse_args()
    dll = dino_dll()

    if args.mode == "pack":
        dll.pack(args.bin, args.tab, args.dir)
        print("DLL packing complete.");
    if args.mode == "unpack":
        dll.unpack(args.bin, args.tab, args.dir)
        print("DLL unpacking complete.");

if __name__ == "__main__":
    main()
