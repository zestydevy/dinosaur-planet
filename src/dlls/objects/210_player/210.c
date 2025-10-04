#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/print.h"

#include "dll.h"
#include "dlls/objects/210_player.h"

#include "prevent_bss_reordering.h"

s32 func_80023D30(Object*,s32,f32,s32);
void func_80024E50(Object*, s16, f32, s32);
s32 func_80025140(Object*, f32, f32, s32);

/*0x0*/ static const u32 _rodata_0[] = {
    0x40020100, 0x00000000, 0x00000000, 0x00000000, 0x40030102, 0x00000000, 0x00000000, 0x00000000, 
    0x40040506, 0x00000000, 0x00000000, 0x00000000, 0x40060507, 0x00000000, 0x00000000, 0x00000000, 
    0x40060302, 0x00000000, 0x00000000, 0x00000000, 0x40030607, 0x00000000, 0x00000000, 0x00000000, 
    0x40030701, 0x00000000, 0x00000000, 0x00000000, 0x40070501, 0x00000000, 0x00000000, 0x00000000, 
    0x40040001, 0x00000000, 0x00000000, 0x00000000, 0x40040105, 0x00000000, 0x00000000, 0x00000000, 
    0x40000402, 0x00000000, 0x00000000, 0x00000000, 0x40020406, 0x00000000, 0x00000000, 0x00000000
};
/*0xC0*/ static const u32 _rodata_C0[] = {
    0xc1680000, 0x41a00000, 0xc1680000, 0x41680000, 0x41a00000, 0xc1680000, 0xc1680000, 0x41a00000, 
    0x41680000, 0x41680000, 0x41a00000, 0x41680000
};
/*0xF0*/ static const u32 _rodata_F0[] = {
    0xc1680000, 0xc0e00000, 0xc1680000, 0x41680000, 0xc0e00000, 0xc1680000, 0xc1680000, 0xc0e00000, 
    0x41680000, 0x41680000, 0xc0e00000, 0x41680000
};
/*0x120*/ static const u32 _rodata_120[] = {
    0x7a627566, 0x74202566, 0x2025660a, 0x00000000, 0x574f424a, 0x20636861, 0x6e67650a, 0x00000000, 
    0x574f424a, 0x20636861, 0x6e676520, 0x320a0000, 0x20484944, 0x44454e20, 0x00000000, 0x554e4b4e, 
    0x4f574e20, 0x6d61696e, 0x47657420, 0x56414c55, 0x45202564, 0x0a000000, 0x20466972, 0x65696e67, 
    0x204f626a, 0x65637420, 0x4d696e64, 0x20526561, 0x64204269, 0x74202569, 0x20000000, 0x204d696e, 
    0x64205245, 0x41442053, 0x7065656c, 0x6c200000, 0x20486176, 0x65204c6f, 0x636b2000, 0x20536574, 
    0x696e6720, 0x53747566, 0x66200000, 0x20616e67, 0x20256920, 0x00000000, 0x20506f72, 0x74616c20, 
    0x52454144, 0x20537065, 0x656c6c20, 0x00000000, 0x20486176, 0x65204c6f, 0x636b2000, 0x20536574, 
    0x696e6720, 0x53747566, 0x66200000, 0x20616e67, 0x20256920, 0x00000000, 0x706f7765, 0x72202564, 
    0x0a000000, 0x20466972, 0x696e6720, 0x4d6f6465, 0x6c206678, 0x20000000, 0x20686974, 0x20736f6d, 
    0x6574696d, 0x65200000
};

/*0x0*/ static u32 _data_0[] = {
    0x00000000
};
/*0x4*/ static u32 _data_4[] = {
    0x3ecccccd
};
/*0x8*/ static u32 _data_8[] = {
    0x3f19999a
};
/*0xC*/ static f32 _data_C[] = {
    0, 0
};
static u8 _data_14[4] = {0, 0, 0, 0};
static s16 _data_18[2] = {-1, 0};
/*0x1C*/ static u32 _data_1C[] = {
    0x00000000
};
/*0x20*/ static u32 _data_20[] = {
    0x01020000
};
/*0x24*/ static u32 _data_24[] = {
    0x00690023
};
/*0x28*/ static u32 _data_28[] = {
    0xc8aaa078, 0x64000000
};
/*0x30*/ static u32 _data_30[] = {
    0x0a000000, 0x00000000
};
/*0x38*/ static u32 _data_38[] = {
    0x00000000
};
/*0x3C*/ static u32 _data_3C[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41880000, 0x00000000, 0x00000000, 0x00000000, 
    0xc1200000, 0x00000000, 0x00000000, 0x00000000
};
/*0x6C*/ static u32 _data_6C[] = {
    0x00000000
};
/*0x70*/ static u32 _data_70[] = {
    0x41080000, 0x00000000, 0x00000000
};
/*0x7C*/ static u32 _data_7C[] = {
    0x00000000, 0x41200000, 0x00000000
};
/*0x88*/ static u32 _data_88[] = {
    0x41080000, 0x41780000
};
/*0x90*/ static u32 _data_90[] = {
    0x004c0000
};
/*0x94*/ static u32 _data_94[] = {
    0x3bd4fdf4
};
//Possibly modanim indices? (s8 bank + s8 index as a s16)
/*0x98*/ static s16 _data_98[] = {
    0x0000, 0x0000, 0x0000, 0x0016, 0x0020, 0x001f, 0x0001, 0x0020, 0x001f, 0x0002, 0x0411, 0x0410,
    0x0003, 0x0024, 0x0023, 0x0003, 0x0024, 0x0023, 0x001e, 0x0045, 0x0043, 0x0004, 0x0044, 0x0000
};
//Possibly modanim indices for walking/running with weapon drawn? (s8 bank + s8 index as a s16)
/*0xC8*/ static s16 _data_C8[] = {
    0x0430, 0x0430, 0x0430, 0x0431, 0x0433, 0x0432, 0x0434, 0x0433, 0x0432, 0x0437, 0x0439, 0x0438, 
    0x043a, 0x043c, 0x043b, 0x043a, 0x043c, 0x043b, 0x001e, 0x0045, 0x0043, 0x0004, 0x0044, 0x0000
};
//Possibly modanim indices? (s8 bank + s8 index as a s16)
/*0xF8*/ static s16 _data_F8[] = {
    0x0008, 0x0008, 0x0008, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x0007, 0x041b, 0x041b, 0x041b, 
    0x041b, 0x041b, 0x041b, 0x041b, 0x041b, 0x041b, 0x0445, 0x0445, 0x0427, 0x0442, 0x0444, 0x0000
};
/*0x128*/ static u32 _data_128[] = {
    0x00890089, 0x00890089, 0x008c008d, 0x008e008f, 0x00900091, 0x00920093
};
/*0x140*/ static u32 _data_140[] = {
    0x00890089, 0x00890089, 0x00f900fa, 0x00fb00fc, 0x00fd00fe, 0x00ff0400
};
/*0x158*/ static u32 _data_158[] = {
    0x001700c6, 0x00180019, 0x001a00be, 0x00c000bf, 0x00ca00cb, 0xffffffff
};
/*0x170*/ static u32 _data_170[] = {
    0x00f500f5, 0x00f500f5, 0x00f500f5, 0x00f300f4, 0x00f600f7, 0xffffffff
};
/*0x188*/ static u32 _data_188[] = {
    0x001b001d, 0x04530454
};
/*0x190*/ static u32 _data_190[] = {
    0x00990099, 0x0000000b, 0x01ff0000, 0x3cac0831, 0xbf800000, 0xbf800000, 0xbf800000, 0x3ea8f5c3, 
    0x3ee147ae, 0x3e99999a, 0x3eb33333, 0x00e60001, 0x00000000, 0x00000000, 0x00000000, 0x009a009a, 
    0x0000000b, 0x01020000, 0x3cbc6a7f, 0x3dcccccd, 0x3f000000, 0x3f000000, 0x3e6147ae, 0x3ecccccd, 
    0x3eb33333, 0x3ee66666, 0x00e30001, 0x00000000, 0x00000000, 0x00000000, 0x009b009b, 0x0000000a, 
    0x01ff0000, 0x3ca3d70a, 0xbf800000, 0xbf800000, 0xbf800000, 0x3e3851ec, 0x3ecccccd, 0x3e851eb8, 
    0x3eb851ec, 0x00e60001, 0x00000000, 0x00000000, 0x00000000, 0x009b009b, 0x0000000a, 0x01040000, 
    0x3ca3d70a, 0x3dcccccd, 0x3f000000, 0x3f000000, 0x3e3851ec, 0x3ecccccd, 0x3e851eb8, 0x3eb851ec, 
    0x00e60001, 0x00000000, 0x00000000, 0x00000000, 0x009a009a, 0x0000000b, 0x01ff0000, 0x3cbc6a7f, 
    0xbf800000, 0xbf800000, 0xbf800000, 0x3e6147ae, 0x3ecccccd, 0x3eb33333, 0x3ee66666, 0x00e30001, 
    0x00000000, 0x00000000, 0x00000000, 0x00960096, 0x0000000a, 0x01ff0000, 0x3c8b4396, 0xbf800000, 
    0xbf800000, 0xbf800000, 0x3e4ccccd, 0x3f333333, 0x3eb33333, 0x3eeb851f, 0x00e50001, 0x00000000, 
    0x00000000, 0x00000000
};
/*0x2F8*/ static u32 _data_2F8[] = {
    0x00990099, 0x0000000b, 0x01ff0000, 0x3ccccccd, 0xbf800000, 0xbf800000, 0xbf800000, 0x3e23d70a, 
    0x3ecccccd, 0x3e800000, 0x3eb33333, 0x06610000, 0x00000000, 0x00000000, 0x00000000, 0x009a009a, 
    0x0000000c, 0x01020000, 0x3ccccccd, 0x3dcccccd, 0x3f000000, 0x3f000000, 0x3e4ccccd, 0x3ee66666, 
    0x3e800000, 0x3e99999a, 0x06620000, 0x00000000, 0x00000000, 0x00000000, 0x009b009b, 0x0000000a, 
    0x01ff0000, 0x3cd4fdf4, 0xbf800000, 0xbf800000, 0xbf800000, 0x3e19999a, 0x3eb851ec, 0x3e851eb8, 
    0x3eb851ec, 0x06630000, 0x00000000, 0x00000000, 0x00000000, 0x009b009b, 0x0000000a, 0x01040000, 
    0x3cd4fdf4, 0x3dcccccd, 0x3f000000, 0x3f000000, 0x3e19999a, 0x3eb851ec, 0x3e851eb8, 0x3eb851ec, 
    0x06630000, 0x00000000, 0x00000000, 0x00000000, 0x009a009a, 0x0000000c, 0x01ff0000, 0x3ccccccd, 
    0xbf800000, 0xbf800000, 0xbf800000, 0x3e4ccccd, 0x3ee66666, 0x3e800000, 0x3e99999a, 0x06620000, 
    0x00000000, 0x00000000, 0x00000000, 0x00960096, 0x0000000a, 0x02ff0000, 0x3c83126f, 0xbf800000, 
    0xbf800000, 0xbf800000, 0x3ecccccd, 0x3f19999a, 0x3eb33333, 0x3eeb851f, 0x06610000, 0x00000000, 
    0x00000000, 0x00000000
};
/*0x460*/ static u32 _data_460[] = {
    0x00b600b7, 0x00b80675, 0x06760370, 0x00df00e0, 0x0b100b0f, 0x0a790a7a, 0x0a7b0a7c, 0x0a7d07aa, 
    0x07ab07ac, 0x07ad00d9, 0x00da00db, 0x0b120b14
};
/*0x490*/ static u32 _data_490[] = {
    0x067a067b, 0x067c0675, 0x06760664, 0x06650666, 0x066707a9, 0x06a606a7, 0x06a806a9, 0x06aa07aa, 
    0x07ab07ac, 0x07ad066b, 0x066c066d, 0x0b110b13
};
/*0x4C0*/ static u32 _data_4C0[] = {
    0x00e900ea, 0x00ec00ed, 0x00eb00ee
};
/*0x4CC*/ static u32 _data_4CC[] = {
    0x00200021, 0x00230024, 0x00220025
};
/*0x4D8*/ static u32 _data_4D8[] = {
    0x0b220b1e, 0x0b220000
};
/*0x4E0*/ static u32 _data_4E0[] = {
    0x002d0040, 0x01d705bd, 0x05ce05fc, 0x07770000
};
/*0x4F0*/ static u32 _data_4F0[] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005
};
/*0x508*/ static u32 _data_508[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff
};
/*0x518*/ static u32 _data_518[] = {
    0x00240025, 0x018c0024, 0x00240024, 0x00240024
};
/*0x528*/ static u32 _data_528[] = {
    0x00000000
};
/*0x52C*/ static u32 _data_52C[] = {
    0x00000000
};
/*0x530*/ static u8 _data_530 = 0x01;
/*0x534*/ static u32 _data_534[] = {
    0x0b04060a, 0x0a03030c, 0x05020e0f, 0x10111200
};
/*0x544*/ static u32 _data_544[] = {
    0x00010002, 0x00040008, 0x00080010, 0x00100020, 0x00400080, 0x01000200, 0x04000080, 0x00010000
};
/*0x564*/ static u32 _data_564[] = {
    0x000e0051
};
/*0x568*/ static u32 _data_568[] = {
    0x000f0010, 0x00460046, 0x00470014
};
/*0x574*/ static u32 _data_574[] = {
    0x00150021, 0x000d0022
};
/*0x57C*/ static u32 _data_57C[] = {
    0x004f0050, 0x005200eb, 0x005300ec, 0x005500e7, 0x005600e8, 0x005800e9, 0x005900ea, 0x04140414, 
    0x04150415
};
/*0x5A0*/ static u32 _data_5A0[] = {
    0x002d002e, 0x00380039, 0x002f0030, 0x00310032
};
/*0x5B0*/ static u32 _data_5B0[] = {
    0x00250029, 0x00330034
};
/*0x5B8*/ static u32 _data_5B8[] = {
    0x003a003e
};
/*0x5BC*/ static u32 _data_5BC[] = {
    0x006e00b6, 0x00ba00b8, 0x006f00b7, 0x00b900b5, 0x00bb0068, 0x006c006a, 0x00bc006b, 0x006d0069
};
/*0x5DC*/ static u32 _data_5DC[] = {
    0x00660071
};
/*0x5E0*/ static u32 _data_5E0[] = {
    0x00720403
};
/*0x5E4*/ static u32 _data_5E4[] = {
    0x04040070
};
/*0x5E8*/ static u32 _data_5E8[] = {
    0x00bd0000, 0x007700e4, 0x007500e3, 0x00d100e6, 0x00cc00e5, 0x00770075, 0x0078007e, 0x0076007d, 
    0x007b007c, 0x0079007a, 0x007f0080, 0x00840085, 0x00810082, 0x00860087, 0x0083007f, 0x008000e4, 
    0x00e30000, 0x00d100cc, 0x00d200d3, 0x00cd00ce, 0x00d800d9, 0x00d600d7, 0x00da00db, 0x00df00e0, 
    0x00dc00dd, 0x00e100e2, 0x00de00da, 0x00db00e6, 0x00e500d4, 0x00d40000, 0x3f800000, 0x3f800000, 
    0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000, 
    0x3f800000, 0x40400000, 0x3f800000, 0x40400000, 0xbf800000
};
/*0x69C*/ static u32 _data_69C[] = {
    0x000e0010, 0x005c0047, 0x00140015, 0x0021005c, 0x005d0000
};
/*0x6B0*/ static u32 _data_6B0[] = {
    0x005a005b, 0x005a005b, 0x005c005d, 0x00140014
};
/*0x6C0*/ static u32 _data_6C0[] = {
    0x005a005b, 0x00140014
};
/*0x6C8*/ static u32 _data_6C8[] = {
    0x005a005b, 0x001c001d
};
/*0x6D0*/ static u32 _data_6D0[] = {
    0x00250026, 0x00270028, 0x0029002a, 0x002b002c
};
/*0x6E0*/ static u32 _data_6E0[] = {
    0x003a003b, 0x003c003d, 0x003e003f, 0x00400041
};
/*0x6F0*/ static u32 _data_6F0[] = {
    0x00660067
};
/*0x6F4*/ static u32 _data_6F4[] = {
    0x00ed00ee
};
/*0x6F8*/ static f32 _data_6F8 = {
    0.005
};
/*0x6FC*/ static u32 _data_6FC[] = {
    0x3d4ccccd, 0x3cf5c28f, 0x3f0a3d71, 0x3f051eb8, 0x3f70a3d7, 0x3f6b851f, 0x3fa8f5c3, 0x3fa66666, 
    0x3fd33333, 0x3ba3d70a
};
/*0x724*/ static u32 _data_724[] = {
    0x3d4ccccd
};
/*0x728*/ static u32 _data_728[] = {
    0x3cf5c28f, 0x3f0a3d71, 0x3f051eb8, 0x3f70a3d7, 0x3f6b851f, 0x3fa8f5c3, 0x3fa66666, 0x3fd33333
};
/*0x748*/ static u32 _data_748[] = {
    0x3ba3d70a, 0x3d4ccccd, 0x3cf5c28f, 0x3f0ccccd, 0x3f07ae14, 0x3fd33333
};
/*0x760*/ static u32 _data_760[] = {
    0x3ba3d70a, 0x3d4ccccd, 0x3cf5c28f, 0x3f0ccccd, 0x3f07ae14, 0x3fd33333
};
/*0x778*/ static u32 _data_778[] = {
    0x00000166, 0x00000167, 0x00000256, 0x0000036e, 0x0000037f, 0x00000380, 0x00000381, 0x00000543, 
    0x00000544, 0x00000545, 0x00000546, 0x0000012e, 0x00000169, 0x000001d0, 0x000001d6, 0x000001ed, 
    0x000001fd
};
/*0x7BC*/ static u32 _data_7BC[] = {
    0x00000000
};
/*0x7C0*/ static u32 _data_7C0[] = {
    0x00000000
};
/*0x7C4*/ static u32 _data_7C4[] = {
    0x00000000
};
/*0x7C8*/ static u32 _data_7C8[] = {
    0x40404040, 0x40404040, 0xff000000
};
/*0x7D4*/ static u32 _data_7D4[] = {
    0x03030303, 0x03030303, 0x08000000
};

/*0x0*/ static u8 _bss_0[0x2];
/*0x2*/ static u8 _bss_2[0x2];
/*0x4*/ static u8 _bss_4[0x4];
/*0x8*/ static u8 _bss_8[0x8];
/*0x10*/ static u8 _bss_10[0x4];
/*0x14*/ static u8 _bss_14[0x4];
/*0x18*/ static u8 _bss_18[0x4];
/*0x1C*/ static u8 _bss_1C[0x4];
/*0x20*/ static u8 _bss_20[0x4];
/*0x24*/ static u8 _bss_24[0x4];
/*0x28*/ static u8 _bss_28[0x4];
/*0x2C*/ static u8 _bss_2C[0x4];
/*0x30*/ static u8 _bss_30[0x4];
/*0x34*/ static u8 _bss_34[0x4];
/*0x38*/ static u8 _bss_38[0x4];
/*0x3C*/ static u8 _bss_3C[0x4];
/*0x40*/ static u8 _bss_40[0xc];
/*0x4C*/ static u8 _bss_4C[0x4];
/*0x50*/ static u8 _bss_50[0x4];
/*0x54*/ static u8 _bss_54[0x4];
/*0x58*/ static u8 _bss_58[0x8];
/*0x60*/ static u8 _bss_60[0x13c];
/*0x19C*/ static u8 _bss_19C[0x4];
/*0x1A0*/ static u8 _bss_1A0[0x4];
/*0x1A4*/ static u8 _bss_1A4[0x4];
/*0x1A8*/ static u8 _bss_1A8[0x2];
/*0x1AA*/ static u8 _bss_1AA[0x2];
/*0x1AC*/ static u8 _bss_1AC[0x4];
/*0x1B0*/ static u8 _bss_1B0[0x4];
/*0x1B4*/ static u8 _bss_1B4[0x4];
/*0x1B8*/ static u8 _bss_1B8[0x4];
/*0x1BC*/ static u8 _bss_1BC[0x4];
/*0x1C0*/ static u8 _bss_1C0[0x8];
/*0x1C8*/ static u8 _bss_1C8[0x8];
/*0x1D0*/ static u8 _bss_1D0[0xc];
/*0x1DC*/ static u8 _bss_1DC[0x4];
/*0x1E0*/ static u8 _bss_1E0[0x18];
/*0x1F8*/ static u8 _bss_1F8[0x8];
/*0x200*/ static u8 _bss_200[0x2];
/*0x202*/ static u8 _bss_202[0x2];
/*0x204*/ static u8 _bss_204[0x4];
/*0x208*/ static u8 _bss_208[0x4];
/*0x20C*/ static u8 _bss_20C[0x4];
/*0x210*/ static u8 _bss_210[0xc];
/*0x21C*/ static u8 _bss_21C[0x4];
/*0x220*/ static u8 _bss_220[0x4];
/*0x224*/ static u8 _bss_224[0x4];
/*0x228*/ static u8 _bss_228[0x8];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_0.s")

// offset: 0x3F8 | ctor
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_ctor.s")
#else
static void dll_210_func_0();

void dll_210_ctor(s32 arg0) {
    dll_210_func_0();
}
#endif

// offset: 0x438 | dtor
void dll_210_dtor(s32 arg0) {
}

// offset: 0x444 | func: 1 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_444.s")

// offset: 0x934 | func: 2 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_934.s")

// offset: 0x11A0 | func: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_11A0.s")

// offset: 0x1BC0 | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1BC0.s")

// offset: 0x1CA8 | func: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CA8.s")

// offset: 0x1DDC | func: 6
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1DDC.s")
#else
// Matches but requires dll_210_func_24FC, dll_210_func_6DD8 as static
void dll_210_func_1DDC(Object* arg0, PlayerState* arg1, PlayerState* arg2) {
    Object* sp8C;
    s32 sp88;
    f32 temp_fv0;
    u32 message;
    f32 var_fs0;
    f32 var_fs1;
    s32 temp_v0;

    sp88 = NULL;
    while (obj_recv_mesg(arg0, &message, &sp8C, (void **)&sp88)) {
        switch (message) {
        case 0x80002:
            if (sp88 == 0x2D) {
                if (dll_210_func_24FC(arg0, arg2) != 0) {
                    temp_v0 = gDLL_2_Camera->vtbl->func3();
                    if ((temp_v0 != 0x64) && (temp_v0 != 0x5E)) {
                        gDLL_2_Camera->vtbl->func6(0x64, 1, 0, 0, NULL, 0x3C, 0xFF);
                        gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 0x3A);
                        arg1->flags |= 0x400000;
                    }
                } else {
                    gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x912U, 0x7FU, NULL, NULL, 0, NULL);
                }
            } else {
                dll_210_func_6DD8(arg0, arg1, sp88);
            }
            break;
        case 0xE0000:
            if (sp8C == arg2->unk2C8) {
                arg2->unk2C8 = 0;
                arg2->unk33D = 0;
                gDLL_2_Camera->vtbl->func17.withOneArg(0);
            }
            break;
        case 0x60003:
            var_fs0 = sp8C->srt.transl.x - arg0->srt.transl.x;
            var_fs1 = sp8C->srt.transl.z - arg0->srt.transl.z;
            temp_fv0 = sqrtf(SQ(var_fs0) + SQ(var_fs1));
            if (temp_fv0 > 1.0f) {
                var_fs0 /= temp_fv0;
                var_fs1 /= temp_fv0;
            }
            arg0->speed.y = 2.5f;
            arg0->speed.x = var_fs0 * 2.5f;
            arg0->speed.z = var_fs1 * 2.5f;
            gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 0x4D);
            dll_210_func_1CD6C(arg0, -sp88);
            if (arg1->unk868 != NULL) {
                arg1->unk868->unk_0xe0 = 0;
                arg1->unk868 = NULL;
            }
            break;
        case 0x60004:
            var_fs0 = sp8C->srt.transl.x - arg0->srt.transl.x;
            var_fs1 = sp8C->srt.transl.z - arg0->srt.transl.z;
            temp_fv0 = sqrtf((var_fs0 * var_fs0) + (var_fs1 * var_fs1));
            if (temp_fv0 > 1.0f) {
                var_fs0 /= temp_fv0;
                var_fs1 /= temp_fv0;
            }
            arg0->speed.y = 2.5f;
            arg0->speed.x = -var_fs0 * 2.5f;
            arg0->speed.z = -var_fs1 * 2.5f;
            gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 0x4D);
            dll_210_func_1CD6C(arg0, -sp88);
            if (arg1->unk868 != NULL) {
                arg1->unk868->unk_0xe0 = 0;
                arg1->unk868 = NULL;
            }
            break;
        case 0x60005:
            var_fs0 = sp8C->srt.transl.x - arg0->srt.transl.x;
            var_fs1 = sp8C->srt.transl.z - arg0->srt.transl.z;
            temp_fv0 = sqrtf((var_fs0 * var_fs0) + (var_fs1 * var_fs1));
            if (temp_fv0 > 1.0f) {
                var_fs0 /= temp_fv0;
                var_fs1 /= temp_fv0;
            }
            arg0->speed.y = 2.5f;
            arg0->speed.x = -var_fs0 * 2.5f;
            arg0->speed.z = -var_fs1 * 2.5f;
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xDAU, 0x7FU, NULL, NULL, 0, NULL);
            gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 0x4D);
            func_80023D30(arg0, 0x450, 0.0f, 0);
            dll_210_func_1CD6C(arg0, -sp88);
            if (arg1->unk868 != NULL) {
                arg1->unk868->unk_0xe0 = 0;
                arg1->unk868 = NULL;
            }
            break;
        case 0x7000A:
            if (sp88 > 0) {
                if (get_gplay_bitstring(sp88) != 0) {
                    obj_send_mesg(sp8C, 0x7000BU, arg0, NULL);
                    if (arg2->unk26C != 0x2B) {
                        gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 0x2B);
                    }
                } else {
                    set_gplay_bitstring(sp88, 1U);
                    if (arg2->unk26C != 0x2A) {
                        gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 0x2A);
                    }
                }
            } else if (arg2->unk26C != 0x2A) {
                gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 0x2A);
            }
            arg1->unk708 = sp8C;
            arg1->unk70C = sp88 & 0xFFFF;
            if (arg1->unk708->ptr0x64 != NULL) {
                arg1->unk708->ptr0x64->flags = 0x20000;
            }
            arg1->unk8A9 = 1;
            break;
        case 0x100008:
            arg1->unk870 = 1;
            if (arg1->unk868 == NULL) {
                arg1->unk868 = sp8C;
                arg1->unk86C = (sp88 >> 0x10) / 10.0f;
                gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 5);
                arg1->unk8A9 = 1;
            }
            break;
        case 0x100010:
            arg1->unk870 = 1;
            if (arg1->unk868 == NULL) {
                arg1->unk868 = sp8C;
                arg1->unk86C = sp88 >> 0x10;
                gDLL_18->vtbl->func4.withThreeArgs(arg0, arg2, 5);
                arg1->unk8A9 = 1;
            }
            break;
        }
    }
}
#endif

// offset: 0x24FC | func: 7
int dll_210_func_24FC(Object *player, PlayerState *state){
  return state->unk26C == 1 || state->unk26C == 3 || state->unk26C == 4;
}

// offset: 0x2534 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_2534.s")

// offset: 0x2C7C | func: 9 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_2C7C.s")

// offset: 0x307C | func: 10 | export: 3
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_307C.s")
#else

void draw_object(Object *obj, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, f32 yPrescale);
s32 func_80031F6C(Object*, s32, f32*, f32*, f32*, s32);
void func_80036FBC(u8,u8,u8,u8);
void func_8004E64C(Object* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
static void dll_210_func_363C(Object* arg0, PlayerState* arg1, Gfx** arg2, Mtx** arg3, Vertex** arg4) {} /* extern */
static void dll_210_func_3B40(Object* arg0, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4) {} /* extern */
static void dll_210_func_8EA4(Object* arg0, PlayerState* arg1, Object* arg2, Gfx** arg3, Mtx** arg4, Vertex** arg5, Triangle** arg6, s32 arg7) {} /* extern */
void func_80032238(Object* obj, s32 arg1, s32 arg2, Vec3f* arg3);              /* extern */
void func_80034FF0(s32);                                 /* extern */
void func_80035AF4(Gfx**, Mtx**, Vertex**, Triangle**, Object*, void*, s32, s32, void*, s32, s32); /* extern */

void dll_210_func_307C(Object* arg0, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4, s8 arg5) {
    Object* sp8C;
    s32 pad;
    PlayerState* state;
    ModelInstance* sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    s32 pad2;
    Object* temp_a2;

    state = arg0->state;
    sp80 = arg0->modelInsts[arg0->modelInstIdx];
    if (arg5 == -1 || !(state->flags & 0x4001)) {
        if ((state->unk858 != NULL) && ((arg0->unk0xb0 & 0x1000) || state->unk26C == 0x24 || state->unk26C == 0x25)) {
            dll_210_func_8EA4(arg0, state, state->unk858, arg1, arg2, arg3, arg4, 1);
        }
        if (state->unk8BE == 1) {
            dll_210_func_3B40(arg0, arg1, arg2, arg3, arg4);
        }
        gDLL_16->vtbl->func1(arg0);
        temp_a2 = state->unk858;
        if (temp_a2 != 0 && ((arg0->unk0xb0 & 0x1000) || state->unk26C == 0x24 || state->unk26C == 0x25)) {
            ((DLL_Unknown*)temp_a2->dll)->vtbl->func[19].withThreeArgs(temp_a2, ((s32*)arg0->def)[1], temp_a2);
        }
        if (state->unk818 > 0.0f) {
            func_80036FBC(0xC8U, 0U, 0U, state->unk81C);
        }
        arg0->srt.transl.y += state->unk83C;
        draw_object(arg0, arg1, arg2, arg3, arg4, 1.0f);
        arg0->srt.transl.y -= state->unk83C;
        if (state->unk858 != 0 && ((arg0->unk0xb0 & 0x1000) || state->unk26C == 0x24 || state->unk26C == 0x25)) {
            func_80034FF0(0);
        }
        if (arg5 != 0) {
            dll_210_func_363C(arg0, state, arg1, arg2, arg3);
        }
        func_80032238(arg0, 4, 2, &state->unk39C);
        func_80031F6C(arg0, 9, &state->unk7EC, &state->unk7F0, &state->unk7F4, 0);
        if (sp80->unk_0x34 & 8) {
            sp8C = state->unk850;
            if (sp8C != NULL && (state->flags & 4)) {
                sp64 = sp8C->srt.scale;
                sp8C->srt.scale /= arg0->srt.scale;
                pad = arg0->unk_0x37;
                if (pad > 0xFF) {
                    pad = 0xFF;
                }
                func_80035AF4(arg1, arg2, arg3, arg4, arg0, sp80, 0, 0, sp8C, 6, arg0->unk_0x37 > 0xFF ? 0xFF : arg0->unk_0x37);
                sp8C->srt.scale = sp64;
            }
        }
        if (state->unk26C == 0x29) {
            func_80031F6C(arg0, 7, &state->unk69C, &state->unk6A0, &state->unk6A4, 0);
        }
        if (state->unk868 != NULL && state->unk868->unk_0xe0 == 1) {
            func_80031F6C(arg0, 6, &sp7C, &sp78, &sp74, 0);
            func_80031F6C(arg0, 7, &sp70, &sp6C, &sp68, 0);
            sp7C = (sp7C + sp70) * 0.5f;
            sp78 = (sp78 + sp6C) * 0.5f;
            sp74 = (sp74 + sp68) * 0.5f;
            sp7C -= fsin16_precise(arg0->srt.yaw) * state->unk86C;
            sp74 -= fcos16_precise(arg0->srt.yaw) * state->unk86C;
            state->unk868->srt.transl.x = sp7C;
            state->unk868->srt.transl.y = sp78;
            state->unk868->srt.transl.z = sp74;
            state->unk868->srt.yaw = arg0->srt.yaw;
            state->unk868->dll->vtbl->draw(state->unk868, arg1, arg2, arg3, arg4, -1);
        }
        func_8004E64C(arg0, (s32) arg1, (s32) arg2, (s32) arg3, (s32) arg4);
    }
}
#endif

// offset: 0x363C | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_363C.s")

// offset: 0x3B40 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_3B40.s")

// offset: 0x3E50 | func: 13 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_3E50.s")

// offset: 0x3F40 | func: 14 | export: 5
s32 dll_210_func_3F40(s32 arg0) {
    return 0x1CB;
}

// offset: 0x3F50 | func: 15 | export: 6
s32 dll_210_func_3F50(s32 arg0, s32 arg1) {
    return 0x8C4;
}

// offset: 0x3F64 | func: 16 | export: 63
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_3F64.s")

// offset: 0x4038 | func: 17 | export: 64
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_4038.s")

// offset: 0x4114 | func: 18 | export: 45
s32 dll_210_func_4114(Object* player) {
    PlayerState* state = player->state;

    if (state->unk8B5 == 7) {
        return state->unk6E8;
    }
    return 0;
}

// offset: 0x413C | func: 19 | export: 46
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_413C.s")

// offset: 0x4198 | func: 20 | export: 48
s16 dll_210_func_4198(Object* player) {
    PlayerState* state = player->state;
    Object* temp_v1;

    if (state->flags & 0x10000) {
        temp_v1 = state->unk868;
        if (temp_v1 != NULL) {
            return temp_v1->id;
        }
    }
    return 0;
}

// offset: 0x41CC | func: 21 | export: 49
s16 dll_210_func_41CC(Object* player) {
    PlayerState* state = player->state;
    return !(state->flags & 0x100000);
}

// offset: 0x41F4 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_41F4.s")

// offset: 0x43EC | func: 23 | export: 65
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_43EC.s")

// offset: 0x44A4 | func: 24 | export: 66
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_44A4.s")

// offset: 0x4634 | func: 25 | export: 67
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_4634.s")

// offset: 0x47B8 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_47B8.s")

// offset: 0x4910 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_4910.s")

// offset: 0x60A8 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_60A8.s")

// offset: 0x618C | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_618C.s")

// offset: 0x63F0 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_63F0.s")

// offset: 0x64B4 | func: 31
void dll_210_func_64B4(Object* arg0, PlayerState* arg1, f32 arg2) {
    Object* temp_s2;
    s32 temp_s3;
    s32 var_s4;
    s32 var_v0;
    f32 f20;
    ModelInstance *modelInstance;
    AnimState *animState;

    temp_s2 = arg0->linkedObject;
    var_v0 = 0;
    f20 = 0.01f;
    do {
        var_s4 = 0;
        switch (arg1->unk878) {
        case 2:
            if (var_v0 != 0) {
                func_80024E50(arg0, arg0->curModAnimId, arg0->animProgress, 0);
                func_80024E50(arg0, 0x8A, 0.0f, 0);
            }
            temp_s3 = func_80025140(arg0, f20, arg2, 0);
            if (arg0->animProgressLayered > 0.24f) {
                arg1->unk8A8 = 1;
            }
            if (arg0->animProgressLayered > 0.59f) {
                arg1->unk8A8 = 2;
            }
            if ((temp_s2 != NULL) && (arg0->animProgressLayered > 0.7f) && (temp_s2->group == 0x30)) {
                ((DLL_Unknown*)temp_s2->dll)->vtbl->func[7].withOneS32OneF32((s32)temp_s2, 0.15f);
            }
            if (temp_s3 != 0) {
                arg1->unk878 = 3;
                var_s4 = 1;
            }
            break;
        case 13:
            if ((temp_s2 != NULL) && (temp_s2->group == 0x30)) {
                ((DLL_Unknown*)temp_s2->dll)->vtbl->func[7].withOneS32OneF32((s32)temp_s2, 1.0f);
            }
            arg1->unk8A8 = 2;
            arg1->unk878 = 0;
            break;
        case 1:
            if (var_v0 != 0) {
                func_80024E50(arg0, arg0->curModAnimId, arg0->animProgress, 0);
                func_80024E50(arg0, 0x8A, 0.99f, 0);
            }
            temp_s3 = func_80025140(arg0, -f20, arg2, 0);
            if (arg0->animProgressLayered < 0.59f) {
                arg1->unk8A8 = 1;
            }
            if (arg0->animProgressLayered < 0.24f) {
                arg1->unk8A8 = 0;
            }
            if ((temp_s2 != NULL) && (arg0->animProgressLayered < 0.7f) && (temp_s2->group == 0x30)) {
                ((DLL_Unknown*)temp_s2->dll)->vtbl->func[8].withOneArg((s32)temp_s2);
            }
            if (temp_s3 != 0) {
                arg1->unk87C = -1;
                arg1->unk878 = 3;
                var_s4 = 1;
            }
            break;
        case 14:
            if (temp_s2->group == 0x30) {
                ((DLL_Unknown*)temp_s2->dll)->vtbl->func[8].withOneArg((s32)temp_s2);
            }
            arg1->unk87C = -1;
            arg1->unk8A8 = 0;
            arg1->unk878 = 0;
            break;
        case 3:
            if (var_v0 != 0) {
                func_80024E50(arg0, arg0->curModAnimId, arg0->animProgress, 0);
            } else {
                func_80025140(arg0, arg0->animProgress - arg0->animProgressLayered, 1.0f, 0);
            }
            modelInstance = arg0->modelInsts[arg0->modelInstIdx];
            animState = modelInstance->animState1;
            if (animState->unk_0x58[0] == 0) {
                arg0->curModAnimIdLayered = -1;
                arg1->unk878 = 0;
            }
            break;
        default:
            if (arg0->linkedObject != NULL) {
                var_v0 = arg1->unk8A9;
                if (arg1->unk8A8 != 0) {
                    if (var_v0 == 0) {
                        arg1->unk878 = 1;
                        var_s4 = 1;
                        break;
                    }
                    if (var_v0 == 1) {
                        arg1->unk878 = 0xE;
                        var_s4 = 1;
                    }
                    break;
                }

                if (var_v0 == 2) {
                    arg1->unk878 = 2;
                    var_s4 = 1;
                    break;
                }
                if (var_v0 == 4) {
                    arg1->unk878 = 0xD;
                    var_s4 = 1;
                }
            }
            break;
        }
        var_v0 = var_s4;
    } while (var_s4 != 0);
}

// offset: 0x692C | func: 32
void dll_210_func_692C(Object* arg0, PlayerState* arg1, f32 arg2) {
    s32 var_s2;
    s32 var_v0;
    ModelInstance *modelInstance;
    AnimState *animState;
    Object *linkedObject = arg0->linkedObject;
    u8 new_var;
    u8 new_var2;
    u8 *temp;

    if (linkedObject == NULL) {
        return;
    }

    var_v0 = 0;
    do {
        temp = &_data_530;
        new_var = 1;
        var_s2 = 0;
        switch (arg1->unk878) {
        case 2:
            if (var_v0 != 0) {
                func_80024E50(arg0, arg0->curModAnimId, arg0->animProgress, 0);
                func_80024E50(arg0, 0x8A, 0.0f, 0);
            }
            if ((arg0->animProgressLayered > 0.2f) && !*temp) {
                gDLL_6_AMSFX->vtbl->play_sound(arg0, arg1->unk3B8[4], 0x7FU, NULL, NULL, 0, NULL);
                _data_530 = new_var;
            }
            if (arg0->animProgressLayered > 0.4f) {
                arg1->unk8A8 = 2;
            }
            if (func_80025140(arg0, 0.015f, arg2, 0) != 0) {
                arg1->unk878 = 3;
                var_s2 = new_var;
            }
            break;
        case 13:
            if (!*temp) {
                gDLL_6_AMSFX->vtbl->play_sound(arg0, arg1->unk3B8[4], 0x7FU, NULL, NULL, 0, NULL);
                _data_530 = new_var;
            }
            arg1->unk8A8 = 2;
            arg1->unk878 = 0;
            break;
        case 1:
            if (var_v0 != 0) {
                func_80024E50(arg0, arg0->curModAnimId, arg0->animProgress, 0);
                func_80024E50(arg0, 0x8A, 0.99f, 0);
            }
            if ((arg0->animProgressLayered < 0.8f) && !*temp) {
                gDLL_6_AMSFX->vtbl->play_sound(arg0, arg1->unk3B8[3], 0x7FU, NULL, NULL, 0, NULL);
                _data_530 = new_var;
            }
            if (arg0->animProgressLayered < 0.4f) {
                arg1->unk8A8 = 0;
            }
            if (func_80025140(arg0, -0.015f, arg2, 0) != 0) {
                arg1->unk87C = -1;
                arg1->unk878 = 3;
                var_s2 = new_var;
            }
            break;
        case 14:
            if (!*temp) {
                gDLL_6_AMSFX->vtbl->play_sound(arg0, arg1->unk3B8[3], 0x7FU, NULL, NULL, 0, NULL);
                _data_530 = new_var;
            }
            arg1->unk87C = -1;
            arg1->unk8A8 = 0;
            arg1->unk878 = 0;
            break;
        case 3:
            if (var_v0 != 0) {
                func_80024E50(arg0, arg0->curModAnimId, arg0->animProgress, 0);
            } else {
                func_80025140(arg0, arg0->animProgress - arg0->animProgressLayered, 1.0f, 0);
            }
            modelInstance = arg0->modelInsts[arg0->modelInstIdx];
            animState = modelInstance->animState1;
            if (animState->unk_0x58[0] == 0) {
                arg0->curModAnimIdLayered = -1;
                arg1->unk878 = 0;
            }
            break;
        default:
            if (arg0->linkedObject != NULL) {
                new_var2 = 0;
                if (arg1->unk8A8 != 0) {
                    if (arg1->unk8A9 == 0) {
                        arg1->unk878 = new_var;
                        var_s2 = new_var;
                        _data_530 = new_var2;
                        break;
                    }
                    if (arg1->unk8A9 == 1) {
                        arg1->unk878 = 0xE;
                        var_s2 = new_var;
                        _data_530 = new_var2;
                    }
                    break;
                }
                if (arg1->unk8A9 == 2) {
                    arg1->unk878 = 2;
                    var_s2 = new_var;
                    _data_530 = new_var2;
                    break;
                }
                if (arg1->unk8A9 == 4) {
                    arg1->unk878 = 0xD;
                    var_s2 = new_var;
                    _data_530 = new_var2;
                }
            }
            break;
        }
        var_v0 = var_s2;
    } while (var_s2 != 0);
}

// offset: 0x6DD8 | func: 33
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_6DD8.s")
#else
// https://decomp.me/scratch/mF6MC
/*
void func_80023A18(Object *obj, s32 param2);
void dll_210_func_90A0(void);
void dll_210_func_955C(void);
void dll_210_func_98CC(void);
*/

void dll_210_func_6DD8(Object* obj, PlayerState* state, s32 arg2) {
    Object* sp3C;
    s16 temp_v1_2;
    void* temp_v0_2;
    Vec3f* temp_v1;
    void* temp_v1_3;
    s16 temp;

    if (state->stats->magic == 0) {
        return;
    }

    switch (arg2) {
        case 0x2D:
            *_bss_220 = 0x2D;
            *_bss_224 = &dll_210_func_90A0;
            break;
        default:
            *_data_0 = 0;
            break;
        case 0x40:
            if (state->unk87C == 0x40) {
                state->unk8BF = -1;
                arg2 = -1;
                dll_210_func_1DE64((s32) _data_38);
            } else {
                state->unk8BF = 1;
                dll_210_func_1DE50((s32) obj, (s32) _data_38, 1);
                func_80023A18(obj, 2);
                *_data_0 = 0;
                gDLL_18->vtbl->func4.withThreeArgs(obj, state, 1);
            }
            gDLL_6_AMSFX->vtbl->play_sound(obj, 0x5B4U, 0x7FU, NULL, NULL, 0, NULL);
            break;
        case 0x1D7:
            arg2 = -1;
            break;
        case 0x777:
            *_bss_220 = 0x777;
            *_bss_224 = &dll_210_func_955C;
            break;
        case 0x5CE:
            *_bss_220 = 0x5CE;
            *_bss_224 = &dll_210_func_98CC;
            break;
        case 0x5FC:
            arg2 = -1;
            sp3C = gDLL_2_Camera->vtbl->func15.asVoidS32();
            if (sp3C != NULL) {
                if ((sp3C->def->unk40->unk10 & 0xF) == 3) {
                    obj->unk0xdc = 4;
                    gDLL_3_Animation->vtbl->func17(3, obj, -1);
                    if (sp3C->unk0x74 != NULL) {
                        temp = arctan2_f(sp3C->unk0x74->x - obj->srt.transl.x, sp3C->unk0x74->z - obj->srt.transl.z) + 0x8000;
                        gDLL_3_Animation->vtbl->func28.withTwoArgs(obj->unk0xb4, temp);
                    }
                    obj->unk0xc4 = sp3C;
                }
            } else {
                break;
            }
        case 0x5BD:
            arg2 = -1;
            sp3C = gDLL_2_Camera->vtbl->func15.asVoidS32();
            if ((sp3C != NULL) && (sp3C->id == 0x414 || sp3C->id == 0x4A9)) {
                gDLL_3_Animation->vtbl->func17(5, obj, -1);
                arg2 = 0x5BD;
                temp = arctan2_f(sp3C->unk0x74->x - obj->srt.transl.x, sp3C->unk0x74->z - obj->srt.transl.z) + 0x8000;
                gDLL_3_Animation->vtbl->func28.withTwoArgs(obj->unk0xb4, temp);
            }
            break;
    }
    state->unk87C = arg2;
}
#endif

// offset: 0x7180 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7180.s")

// offset: 0x7260 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7260.s")

// offset: 0x7300 | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7300.s")

// offset: 0x75B0 | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_75B0.s")

// offset: 0x77DC | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_77DC.s")

// offset: 0x78A8 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_78A8.s")

// offset: 0x7AAC | func: 40
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7AAC.s")

// offset: 0x7B98 | func: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7B98.s")

// offset: 0x7BC4 | func: 42
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7BC4.s")

// offset: 0x7CF8 | func: 43
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7CF8.s")

// offset: 0x7DA0 | func: 44
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7DA0.s")

// offset: 0x7E6C | func: 45
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7E6C.s")

// offset: 0x8AE0 | func: 46
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_8AE0.s")

// offset: 0x8EA4 | func: 47
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_8EA4.s")

// offset: 0x90A0 | func: 48
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_90A0.s")

// offset: 0x955C | func: 49
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_955C.s")

// offset: 0x98CC | func: 50
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_98CC.s")

// offset: 0x9E00 | func: 51
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_9E00.s")

// offset: 0x9F1C | func: 52
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_9F1C.s")

// offset: 0xA018 | func: 53
s32 dll_210_func_A018(void) {
    return 1;
}

// offset: 0xA024 | func: 54
void dll_210_func_A024(Object* player, PlayerState* state) {
    state->unk28C = 0.0f;
    state->unk27C = 0.0f;
    state->unk278 = 0.0f;
    player->speed.x = 0.0f;
    player->speed.y = 0.0f;
    player->speed.z = 0.0f;
}

// offset: 0xA058 | func: 55
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_A058.s")

// offset: 0xA3E4 | func: 56
s32 dll_210_func_A3E4(Object* player, s32 arg1, s32 arg2) {
    return 2;
}

// offset: 0xA3FC | func: 57
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_A3FC.s")

// offset: 0xA8CC | func: 58
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_A8CC.s")

// offset: 0xAA80 | func: 59
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_AA80.s")

// offset: 0xAE34 | func: 60
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_AE34.s")
#else
// matches but requires dll_210_func_B4C8, dll_210_func_BA38, dll_210_func_C1F4 as static

//arg1 seems to point to Krystal's PlayerState struct!
//krystal.c: objGetAnimChange()?
static void dll_210_func_B4C8(Object* player, s32 arg1);
static s32 dll_210_func_BA38(Object* arg0, void* arg1, f32 arg2);
static s32 dll_210_func_C1F4(Object* arg0, void* arg1, f32 arg2);
s32 func_8002493C(Object*, f32, f32*);
//arg1 seems to point to Krystal's PlayerState struct!

//krystal.c: objGetAnimChange()?
s32 dll_210_func_AE34(Object* player, PlayerState* arg1, f32 arg2) {
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fv0;
    f32 animProgress;
    f32 var_fa0;
    s32 temp_t1;
    s32 temp_v0;
    s16 *modAnimIds;
    PlayerState *state;
    s32 var_a1;
    s32 temp_t3;
    
    state = player->state;
    if (arg1->unk272 != 0){
        state->unk8C0 = 0;
        state->unk3C8 = 1.65f;
        state->unk888 = 0;
        arg1->unk2FC = &dll_210_func_B4C8;
    }
    
    if (state->unk868 != 0){
        state->unk3C4 = &_data_6F8;
        state->modAnims = (s16 *) (&_data_F8);
        if (state->unk870 == 0){
            return 8;
        }
    } else if (state->unk8A8 != 0){
        state->unk3C4 = &_data_6F8;
        state->modAnims = (s16 *) (&_data_C8);
    } else {
        state->unk3C4 = &_data_6F8;
        state->modAnims = (s16 *) (&_data_98);
    }
    
    state->unk8BD = state->unk8BD & 0xFFFE;
    arg1->unk0 |= 0x800000;
    if (!state->unk868){
        temp_v0 = dll_210_func_BA38(player, arg1, arg2);
        if (temp_v0){
            return temp_v0;
        }
        temp_v0 = dll_210_func_C1F4(player, arg1, arg2);
        if (temp_v0){
            return temp_v0;
        }
        if ((arg1->unk1B4 > 25.0f) && (arg1->unk1AC < 100.0f)){
            return 0x21;
        }
        if (arg1->unk2C8){
            if (arg1->unk33D == 1){
                return -0x35;
            }
            return -0x43;
        }
    }
    
    if (arg1->unk272){
        if ((arg1->unk26E != 0xB) && (arg1->unk26E != 0xD)){
            player->srt.yaw += arg1->unk32A * 0xB6;
            arg1->unk328 = 0;
            arg1->unk32A = 0;
        }
    }
    if (arg1->unk290 < 0.05f){
        arg1->unk328 = 0;
        arg1->unk32A = 0;
        arg1->unk290 = 0.0f;
    }
    var_fv0 = (arg1->unk290 - 0.4f) / 0.6f;
    if (var_fv0 < 0.0f){
        var_fv0 = 0.0f;
    }
    if (var_fv0 > 1.0f){
        var_fv0 = 1.0f;
    }
    var_fa0 = (state->unk3C8 - 0.05f) * var_fv0;
    if (arg1->unk328 < 0x5A){
        player->srt.yaw = (s16) (player->srt.yaw + (((arg1->unk32A * arg2) / 9.0f) * 182.0f));
    } else {
        var_fa0 = -var_fa0;
    }
    
    arg1->unk28C = (f32) (arg1->unk28C + (((var_fa0 - arg1->unk28C) / arg1->unk2B0) * arg2));
    if (arg1->unk198 > 0){
        var_fa0 -= fsin16_precise(arg1->unk198) * 0.65f;
    }
    else {
        var_fa0 -= fsin16_precise(arg1->unk198) * 0.35f;
    }
    if (state->unk3C8 < arg1->unk28C){
        arg1->unk28C = state->unk3C8;
    }
    if (arg1->unk28C > 1.32f){
        state->unk888 = (s16) (state->unk888 + 1);
    } else {
        state->unk888 = 0;
        state->unk3C8 = 1.65f;
    }
    if (state->unk888 >= 0xB4){
        state->unk888 = 0xB4;
        state->unk3C8 = 1.9f;
    }
    if (var_fa0 < state->unk3C4[2]){
        var_fa0 = state->unk3C4[2];
    }
    arg1->unk278 += (var_fa0 - arg1->unk278) /arg1->unk2B0 * arg2;
    if (state->unk3C8 < arg1->unk278){
        arg1->unk278 = state->unk3C8;
    }
    var_a1 = 0;
    arg1->unk278 += _data_C[0];
    arg1->unk27C += _data_C[1];
    _data_C[0] = 0.0f;
    _data_C[1] = 0.0f;
    animProgress = player->animProgress;
    
    temp_t1 = (state->unk8C0 / 3) * 2;    
    state->unk8A5 = (temp_t1 >> 1) + 1;
    if ((u8)state->unk8A5 >= 4) {
        state->unk89C = state->unk894;
    }
    else {
        state->unk89C = state->unk890;
    }
    
    if (arg1->unk28C < state->unk3C4[temp_t1]){
        var_a1 = 1;
        if (state->unk8C0 == 3){
            return 2;
        }
        state->unk8C0 -= 3;
    } else if (state->unk3C4[temp_t1 + 1] <= arg1->unk28C){
        if (state->unk8C0 < 0xC){
            var_a1 = 1;
            if (state->unk8C0 == 0){
                animProgress = 0.0f;
            }
            state->unk8C0 += 3;
        }
    }
    
    modAnimIds = state->modAnims;
    if (var_a1 || state->modAnims != modAnimIds){
        func_80023D30(player, state->modAnims[state->unk8C0], animProgress, 0);
    }

    // calculations here are absolutely useless but requires to match
    temp_fv0 = (f32)arg1->unk198 / 0x2000;
    if (1.0f < temp_fv0) { temp_fv0 = 1.0f; }
    else if (temp_fv0 < -1.0f) { temp_fv0 = -1.0f; }

    if (0.0f > temp_fv0) {
        // @fake
        if (arg1->unk278 && arg1->unk278) {}
    }

    if (!func_8002493C(player, arg1->unk278, &arg1->unk298)){
        diPrintf("krystal.c: objGetAnimChange Error\n");
    }
    return 0;
}
#endif

// offset: 0xB4C8 | func: 61
void dll_210_func_B4C8(Object* player, s32 arg1) {
    PlayerState* state = player->state;

    state->unk89C = state->unk894;
}

// offset: 0xB4E0 | func: 62
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_B4E0.s")

// offset: 0xB73C | func: 63
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_B73C.s")

// offset: 0xB864 | func: 64
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_B864.s")

// offset: 0xBA38 | func: 65
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_BA38.s")

// offset: 0xC1F4 | func: 66
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_C1F4.s")

// offset: 0xC264 | func: 67
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_C264.s")

// offset: 0xC3D0 | func: 68
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_C3D0.s")

// offset: 0xCAA8 | func: 69
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_CAA8.s")

// offset: 0xCC24 | func: 70
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_CC24.s")

// offset: 0xD3E4 | func: 71
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_D3E4.s")

// offset: 0xD480 | func: 72
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_D480.s")

// offset: 0xD510 | func: 73
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_D510.s")

// offset: 0xD5F0 | func: 74
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_D5F0.s")

// offset: 0xD788 | func: 75
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_D788.s")

// offset: 0xDC10 | func: 76
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_DC10.s")

// offset: 0xE14C | func: 77
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_E14C.s")

// offset: 0xEB1C | func: 78
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_EB1C.s")

// offset: 0xEF8C | func: 79
void dll_210_func_EF8C(s32 arg0, s32 arg1) {
}

// offset: 0xEF9C | func: 80
s32 dll_210_func_EF9C(Object* player, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0xEFB4 | func: 81
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_EFB4.s")

// offset: 0xF00C | func: 82
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_F00C.s")

// offset: 0xF690 | func: 83
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_F690.s")

// offset: 0x1034C | func: 84
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1034C.s")

// offset: 0x10898 | func: 85
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_10898.s")

// offset: 0x10A0C | func: 86
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_10A0C.s")

// offset: 0x10E94 | func: 87
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_10E94.s")

// offset: 0x11C60 | func: 88
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_11C60.s")

// offset: 0x1209C | func: 89
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1209C.s")

// offset: 0x12514 | func: 90
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_12514.s")

// offset: 0x125BC | func: 91
s32 dll_210_func_125BC(Object* arg0, PlayerState* arg1, u32 arg2) {
    f32 temp_fs0;
    f32 temp_fs1;
    f32 f2;
    f32 f0;
    s32 i;
    PlayerState* temp_s3;

    if (arg1->unk272 != 0) {
        ((s16*)arg1)[0x138] = 0x1F;
    }
    arg1->unk0 |= 0x200000;
    temp_s3 = (PlayerState *) &arg1->unk4;
    if (arg1->unk272 != 0) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x3D8U, 0x7FU, NULL, NULL, 0, NULL);
        for (i = 0; i < 3; i++) {
            temp_fs0 = ((f32) rand_next(-0x32, 0x32) / 10.0f) + arg0->srt.transl.x;
            temp_fs1 = ((f32) rand_next(-0x32, 0x32) / 10.0f) + arg0->srt.transl.z;
            gDLL_24_Waterfx->vtbl->func_174C(temp_fs0, temp_s3->unk1B8, temp_fs1, 4.0f);
            gDLL_24_Waterfx->vtbl->func_1CC8(temp_fs0, temp_s3->unk1B8, temp_fs1, 0, 0.0f, 3);
        }
    }
    if (temp_s3->unk1B0 > 25.0f && temp_s3->unk1A8 < 100.0f) {
        return 0x21;
    }
    if (temp_s3->unk25C & 0x10) {
        return 2;
    }
    f0 = temp_s3->unk1B8 - 6.0f;
    f2 = f0 - arg0->srt.transl.y;
    if (f2 > 25.0f) {
        f2 = 25.0f;
    }
    arg0->speed.y += (f2 / 25.0f) * 0.13f * delayFloat;
    arg0->speed.y -= 0.1f * delayFloat;
    arg0->speed.y *= 0.96f;
    if (arg0->speed.y > 1.4f) {
        arg0->speed.y = 1.4f;
    }
    arg0->speed.x *= 0.98f;
    arg0->speed.z *= 0.98f;
    for (i = 0; i < 4; i++) {
        gDLL_17->vtbl->func1(arg0, 0x202, NULL, 0, -1, NULL);
    }
    return 0;
}

// offset: 0x128F4 | func: 92
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_128F4.s")

// offset: 0x12BF0 | func: 93
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_12BF0.s")

// offset: 0x13524 | func: 94
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_13524.s")

// offset: 0x13D08 | func: 95
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_13D08.s")

// offset: 0x1426C | func: 96
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1426C.s")

// offset: 0x142C4 | func: 97
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_142C4.s")

// offset: 0x146D8 | func: 98
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_146D8.s")

// offset: 0x14B70 | func: 99
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_14B70.s")

// offset: 0x14BE8 | func: 100
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_14BE8.s")

// offset: 0x151A0 | func: 101
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_151A0.s")

// offset: 0x15744 | func: 102
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_15744.s")

// offset: 0x158E0 | func: 103
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_158E0.s")

// offset: 0x16204 | func: 104
void dll_210_func_16204(s32 arg0, s32* arg1) {
    *arg1 &= ~0x4000;
}

// offset: 0x16220 | func: 105
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_16220.s")

// offset: 0x164DC | func: 106
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_164DC.s")

// offset: 0x1660C | func: 107
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1660C.s")
#else
static void func_8002674C(); 

void dll_210_func_1660C(Object* player) {
    func_8002674C();
}
#endif

// offset: 0x16648 | func: 108
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_16648.s")

// offset: 0x16EB4 | func: 109
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_16EB4.s")

// offset: 0x178A0 | func: 110
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_178A0.s")

// offset: 0x178EC | func: 111
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_178EC.s")

// offset: 0x17A88 | func: 112
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_17A88.s")

// offset: 0x17B5C | func: 113
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_17B5C.s")

// offset: 0x17C14 | func: 114
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_17C14.s")

// offset: 0x17DA8 | func: 115
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_17DA8.s")

// offset: 0x17EF0 | func: 116
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_17EF0.s")

// offset: 0x17F90 | func: 117
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_17F90.s")

// offset: 0x180D4 | func: 118
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_180D4.s")

// offset: 0x18630 | func: 119
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_18630.s")

// offset: 0x18DB0 | func: 120
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_18DB0.s")

// offset: 0x18E10 | func: 121
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_18E10.s")

// offset: 0x18E80 | func: 122
s32 dll_210_func_18E80(Object* player, PlayerState* state, s32 arg2) {
    if (state->unk310 & 0x8000) {
        return 0x3D;
    }
    return 0;
}

// offset: 0x18EAC | func: 123
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_18EAC.s")

// offset: 0x1A9D4 | func: 124
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1A9D4.s")

// offset: 0x1AAD8 | func: 125
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1AAD8.s")

// offset: 0x1AC3C | func: 126
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1AC3C.s")

// offset: 0x1ADA4 | func: 127
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1ADA4.s")

// offset: 0x1AFF8 | func: 128
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1AFF8.s")

// offset: 0x1B0A8 | func: 129
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1B0A8.s")

// offset: 0x1B1E8 | func: 130
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1B1E8.s")

// offset: 0x1B414 | func: 131
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1B414.s")

// offset: 0x1B640 | func: 132
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1B640.s")

// offset: 0x1B878 | func: 133
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1B878.s")

// offset: 0x1BAC8 | func: 134
void dll_210_func_1BAC8(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    state->flags &= ~0x800;
}

// offset: 0x1BAE8 | func: 135
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1BAE8.s")

// offset: 0x1BC14 | func: 136
s32 dll_210_func_1BC14(Object* player, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x1BC2C | func: 137
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1BC2C.s")

// offset: 0x1BEBC | func: 138
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1BEBC.s")

// offset: 0x1BF8C | func: 139
void dll_210_func_1BF8C(Object* player, s32 arg1) {
    ObjectStruct64* shadow;
    player->ptr0x64->flags &= ~0x1000;
}

// offset: 0x1BFAC | func: 140
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1BFAC.s")

// offset: 0x1C02C | func: 141
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1C02C.s")

// offset: 0x1C07C | func: 142
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1C07C.s")

// offset: 0x1C230 | func: 143
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1C230.s")

// offset: 0x1C460 | func: 144
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1C460.s")

// offset: 0x1C564 | func: 145
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1C564.s")

// offset: 0x1C620 | func: 146
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1C620.s")

// offset: 0x1C9D8 | func: 147
s32 dll_210_func_1C9D8(Object* player, PlayerState* arg1, s32 arg2) {
    PlayerState* state = player->state;

    //NOTE: not sure whether arg1 is really a PlayerState struct
    arg1->unk341 = 3;
    if (state->stats->health > 0) {
        func_80023D30(player, 0xC5, 0.0f, 0);
        return -0x4D;
    }
    return 0;
}

// offset: 0x1CA44 | func: 148
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CA44.s")

// offset: 0x1CB3C | func: 149
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CB3C.s")

// offset: 0x1CC34 | func: 150
s32 dll_210_func_1CC34(Object* player, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x1CC4C | func: 151 | export: 7
Object *dll_210_func_1CC4C(Object* player) {
    PlayerState* state = player->state;
    return state->unk858;
}

// offset: 0x1CC5C | func: 152 | export: 69
void dll_210_func_1CC5C(Object* player, s32 arg1) {
    PlayerState* state = player->state;

    if (arg1 != 0) {
        state->flags |= 0x200000;
        return;
    }
    state->flags &= 0xFFDFFFFF;
}

// offset: 0x1CC94 | func: 153 | export: 68
void dll_210_func_1CC94(Object* player) {
    PlayerState* state = player->state;
    state->flags &= ~2;
}

// offset: 0x1CCB0 | func: 154 | export: 50
s16 dll_210_func_1CCB0(Object* player) {
    PlayerState* state = player->state;
    return state->unk87C;
}

// offset: 0x1CCC0 | func: 155 | export: 51
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CCC0.s")

// offset: 0x1CD04 | func: 156 | export: 22
void dll_210_func_1CD04(Object* player, s32 arg1) {
    PlayerState* state;

    state = player->state;
    if (arg1 < 0) {
        arg1 = 0;
    } else {
        if (state->stats->healthMax < arg1) {
            arg1 = state->stats->healthMax;
        }
    }
    state->stats->health = arg1;
}

// offset: 0x1CD3C | func: 157 | export: 23
void dll_210_func_1CD3C(Object* player, s32 arg1) {
    PlayerState* state = player->state;

    if (arg1 < 0) {
        arg1 = 0;
    } else if (arg1 >= 0x51) {
        arg1 = 0x50;
    }
    state->stats->healthMax = arg1;
}

// offset: 0x1CD6C | func: 158 | export: 24
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CD6C.s")
#else
void dll_210_func_1CD6C(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    PlayerStateUnk34C* temp;

    temp = state->unk34C;
    arg1 = state->unk34C->unk1 + arg1;
    if (arg1 < 0) {
        arg1 = 0;
    } else {
        if (temp->unk2 < arg1) {
            arg1 = temp->unk2;
        }
    }
    temp->unk1 = arg1;
}
#endif

// offset: 0x1CDAC | func: 159 | export: 25
void dll_210_func_1CDAC(Object* player, s32 arg1) {
    PlayerState* state;
    s32 var;

    state = player->state;
    var = state->stats->healthMax;
    var += arg1;

    if (var < 0){
        var = 0;
    } else if (var >= 0x51){
        var = 0x50;
    }
    state->stats->healthMax = var;
}

// offset: 0x1CDE4 | func: 160 | export: 26
s8 dll_210_func_1CDE4(Object* player) {
    PlayerState* state = player->state;
    return state->stats->health;
}

// offset: 0x1CDF8 | func: 161 | export: 27
s8 dll_210_func_1CDF8(Object* player) {
    PlayerState* state = player->state;
    return state->stats->healthMax;
}

// offset: 0x1CE0C | func: 162 | export: 13
void dll_210_func_1CE0C(Object* player, s32 arg1) {
    PlayerState* state;

    state = player->state;
    if (state->unk8BB != 0) {
        if (arg1 < 0) {
            arg1 = 0;
        } else if (arg1 >= 0x65) {
            arg1 = 0x64;
        }
        state->stats->magicMax = arg1;
    }
}

// offset: 0x1CE48 | func: 163 | export: 15
void dll_210_func_1CE48(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    s32 var_v1;

    if (state->unk8BB != 0) {
        var_v1 = state->stats->magicMax;
        var_v1 += arg1;
        if (var_v1 < 0) {
            var_v1 = 0;
        } else if (var_v1 >= 0x65) {
            var_v1 = 0x64;
        }
        state->stats->magicMax = var_v1;
    }
}

// offset: 0x1CE8C | func: 164 | export: 17
s16 dll_210_func_1CE8C(Object* player) {
    PlayerState* state;

    state = player->state;
    if (state->unk8BB == 0) {
        return 0;
    }
    return state->stats->magicMax;
}

// offset: 0x1CEB4 | func: 165 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CEB4.s")

// offset: 0x1CEFC | func: 166 | export: 14
//Plays sound effect when magic refilled?
//NOTE: Patched in Dinomod to prevent undebounced sound!
//https://dinosaurpla.net/wiki/Dinomod_Enhanced/Differences/DLLs/210_KrystalSabre#dll_210_func_1CEFC
void dll_210_func_1CEFC(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    PlayerStats* temp_v1;
    s32 var_v0;

    if (state->unk8BB != 0) {
        temp_v1 = state->stats;
        var_v0 = temp_v1->magic;
        var_v0 += arg1;
        if (var_v0 < 0) {
            var_v0 = 0;
        } else {
            if (temp_v1->magicMax < var_v0) {
                var_v0 = temp_v1->magicMax;
            }
        }
        temp_v1->magic = var_v0;
        if (arg1 > 0) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_5EB, MAX_VOLUME, 0, 0, 0, 0);
        }
    }
}

// offset: 0x1CFA4 | func: 167 | export: 16
s16 dll_210_func_1CFA4(Object* arg0) {
    PlayerState* state;

    state = arg0->state;
    if (state->unk8BB == 0) {
        return 0;
    }
    return state->stats->magic;
}

// offset: 0x1CFCC | func: 168 | export: 70
s8 dll_210_func_1CFCC(Object* player) {
    PlayerState* state = player->state;
    
    if (state->unk260 & 0x10) {
        return state->unkBC;
    }
    return -1;
}

// offset: 0x1CFF4 | func: 169 | export: 62
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CFF4.s")

// offset: 0x1D04C | func: 170 | export: 18
void dll_210_func_1D04C(Object* player, s32 arg1) {
    PlayerState* state = player->state;

    if (arg1 < 0) {
        arg1 = 0;
    } else if (arg1 >= 0x3E8) {
        arg1 = 0x3E7;
    }
    state->stats->scarabs = arg1;
}

// offset: 0x1D07C | func: 171 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D07C.s")

// offset: 0x1D128 | func: 172 | export: 20
u16 dll_210_func_1D128(Object* player) {
    PlayerState* state = player->state;
    return state->stats->scarabs;
}

// offset: 0x1D13C | func: 173 | export: 21
s8 dll_210_func_1D13C(Object* player) {
    PlayerState* state = player->state;
    s8 previous;

    previous = state->stats->unkA;
    state->stats->unkA = 0;
    return previous;
}

// offset: 0x1D154 | func: 174 | export: 28
void dll_210_func_1D154(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    state->unk8A9 = arg1 ? 4 : 2;
}

// offset: 0x1D178 | func: 175 | export: 29
void dll_210_func_1D178(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    state->unk8A9 = arg1 ? 1 : 0;
}

// offset: 0x1D198 | func: 176 | export: 30
s32 dll_210_func_1D198(Object* player) {
    PlayerState* state = player->state;
    return state->unk8A9 == 2;
}

// offset: 0x1D1B0 | func: 177 | export: 31
void dll_210_func_1D1B0(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    state->unk8A9 = arg1;
}

// offset: 0x1D1C0 | func: 178 | export: 32
u8 dll_210_func_1D1C0(Object* player) {
    PlayerState* state = player->state;
    return state->unk8A9;
}

// offset: 0x1D1D0 | func: 179 | export: 78
s32 dll_210_func_1D1D0(Object* player) {
    PlayerState* state = player->state;
    return state->unk864;
}

// offset: 0x1D1E0 | func: 180 | export: 79
f32 dll_210_func_1D1E0(Object* player) {
    PlayerState* state = player->state;
    return state->unk7FC;
}

// offset: 0x1D1F0 | func: 181 | export: 8
s32 dll_210_func_1D1F0(Object* player, Object** arg1) {
    PlayerState* state = player->state;
    *arg1 = state->unk708;
    return state->unk708 != NULL;
}

// offset: 0x1D210 | func: 182 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D210.s")

// offset: 0x1D288 | func: 183 | export: 10
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D288.s")
#else
s32 dll_210_func_1D288(Object* player, s32* arg1) {
    PlayerState* state = player->state;
    *arg1 = state->unk868;
    return state->unk868 != 0;
}
#endif

// offset: 0x1D2A8 | func: 184 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D2A8.s")

// offset: 0x1D368 | func: 185 | export: 36
u8 dll_210_func_1D368(Object* player, f32* arg1) {
    PlayerState* state = player->state;

    *arg1 = state->unk800;
    return state->unk8B7;
}

// offset: 0x1D380 | func: 186 | export: 37
void dll_210_func_1D380(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    state->unk8B7 = arg1;
}

// offset: 0x1D390 | func: 187 | export: 38
s32 dll_210_func_1D390(Object* player, s32 arg1) {
    PlayerState* state = player->state;
    return state->unk8BA & arg1;
}

// offset: 0x1D3A4 | func: 188 | export: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D3A4.s")

// offset: 0x1D40C | func: 189 | export: 40
s32 dll_210_func_1D40C(Object* player, u32 arg1) {
    PlayerState* state = player->state;
    if (arg1 >= 8) {
        return 0;
    }
    return state->unk8BB & (1 << arg1);
}

// offset: 0x1D438 | func: 190 | export: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D438.s")

// offset: 0x1D4C8 | func: 191 | export: 44
s32 dll_210_func_1D4C8(Object* player) {
    PlayerState* state = player->state;
    return state->unk372 == 1;
}

// offset: 0x1D4E0 | func: 192 | export: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D4E0.s")

// offset: 0x1D588 | func: 193 | export: 35
s32 dll_210_func_1D588(s32 arg0) {
    return 1;
}

// offset: 0x1D598 | func: 194 | export: 34
s32 dll_210_func_1D598(Object* player) {
    PlayerState* state = player->state;
    return state->flags & 1;
}

// offset: 0x1D5B0 | func: 195 | export: 42
u8 dll_210_func_1D5B0(Object* player) {
    PlayerState* state = player->state;
    return state->unk8B8;
}

// offset: 0x1D5C0 | func: 196 | export: 43
int dll_210_func_1D5C0(Object* player) {
    PlayerState* state = player->state;
    return state->unk26C == 0x20 || state->unk26C == 0x21;
}

// offset: 0x1D5E8 | func: 197 | export: 47
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D5E8.s")
#else
s32 dll_210_func_1D5E8(Object* player, s32 arg1, s8* arg2) {
    PlayerState* state = player->state;

    *arg2 = state->unk6AE;
    return (state->unk26C == 0x28 && state->unk6A8 == arg1) & 0xFF;
}
#endif

// offset: 0x1D620 | func: 198 | export: 60
s32 dll_210_func_1D620(Object* player) {
    PlayerState* state = player->state;
    return state->unk260 & 0x10;
}

// offset: 0x1D638 | func: 199 | export: 61
void dll_210_func_1D638(Object* player, s32 arg1) {
    PlayerState* state = player->state;

    if (arg1 != 0) {
        state->flags |= 0x10;
        return;
    }
    state->flags &= ~0x10;
}

// offset: 0x1D668 | func: 200 | export: 58
void dll_210_func_1D668(Object* player, f32 arg1) {
    PlayerState* state = player->state;
    state->unk808 = arg1;
}

// offset: 0x1D680 | func: 201 | export: 59
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D680.s")

// offset: 0x1D6E0 | func: 202 | export: 71
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D6E0.s")

// offset: 0x1D754 | func: 203 | export: 52
s32 dll_210_func_1D754(Object* player) {
    PlayerState* state = player->state;
    state->unk854 = 0;
    return 0;
}

// offset: 0x1D768 | func: 204 | export: 53
Object *dll_210_func_1D768(Object* player) {
    PlayerState* state = player->state;
    return state->unk2C8;
}

// offset: 0x1D778 | func: 205 | export: 54
s8* dll_210_func_1D778(Object* player) {
    PlayerState* state = player->state;
    return &state->unk354;
}

// offset: 0x1D788 | func: 206 | export: 56
f32 dll_210_func_1D788(Object* player) {
    PlayerState* state = player->state;
    return state->unk278;
}

// offset: 0x1D798 | func: 207 | export: 55
void dll_210_func_1D798(Object* player, PlayerState* state, s32* arg2, s8* arg3, void** arg4) {
    PlayerState* newState = player->state;

    *arg3 = 0x7F;
    *arg2 = newState->unk89C;
    *arg4 = (s8*)newState + 0x39C;
}

// offset: 0x1D7C4 | func: 208 | export: 57
s32 dll_210_func_1D7C4(Object* player) {
  PlayerState* state;
  state = player->state;
  return (s32)((s8*) state) + 4;
}

// offset: 0x1D7D4 | func: 209 | export: 72
void dll_210_func_1D7D4(Object* player, s16 arg1) {
    PlayerState* state = player->state;
    state->unk886 = arg1;
}

// offset: 0x1D7F0 | func: 210 | export: 73
void dll_210_func_1D7F0(s32 arg0, f32 arg1, f32 arg2) {
    _data_C[0] = arg1;
    _data_C[1] = arg2;
}

// offset: 0x1D820 | func: 211 | export: 74
s16 dll_210_func_1D820(s32 arg0) {
    s16 var_v1;

    if (_data_14[0] == 0) {
        var_v1 = _data_18[0];
    } else {
        var_v1 = -1;
    }
    return var_v1;
}

// offset: 0x1D85C | func: 212 | export: 75
s32 dll_210_func_1D85C(s32 arg0, s32 arg1) {
    _data_14[0] = arg1;
    return 0;
}

// offset: 0x1D880 | func: 213 | export: 77
//While aiming with the Projectile Spell
s32 dll_210_func_1D880(Object* player, s32* arg1, s32* arg2) {
    PlayerState* state;

    state = player->state;
    if (state->flags & 0x400) {
        *arg1 = state->aimX;
        *arg2 = state->aimY;
        return 1;
    }
    return 0;
}

// offset: 0x1D8B8 | func: 214 | export: 76
void dll_210_func_1D8B8(Object* player) {
    PlayerState* state = player->state;

    state->unk8BE = 1;
    state->unk840 = -0.2f;
    state->unk844 = 0.0f;
}

// offset: 0x1D8EC | func: 215
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D8EC.s")

// offset: 0x1DAB0 | func: 216
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1DAB0.s")

// offset: 0x1DB6C | func: 217
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1DB6C.s")

// offset: 0x1DC48 | func: 218
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1DC48.s")

// offset: 0x1DD94 | func: 219
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1DD94.s")

// offset: 0x1DE50 | func: 220
void dll_210_func_1DE50(s32 arg0, s32 arg1, s32 arg2) {
}

// offset: 0x1DE64 | func: 221
void dll_210_func_1DE64(s32 arg0) {
}
