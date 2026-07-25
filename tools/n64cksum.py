#!/usr/bin/env python3

import sys
import struct

# Original code from: https://gist.github.com/dkosmari/ee7bb471ea12c21b008d0ecffebd6384
# Modified to not print

# tool to recalculate N64 rom checksums
# reference code:
# https://github.com/queueRAM/sm64tools/blob/master/n64cksum.c
# https://github.com/queueRAM/sm64tools/blob/master/libsm64.c

mask32 = 0xffffffff


def read_u32_be(buffer : bytearray, offset):
    return struct.unpack_from(">I", buffer, offset)[0]


def write_u32_be(buffer : bytearray, offset, value):
    struct.pack_into(">I", buffer, offset, value)


def sm64_calc_checksums(buf : bytearray):
    #local t0, t1, t2, t3, t4, t5, t6, t7, t8, t9
    #local s0, s6
    #local a0, a1, a2, a3, at
    #local lo
    #local v0, v1
    #local ra

    # derived from the SM64 boot code
    s6 = 0x3f
    a0 = 0x1000                     # 59c:   8d640008    lw a0,8(t3)
    a1 = s6                         # 5a0:   02c02825    move  a1,s6
    at = 0x5d588b65                 # 5a4:   3c015d58    lui   at,0x5d58
                                    # 5a8:   34218b65    ori   at,at,0x8b65
    lo = (a1 * at) & mask32         # 5ac:   00a10019    multu a1,at    16 F8CA 4DDB

    ra = 0x100000                   # 5bc:  3c1f0010    lui   ra,0x10
    v1 = 0                          # 5c0:  00001825    move  v1,zero
    t0 = 0                          # 5c4:  00004025    move  t0,zero
    t1 = a0                         # 5c8:  00804825    move  t1,a0
    t5 = 32                         # 5cc:  240d0020    li t5,32
    v0 = lo                         # 5d0:  00001012    mflo  v0
    v0 = (v0 + 1) & mask32          # 5d4:  24420001    addiu v0,v0,1
    a3 = v0                         # 5d8:  00403825    move  a3,v0
    t2 = v0                         # 5dc:  00405025    move  t2,v0
    t3 = v0                         # 5e0:  00405825    move  t3,v0
    s0 = v0                         # 5e4:  00408025    move  s0,v0
    a2 = v0                         # 5e8:  00403025    move  a2,v0
    t4 = v0                         # 5ec:  00406025    move  t4,v0

    while t0 != ra:
        v0 = read_u32_be(buf, t1)   # 5f0: 8d220000    lw v0,0(t1)
        v1 = (a3 + v0) & mask32     # 5f4: 00e21821    addu  v1,a3,v0
        at = v1 < a3                # 5f8: 0067082b    sltu  at,v1,a3
        a1 = v1                     # 600: 00602825    move  a1,v1 branch delay slot
        if at:                      # 5fc: 10200002    beqz  at,0x608
            t2 = (t2 + 1) & mask32  # 604: 254a0001    addiu t2,t2,1

        v1 = v0 & 0x1F              # 608: 3043001f    andi  v1,v0,0x1f
        t7 = (t5 - v1) & mask32     # 60c: 01a37823    subu  t7,t5,v1
        t8 = v0 >> t7               # 610: 01e2c006    srlv  t8,v0,t7
        t6 = (v0 << v1) & mask32    # 614: 00627004    sllv  t6,v0,v1
        a0 = t6 | t8                # 618: 01d82025    or a0,t6,t8
        at = a2 < v0                # 61c: 00c2082b    sltu  at,a2,v0
        a3 = a1                     # 620: 00a03825    move  a3,a1
        t3 = (t3 ^ v0) & mask32     # 624: 01625826    xor   t3,t3,v0
        s0 = (s0 + a0) & mask32     # 62c: 02048021    addu  s0,s0,a0 branch delay slot
        if at:                      # 628: 10200004    beqz  at,0x63c
            t9 = (a3 ^ v0) & mask32 # 630: 00e2c826    xor   t9,a3,v0
                                    # 634: 10000002    b  0x640
            a2 = (a2 ^ t9) & mask32 # 638: 03263026    xor   a2,t9,a2 branch delay
        else:
            a2 = (a2 ^ a0) & mask32 # 63c: 00c43026    xor   a2,a2,a0

        t0 += 4                     # 640: 25080004    addiu t0,t0,4
        t7 = (v0 ^ s0) & mask32     # 644: 00507826    xor   t7,v0,s0
        t1 += 4                     # 648: 25290004    addiu t1,t1,4
        t4 = (t4 + t7) & mask32     # 650: 01ec6021    addu  t4,t7,t4 branch delay
                                    # 64c: 151fffe8    bne   t0,ra,0x5f0

    t6 = (a3 ^ t2) & mask32         # 654: 00ea7026    xor   t6,a3,t2
    a3 = (t6 ^ t3) & mask32         # 658: 01cb3826    xor   a3,t6,t3
    t8 = (s0 ^ a2) & mask32         # 65c: 0206c026    xor   t8,s0,a2
    s0 = (t8 ^ t4) & mask32         # 660: 030c8026    xor   s0,t8,t4

    return a3, s0


def sm64_update_checksums(buf: bytearray):
    cksum_offsets = [0x10, 0x14]
    read_cksum = [0, 0]
    calc_cksum = [0, 0]

    # assume CIC-NUS-6102
    # print("BootChip: CIC-NUS-6102");

    # calculate new N64 header checksum
    calc_cksum = sm64_calc_checksums(buf)

    # mimic the n64sums output
    for i in range(2):
        read_cksum[i] = read_u32_be(buf, cksum_offsets[i])
        # print("CRC{}: 0x{:08X} ".format(i+1, read_cksum[i]), end="")
        # print("Calculated: 0x{:08X} ".format(calc_cksum[i]), end="")

    # write checksums into header
    # print("Writing back calculated Checksum")
    write_u32_be(buf, cksum_offsets[0], calc_cksum[0])
    write_u32_be(buf, cksum_offsets[1], calc_cksum[1])



def print_usage():
    print("TODO: show help")


def read_file(fname):
    with open(fname, "rb") as f:
        return bytearray(f.read())


def write_file(fname, data):
    with open(fname, "wb") as f:
        f.write(data)




if len(sys.argv) < 2:
    print_usage()
    sys.exit(1)


input_name = sys.argv[1]
output_name = input_name
if len(sys.argv) > 2:
    output_name = sys.argv[2]


rom_data = read_file(input_name)

sm64_update_checksums(rom_data)

write_file(output_name, rom_data)