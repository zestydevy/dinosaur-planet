import sys
import shutil

"""
patchmips3 - overwrites a byte in an -mips3 object file which makes it identify
as a -mips2 object file, which allows it to be linked with the rest of the
project.

This is required become some libultra files must be compiled with -mips3.
"""

shutil.copyfile(sys.argv[1], sys.argv[2])

fd = open(sys.argv[2], 'rb+')
fd.seek(0x24)
fd.write(bytes([0x10]))
fd.close()
