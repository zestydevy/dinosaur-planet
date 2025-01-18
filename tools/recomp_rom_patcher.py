import argparse
from io import BufferedRandom
import os
import shutil

from dino.dll_tab import DLLTab
from dino.dll import DLL

DLLS_ROM_OFFSET = 0x38317CC
DLLS_ROM_SIZE = 0x2D3410
DLLS_TAB_ROM_OFFSET = 0x3B04BDC
DLLS_TAB_ROM_SIZE = 0x1900

# DLLs in dino planet don't use a standard MIPS $gp prologue and this messes up recomp,
# which is expecting standard code when it applies relocations to the prologue. This patches
# the prologue into something that works with recomp without affecting functionality.
def patch_dll_gp_initializers(patched_file: BufferedRandom, dlls: list[tuple[DLL, int]]):
    # The expected non-standard $gp prologue that rare uses for dino's DLLs
    dino_gp_prologue_expected = bytearray([
        0x3c, 0x1c, 0x00, 0x00, # lui $gp, 0x0
        0x37, 0x9c, 0x00, 0x00, # ori $gp, $gp, 0x0
        0x00, 0x00, 0x00, 0x00, # nop
    ])

    # Byte patch to turn the ori into an addiu
    addiu_start_byte = bytearray([0x27])

    for (dll, offset) in dlls:
        header_size = dll.header.size
        for reloc in dll.reloc_table.gp_relocations:
            patched_file.seek(DLLS_ROM_OFFSET + offset + header_size + reloc, os.SEEK_SET)
            assert(patched_file.read(len(dino_gp_prologue_expected)) == dino_gp_prologue_expected)
            patched_file.seek(-len(dino_gp_prologue_expected) + 4, os.SEEK_CUR)
            patched_file.write(addiu_start_byte)

def patch(patched_file: BufferedRandom):
    # Parse DLL headers
    dlls: list[tuple[DLL, int]] = []

    patched_file.seek(DLLS_TAB_ROM_OFFSET, os.SEEK_SET)
    dlls_tab = DLLTab.parse(patched_file.read(DLLS_TAB_ROM_SIZE))
    number = 1
    for entry in dlls_tab.entries:
        patched_file.seek(DLLS_ROM_OFFSET + entry.start_offset, os.SEEK_SET)
        dlls.append((
            DLL.parse(
                patched_file.read(entry.size),
                str(number)
            ),
            entry.start_offset
        ))
        number += 1
    
    # Patch DLL $gp initializers
    print("Patching $gp initializers...")
    patch_dll_gp_initializers(patched_file, dlls)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", help="The original game ROM.")
    parser.add_argument("-o", "--output", help="The path/name of the patched ROM. Defaults to baserom.patched.z64.", default="baserom.patched.z64")
    args = parser.parse_args()

    # Clone base rom
    shutil.copyfile(args.rom, args.output)

    # Patch
    with open(args.output, "r+b") as patched_file:
        patch(patched_file)

    print("Done.")

if __name__ == "__main__":
    main()
