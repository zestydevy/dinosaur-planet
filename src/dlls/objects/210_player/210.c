#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/print.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/gfx/gx.h"
#include "sys/camera.h"
#include "sys/menu.h"
#include "sys/objmsg.h"
#include "sys/map.h"
#include "functions.h"

#include "dll.h"
#include "dlls/objects/210_player.h"
#include "segment_334F0.h"

static void dll_210_func_14B70(Object* arg0, u32 arg1);
static void dll_210_func_7260(Object* arg0, PlayerState* arg1);

// These funcs are static funcs but can only be made static once the funcs they are used in are matched
/* static */ void dll_210_func_1D8EC(Object* arg0, PlayerState* arg1, s32 arg2);
/* static */ void dll_210_func_41F4(Object* arg0, PlayerState* arg1);
/* static */ void dll_210_func_9E00(Object* arg0);
/* static */ void dll_210_func_1CEFC(Object* player, s32 arg1);
/* static */ void dll_210_func_363C(Object* arg0, PlayerState* arg1, Gfx** arg2, Mtx** arg3, Vertex** arg4);
/* static */ void dll_210_func_3B40(Object* arg0, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4);
/* static */ void dll_210_func_1BC0(Object* arg0, PlayerState* arg1);
/* static */ void dll_210_func_1DB6C(Object* arg0, f32 arg1);
/* static */ f32 dll_210_func_63F0(PlayerState* arg0, f32 arg1);
/* static */ void dll_210_func_9F1C(Object* arg0, s32 arg1);
/* static */ void dll_210_func_60A8(Object* arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
/* static */ void dll_210_func_1DE64(s32 arg0);
/* static */ void dll_210_func_1DE50(s32 arg0, s32 arg1, s32 arg2);
/* static */ void dll_210_func_90A0(Object* arg0, PlayerState* arg1, f32 arg2);
/* static */ void dll_210_func_955C(Object* arg0, PlayerState* arg1, f32 arg2);
/* static */ void dll_210_func_98CC(Object* arg0, PlayerState* arg1, f32 arg2);
/* static */ void dll_210_func_6DD8(Object* obj, PlayerState* state, s32 arg2);
/* static */ s32 dll_210_func_C1F4(Object* arg0, PlayerState* arg1, f32 arg2);
/* static */ void dll_210_func_B4C8(Object* player, s32 arg1);
/* static */ void dll_210_func_1AAD8(Object* arg0, UNK_TYPE_32 arg1);
/* static */ s32 dll_210_func_18E80(Object* player, PlayerState* state, f32 arg2);
/* static */ s32 dll_210_func_1A9D4(Object* arg0, s32* arg1, s32* arg2, s32* arg3, f32 arg4, f32 arg5);
/* static */ Object *dll_210_func_1DD94(Object* obj, s32 arg1);
/* static */ void dll_210_func_1DC48(Object* obj);
/* static */ void dll_210_func_8EA4(Object* arg0, PlayerState* arg1, Object* arg2, Gfx** arg3, Mtx** arg4, Vertex** arg5, Triangle** arg6, s32 arg7);
/* static */ s32 dll_210_func_BA38(Object* arg0, PlayerState* arg1, f32 arg2);

// Used before declared / implemented
void dll_210_func_1D8B8(Object* player);
void dll_210_func_1D4E0(Object* arg0, s32 arg1);
s32 dll_210_func_1D2A8(Object* arg0, Object* arg1);

s32 func_80025140(Object*, f32, f32, s32);
MtxF* func_80032170(Object*, s32);
void func_80007E2C(Vec3f*, s32*);
void func_80007EE0(Vec3f*, s32*);
s32 func_80008048(s32*, s32*, s32*, s32, s32);
s32 func_80031F6C(Object*, s32, f32*, f32*, f32*, s32);
void func_8004E64C(Object* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80032238(Object* obj, s32 arg1, s32 arg2, Vec3f* arg3);
void func_80034FF0(s32);
void func_80035AF4(Gfx**, Mtx**, Vertex**, Triangle**, Object*, void*, s32, s32, void*, s32, s32);

/*0x0*/ static const DLTri _rodata_0[] = {
    { 0x40, 0x02, 0x01, 0x00, { 0, 0, 0, 0, 0, 0 }}, {0x40, 0x03, 0x01, 0x02, { 0, 0, 0, 0, 0, 0 }},
    { 0x40, 0x04, 0x05, 0x06, { 0, 0, 0, 0, 0, 0 }}, {0x40, 0x06, 0x05, 0x07, { 0, 0, 0, 0, 0, 0 }},
    { 0x40, 0x06, 0x03, 0x02, { 0, 0, 0, 0, 0, 0 }}, {0x40, 0x03, 0x06, 0x07, { 0, 0, 0, 0, 0, 0 }},
    { 0x40, 0x03, 0x07, 0x01, { 0, 0, 0, 0, 0, 0 }}, {0x40, 0x07, 0x05, 0x01, { 0, 0, 0, 0, 0, 0 }},
    { 0x40, 0x04, 0x00, 0x01, { 0, 0, 0, 0, 0, 0 }}, {0x40, 0x04, 0x01, 0x05, { 0, 0, 0, 0, 0, 0 }},
    { 0x40, 0x00, 0x04, 0x02, { 0, 0, 0, 0, 0, 0 }}, {0x40, 0x02, 0x04, 0x06, { 0, 0, 0, 0, 0, 0 }}
};
/*0xC0*/ static const Vec3f _rodata_C0[] = {
    { -14.5f, 20.0f, -14.5f },
    { 14.5f, 20.0f, -14.5f },
    { -14.5f, 20.0f, 14.5f },
    { 14.5f, 20.0f, 14.5f }
};
/*0xF0*/ static const Vec3f _rodata_F0[] = {
    { -14.5f, -7.0f, -14.5f },
    { 14.5f, -7.0f, -14.5f },
    { -14.5f, -7.0f, 14.5f },
    { 14.5f, -7.0f, 14.5f }
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
/*0xC*/ static f32 _data_C[] = { 0.0f, 0.0f };
/*0x14*/ static u8 _data_14[4] = { 0, 0, 0, 0 };
/*0x18*/ static s16 _data_18[2] = { -1, 0 };
/*0x1C*/ static u32 _data_1C[] = {
    0x00000000
};
/*0x20*/ static u32 _data_20[] = {
    0x01020000
};
/*0x24*/ static s16 _data_24[] = { 0x0069, 0x0023 };
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
/*0x70*/ static f32 _data_70[] = { 8.5f, 0.0f, 0.0f };
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
/*0x158*/ static s16 _data_158[] = {
    0x0017, 0x00c6, 0x0018, 0x0019, 0x001a, 0x00be, 0x00c0, 0x00bf, 0x00ca, 0x00cb, 0xffff, 0xffff
};
/*0x170*/ static s16 _data_170[] = {
    0x00f5, 0x00f5, 0x00f5, 0x00f5, 0x00f5, 0x00f5, 0x00f3, 0x00f4, 0x00f6, 0x00f7, 0xffff, 0xffff
};
/*0x188*/ static s16 _data_188[] = {
    0x001b, 0x001d, 0x0453, 0x0454
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
/*0x528*/ static f32 _data_528 = 0.0f;
/*0x52C*/ static s8 _data_52C = 0;
/*0x530*/ static u8 _data_530 = 1;
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
/*0x7BC*/ static s8 _data_7BC = 0;
/*0x7C0*/ static DLL_Unknown *_data_7C0 = NULL;
/*0x7C4*/ static s16 _data_7C4[2] = { 0, 0 };
/*0x7C8*/ static u8 _data_7C8[] = { 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xFF, 0x00, 0x00, 0x00 };
/*0x7D4*/ static u8 _data_7D4[] = { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x08, 0x00, 0x00, 0x00 };

/*0x0*/ static u8 _bss_0[0x2];
/*0x2*/ static u8 _bss_2[0x2];
/*0x4*/ static u8 _bss_4[0x4];
/*0x8*/ static u8 _bss_8[0x8];
/*0x10*/ static u8 _bss_10[0x4];
/*0x14*/ static u8 _bss_14[0x4];
/*0x18*/ static u8 _bss_18[0x4];
/*0x1C*/ static u8 _bss_1C[0x4];
/*0x20*/ static f32 _bss_20;
/*0x24*/ static f32 _bss_24;
/*0x28*/ static f32 _bss_28;
/*0x2C*/ static f32 _bss_2C;
/*0x30*/ static f32 _bss_30;
/*0x34*/ static u8 _bss_34[0x4];
/*0x38*/ static u8 _bss_38[0x4];
/*0x3C*/ static f32 _bss_3C;
/*0x40*/ static u8 _bss_40[0xc]; // srt *?
/*0x4C*/ static f32 _bss_4C; // obj x
/*0x50*/ static f32 _bss_50; // obj y
/*0x54*/ static f32 _bss_54; // obj z
/*0x58*/ static u8 _bss_58[0x8];
/*0x60*/ static u8 _bss_60[0x13c];
/*0x19C*/ static u8 _bss_19C[0x4];
/*0x1A0*/ static u8 _bss_1A0[0x4];
/*0x1A4*/ static u8 _bss_1A4[0x4];
/*0x1A8*/ static u8 _bss_1A8[0x2];
/*0x1AA*/ static u8 _bss_1AA[0x2];
/*0x1AC*/ static f32 _bss_1AC;
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
/*0x200*/ static s16 _bss_200;
/*0x202*/ static u8 _bss_202[0x2];
/*0x204*/ static u8 _bss_204[0x4];
/*0x208*/ static u8 _bss_208[0x4];
/*0x20C*/ static u8 _bss_20C[0x4];
/*0x210*/ static Object *_bss_210[4];
/*0x220*/ static s16 _bss_220[2];
/*0x224*/ static s32 _bss_224[1];
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
void dll_210_func_1BC0(Object* arg0, PlayerState* arg1) {
    Object* linkedObj;

    arg0->unk0xb0 &= ~3;
    if (arg1->unk8A8 == 2) {
        arg0->unk0xb0 |= 3;
    } else if (arg1->unk8A8 == 0) {
        arg0->unk0xb0 |= 1;
    }
    if (arg0->id != 0) {
        return;
    }

    linkedObj = arg0->linkedObject;
    if (linkedObj == NULL) {
        return;
    }

    if ((arg1->unk8A8 != 0) && (linkedObj->modelInstIdx != 0)) {
        func_80023A18(linkedObj, 0);
    } else if ((arg1->unk8A8 == 0) && (linkedObj->modelInstIdx != 1)) {
        func_80023A18(linkedObj, 1);
    }
}

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
                        gDLL_18->vtbl->func4(arg0, arg2, 0x3A);
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
            gDLL_18->vtbl->func4(arg0, arg2, 0x4D);
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
            gDLL_18->vtbl->func4(arg0, arg2, 0x4D);
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
            gDLL_18->vtbl->func4(arg0, arg2, 0x4D);
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
                        gDLL_18->vtbl->func4(arg0, arg2, 0x2B);
                    }
                } else {
                    set_gplay_bitstring(sp88, 1U);
                    if (arg2->unk26C != 0x2A) {
                        gDLL_18->vtbl->func4(arg0, arg2, 0x2A);
                    }
                }
            } else if (arg2->unk26C != 0x2A) {
                gDLL_18->vtbl->func4(arg0, arg2, 0x2A);
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
                gDLL_18->vtbl->func4(arg0, arg2, 5);
                arg1->unk8A9 = 1;
            }
            break;
        case 0x100010:
            arg1->unk870 = 1;
            if (arg1->unk868 == NULL) {
                arg1->unk868 = sp8C;
                arg1->unk86C = sp88 >> 0x10;
                gDLL_18->vtbl->func4(arg0, arg2, 5);
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
        func_80031F6C(arg0, 9, &state->unk7EC.x, &state->unk7EC.y, &state->unk7EC.z, 0);
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
            func_80031F6C(arg0, 7, &state->unk680.unk1C, &state->unk680.unk20, &state->unk680.unk24, 0);
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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_363C.s")
#else
// Matches but has weird bss issues
void dll_210_func_363C(Object* arg0, PlayerState* arg1, Gfx** arg2, Mtx** arg3, Vertex** arg4) {
    static SRT _bss_40;
    s32 i;

    gDLL_14_Modgfx->vtbl->func6(arg2, arg3, arg4, 1, arg0->linkedObject);
    if (arg1->unk87C == 0x40) {
        *_data_0 += delayByte;
        if ((*_data_0 >= 0x65) && !(arg0->unk0xb0 & 0x1000)) {
            dll_210_func_1CEFC(arg0, -1);
            *_data_0 = 0;
        }
    }
    if (((arg1->unk8BF == 1U) || (arg1->unk8BF == -1)) && (arg0->modelInstIdx == 2) && ((arg1->unk8BF == 1) || (arg1->unk8BF == -1))) {
        dll_210_func_1D8EC(arg0, arg1, delayByte);
    }
    if (arg1->unk87C == 0x1D7) {
        gDLL_32->vtbl->func4(arg2, arg3, arg0);
        *_data_0 += delayByte;
        if ((*_data_0 >= 0x65) && !(arg0->unk0xb0 & 0x1000)) {
            dll_210_func_1CEFC(arg0, -1);
            *_data_0 = 0;
        }
    }
    if (arg1->unk886 != 0) {
        gDLL_32->vtbl->func2(arg0, arg1->unk886, 0);
    }
    arg1->unk886 = 0;
    if (arg1->unk8BE == 1) {
        dll_210_func_9E00(arg0);
    }
    if (arg1->unk8BA != 0) {
        gDLL_32->vtbl->func5(arg2, arg3, arg0);
    }
    if (gDLL_7_Newday->vtbl->func12.asVoidS32() != 0) {
        dll_210_func_41F4(arg0, arg1);
    }
    if (arg1->flags & 0x60000) {
        _bss_40.transl.x = arg0->srt.transl.x;
        _bss_40.transl.y = arg0->srt.transl.y;
        _bss_40.transl.z = arg0->srt.transl.z;
        if (arg1->flags & 0x40000) {
            gDLL_17->vtbl->func1(arg0, 0x427, &_bss_40, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x427, &_bss_40, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x427, &_bss_40, 0x200001, -1, NULL);
        }
        if (arg1->flags & 0x20000) {
            for (i = 0; i < 0x14; i++) {
                gDLL_17->vtbl->func1(arg0, 0x427, &_bss_40, 0x200001, -1, NULL);
            }
            for (i = 0; i < 7; i++) {
                _bss_40.transl.x += rand_next(-0x32, 0x32) / 10.0f;
                _bss_40.transl.z += rand_next(-0x32, 0x32) / 10.0f;
                gDLL_24_Waterfx->vtbl->func_174C(_bss_40.transl.x, arg0->srt.transl.y, _bss_40.transl.z, 4.0f);
                gDLL_24_Waterfx->vtbl->func_1CC8(_bss_40.transl.x, arg0->srt.transl.y, _bss_40.transl.z, 0, 0.0f, 3);
            }
            arg1->flags &= 0xFFFDFFFF;
        }
    }
}
#endif

// offset: 0x3B40 | func: 12
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_3B40.s")
#else
// https://decomp.me/scratch/oPG0g
void dll_210_func_3B40(Object* arg0, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle **arg4) {
    Vertex* sp74;
    Vertex* sp70;
    f32 sp64;
    SRT sp48;
    PlayerState* sp44;
    Gfx* temp_v0_4;
    s32 var_v0_2;
    s32 i;

    sp44 = arg0->state;
    sp64 = sp44->unk844;
    if (sp64 < 0.0f) {
    }
    sp70 = *arg3;
    sp74 = *arg3;
    dl_set_prim_color(arg1, 0xFFU, 0xFFU, 0xFFU, 0x80U);
    gSPGeometryMode(*arg1, 0xFFFFFF, G_FOG| G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER);
    dl_apply_geometry_mode(arg1);
    gDPSetCombineLERP(*arg1, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dl_apply_combine(arg1);
    sp70 = *arg3;
    for (i = 0; i < 4; i++) {
        sp70[i].x = -14.5f * 20.0f;
        sp70[i].y = ((40.0f - sp64) * 20.0f);
        sp70[i].z = -14.5f * 20.0f;
        sp70[i].r = 0xFF;
        sp70[i].g = 0;
        sp70[i].b = 0;
        sp70[i].a = 0x40;
    }
    temp_v0_4 = *arg1;
    temp_v0_4->words.w0 = 0xEF182C00;\
    temp_v0_4->words.w1 = 0xCB104B70;
    dl_apply_other_mode(arg1);
    sp48.transl.x = arg0->srt.transl.x;
    sp48.transl.y = arg0->srt.transl.y;
    sp48.transl.z = arg0->srt.transl.z;
    sp48.roll = 0;
    sp48.pitch = 0;
    sp48.yaw = arg0->srt.yaw;
    sp48.scale = 0.05f;
    func_800032C4(arg1, arg2, &sp48, 1.0f, 0.0f, NULL);
    gSPVertex((*arg1)++, OS_PHYSICAL_TO_K0(sp74), 8, 0);
    dl_triangles(arg1, (DLTri* ) _rodata_0, 0xC);
    if (sp44->unk844 >= 68.0f) {
        var_v0_2 = arg0->unk_0x36 - (delayByte * 4);
        if (var_v0_2 < 0) {
            var_v0_2 = 0;
        }
        arg0->unk_0x36 = (u8) var_v0_2;
    }
    *arg3 = sp70;
}
#endif

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
Object *dll_210_func_4114(Object* player) {
    PlayerState* state = player->state;

    if (state->unk8B5 == 7) {
        return state->unk6B0.unk38;
    }
    return 0;
}

// offset: 0x413C | func: 19 | export: 46
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_413C.s")

// offset: 0x4198 | func: 20 | export: 48
s16 dll_210_func_4198(Object* player) {
    PlayerState* state = player->state;
    Object* heldObject;

    if (state->flags & 0x10000) {
        heldObject = state->unk868;
        if (heldObject != NULL) {
            return heldObject->id;
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
void dll_210_func_41F4(Object* arg0, PlayerState* arg1) {
    SRT sp48;
    Vec3f sp3C;
    MtxF* temp_v0;

    if (arg1->unk8A6 == 0) {
        return;
    }

    if (arg1->unk8A6 & 1) {
        temp_v0 = func_80032170(arg0, 2);
        sp3C.x = temp_v0->m[3][0];
        sp3C.y = temp_v0->m[3][1];
        sp3C.z = temp_v0->m[3][2];
        temp_v0->m[3][0] = 0.0f;
        temp_v0->m[3][1] = 0.0f;
        temp_v0->m[3][2] = 0.0f;
        sp48.transl.x = 0.0f;
        sp48.transl.y = rand_next(4, 10) * -0.05f;
        sp48.transl.z = rand_next(4, 8) * -0.25f;
        vec3_transform(temp_v0, sp48.transl.x, sp48.transl.y, sp48.transl.z, &sp48.transl.x, &sp48.transl.y, &sp48.transl.z);
        temp_v0->m[3][0] = sp3C.x;
        temp_v0->m[3][1] = sp3C.y;
        temp_v0->m[3][2] = sp3C.z;
        sp3C.x = sp48.transl.x;
        sp3C.y = sp48.transl.y;
        sp3C.z = sp48.transl.z;
        sp48.transl.x = 0.0f;
        sp48.transl.y = -3.0f;
        sp48.transl.z = -50.0f;
        func_80031F6C(arg0, 8, &sp48.transl.x, &sp48.transl.y, &sp48.transl.z, 1);
        gDLL_17->vtbl->func1(arg0, 0xC9, &sp48, 0x200001, -1, &sp3C.x);
    }
    arg1->unk8A6--;
}

// offset: 0x43EC | func: 23 | export: 65
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_43EC.s")

// offset: 0x44A4 | func: 24 | export: 66
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_44A4.s")

// offset: 0x4634 | func: 25 | export: 67
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_4634.s")

// offset: 0x47B8 | func: 26
void dll_210_func_47B8(Object* arg0, PlayerState* arg1) {
    AABBs32 spB0;
    s32 pad2;
    s32 pad;
    f32 spA4;
    Vec3f sp98;
    Unk80027934 sp2C;

    spA4 = 0.0f;
    sp2C.unk40[0] = 0.0f;
    sp2C.unk50[0] = -1;
    sp2C.unk54[0] = 3;
    sp98.x = arg0->srt.transl.x - (fsin16_precise(arg0->srt.yaw) * 15.0f);
    sp98.y = arg0->srt.transl.y + 5.0f;
    sp98.z = arg0->srt.transl.z - (fcos16_precise(arg0->srt.yaw) * 15.0f);
    fit_aabb_around_cubes(&spB0, &arg0->srt.transl, &sp98, &spA4, 1);
    func_80053750(arg0, &spB0, 8);
    if (func_8005509C(arg0, &arg0->srt.transl.x, (f32*)&sp98, 1, &sp2C, 0U) != 0) {
        arg1->flags |= 0x100000;
        return;
    }
    arg1->flags &= ~0x100000;
}


// offset: 0x4910 | func: 27
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_4910.s")
#else
// Needs these as static:
// dll_210_func_1BC0 (matched)
// dll_210_func_1DB6C (matched)
// dll_210_func_63F0 (matched)
// dll_210_func_9F1C (matched)
// dll_210_func_60A8 (matched)

typedef struct Unk {
    u8 pad0[0x24];
    f32 unk24;
    u8 pad28[0x4C - 0x28];
    Vec3f unk4C;
    f32 unk58;
    s16 yawDiff;
    s16 pitchDiff;
    s16 rollDiff;
    s8 unk62;
    u8 pad63;
    s16 unk64;
    s16 unk66;
    u8 pad68[0x7A - 0x68];
    s16 unk7A;
    s16 unk7C;
    u8 unk7E[0x8D - 0x7E];
    u8 unk8D;
    u8 unk8E[0x98 - 0x8E]; // unknwon size
    u8 unk98;
    u8 pad99[0x9D - 0x99];
    u8 unk9D;
    u8 pad9E[0xF4 - 0x9E];
    void *unkF4;
} Unk;
s32 dll_210_func_4910(Object* arg0, Object* arg1, Unk* arg2, s8 arg3) {
    static s8 _bss_0;
    static s16 _bss_2;
    ObjCreateInfo* temp_s1;
    Object* temp_a0_4;
    s32 var_v0;
    s32 spC8;
    Object** objects;
    s32 spC0;
    f32 temp_fa1;
    f32 temp_fv0_2;
    f32 spB4;
    f32 spB0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    f32 temp_fv0_4;
    f32 sp9C;
    s16* temp_s0_2;
    s32 temp_t1;
    s32 temp_v1_8;
    Object* var_s0; // sp8C
    s32 var_s1;
    PlayerState* temp_fp;
    Vec3f* temp_s0;
    Object* temp_v0_6;
    f32 *temp_data_528 = &_data_528;
    f32 sp6C[3];
    s16 sp6A;
    s8 *temp_data_52C = &_data_52C;
    f32 sp60;
    Object *tempObj;

    temp_fp = arg0->state;
    temp_s1 = arg1->createInfo;
    spC8 = 0;
    arg2->unkF4 = dll_210_func_60A8;
    temp_fp->unk818 = 0.0f;
    _bss_1AC = dll_210_func_63F0(temp_fp, delayFloat);
    if (_bss_1AC > 4.0f) {
        _bss_1AC = 4.0f;
    }
    if (temp_fp->flags & 0x8000) {
        if (temp_fp->stats->health > 0) {
            gDLL_18->vtbl->func4(arg0, temp_fp, 1);
            temp_fp->flags &= 0xFFFF7FFF;
            return 0;
        }
        return 1;
    }
    if (arg0->linkedObject == NULL) {
        arg0->linkedObject = obj_create(obj_alloc_create_info(0x18, _data_24[temp_fp->unk8B4]), 4U, -1, -1, arg0->parent);
    } else {
        arg0->linkedObject->parent = arg0->parent;
    }
    func_800267A4(arg0);
    temp_fp->flags &= ~2;
    if (arg2->unk62 != 0) {
        temp_fp->unk8A9 = 1;
        temp_fp->flags &= ~0x400;
        if ((((s8*)temp_s1)[0x20] == 0) || arg2->unk62 == 3 || arg2->unk62 == 2) {
            arg2->unk7A = arg2->unk7C;
            if (arg2->unk62 != 2) {
                arg2->unk58 = 1.0f;
                arg2->unk4C.x = arg0->srt.transl.x - arg1->srt.transl.x;
                arg2->unk4C.y = arg0->srt.transl.y - arg1->srt.transl.y;
                arg2->unk4C.z = arg0->srt.transl.z - arg1->srt.transl.z;
                arg2->yawDiff = arg0->srt.yaw - (arg1->srt.yaw & 0xFFFF);
                if (arg2->yawDiff >= 0x8001) {
                    arg2->yawDiff += 0xFFFF0001;
                }
                if (arg2->yawDiff < -0x8000) {
                    arg2->yawDiff += 0xFFFF;
                }
                arg2->pitchDiff = arg0->srt.pitch - (arg1->srt.pitch & 0xFFFF);
                if (arg2->pitchDiff >= 0x8001) {
                    arg2->pitchDiff += 0xFFFF0001;
                }
                if (arg2->pitchDiff < -0x8000) {
                    arg2->pitchDiff += 0xFFFF;
                }
                arg2->rollDiff = (arg1->srt.roll  & 0xFFFF) - (arg0->srt.roll & 0xFFFF);
                if (arg2->rollDiff >= 0x8001) {
                    arg2->rollDiff += 0xFFFF0001;
                }
                if (arg2->rollDiff < -0x8000) {
                    arg2->rollDiff += 0xFFFF;
                }
                arg2->unk62 = 2;
            }
            arg2->unk58 -= arg2->unk24 * delayFloat;
            if (arg2->unk58 <= 0.0f) {
                arg2->unk62 = 0;
            }
            arg0->curModAnimIdLayered = -1;
        } else if (arg2->unk62 == 4) {
            arg2->unk7A &= 0xFFF3;
            arg2->unk7C &= 0xFFF7;
            temp_v0_6 = gDLL_2_Camera->vtbl->func15.asVoidS32();
            if (temp_v0_6 == NULL || temp_v0_6->unk0x74 == NULL) {
                return 0;
            }
            temp_s0 = temp_v0_6->unk0x74;
            spB4 = arg0->srt.transl.x - temp_s0->x;
            spB0 = arg0->srt.transl.z - temp_s0->z;
            spA4 = (arg0->srt.transl.y + 30.0f) - temp_s0->y;
            _bss_2 = arctan2_f(spB4, spB0);
            temp_fv0_4 = sqrtf(SQ(spB4) + SQ(spB0));
            arg2->yawDiff = _bss_2 - (arg0->srt.yaw & 0xFFFF);
            if (arg2->yawDiff >= 0x8001) {
                arg2->yawDiff += 0xFFFF0001;
            }
            if (arg2->yawDiff < -0x8000) {
                arg2->yawDiff += 0xFFFF;
            }
            arg2->pitchDiff = -arctan2_f(spA4, temp_fv0_4);
            arg2->rollDiff = 0;
            arg2->unk58 = 0.0f;
            arg2->unk24 = 0.083333336f;
            arg2->unk62 = 5;
            temp_fp->unk354.unk16 = func_80034804(arg0, 0)[1];
            temp_fp->unk378.unk16 = 0;
            temp_fp->unk354.unk14 = arg2->yawDiff;
            temp_fp->unk378.unk14 = arg2->pitchDiff;
            _bss_0 = 0;
            sp6C[0] = temp_s0->x;
            sp6C[1] = temp_s0->y;
            sp6C[2] = temp_s0->z;
            if (arg0->curModAnimId != 0) {
                func_80023D30(arg0, 0, 0.0f, 0U);
            }
            return 1;
        } else if (arg2->unk62 == 5) {
            arg2->unk7A &= 0xFFFB;
            func_8002674C(arg0);
            temp_s0_2 = func_80034804(arg0, 0);
            if (_bss_0 == 3) {
                if ((arg2->unk58 >= 1.0f) && (gDLL_2_Camera->vtbl->func18.asVoidS32() == 0)) {
                    if (arg3 == 0) {
                        arg2->unk62 = 0;
                    } else {
                        arg2->unk62 = 6;
                    }
                    if (temp_fp->unk858 != NULL) {
                        gDLL_18->vtbl->func4(arg0, temp_fp, 0x24);
                        return 1;
                    }
                    gDLL_18->vtbl->func4(arg0, temp_fp, 1);
                    return 1;
                }
                temp_fv0_2 = arg2->unk58;
                arg2->unk58 += (arg2->unk24 * delayFloat);
                if (arg2->unk58 > 1.0f) {
                    arg2->unk58 = 1.0f;
                }
                temp_fv0_2 = arg2->unk58 - temp_fv0_2;
                arg0->srt.yaw += (s16) (temp_fv0_2 * arg2->yawDiff);
                temp_s0_2[1] = _bss_2 - (arg0->srt.yaw & 0xFFFF);
                if (temp_s0_2[1] >= 0x8001) {
                    temp_s0_2[1] += 0xFFFF0001;
                }
                if (temp_s0_2[1] < -0x8000) {
                    temp_s0_2[1] += 0xFFFF;
                }
            } else {
                _bss_0 |= func_800343B8((Unk80032CF8 *)&temp_fp->unk354, temp_s0_2, 100.0f, 2000.0f);
                _bss_0 |= func_80034518((Unk80032CF8 *)&temp_fp->unk378, temp_s0_2, 100.0f, 2000.0f) * 2;
            }
            return 1;
        } else if (arg2->unk62 == 6) {
            if (arg3 == 0) {
                arg2->unk62 = 0;
                return 0;
            }
            func_8002674C(arg0);
            return 0;
        } else {
            if (arg2->unk62 != 1) {
                arg2->unk4C.x = arg0->srt.transl.x;
                arg2->unk4C.y = arg0->srt.transl.y;
                arg2->unk4C.z = arg0->srt.transl.z;
                *temp_data_528 = 10000.0f;
                *temp_data_52C = 0;
            }
            spC8 = 1;
            arg2->unk7A = 0;
            arg2->unk62 = 1;
            sp9C = sqrtf(SQ(arg2->unk4C.x - arg0->srt.transl.x) + SQ(arg2->unk4C.z - arg0->srt.transl.z));
            spAC = arg1->srt.transl.x - arg2->unk4C.x;
            spA8 = arg1->srt.transl.z - arg2->unk4C.z;
            temp_fv0_4 = sqrtf(SQ(spAC) + SQ(spA8));
            if (sp9C <= *temp_data_528) {
                *temp_data_52C += 1;
            }
            if ((temp_fv0_4 <= sp9C) || (*temp_data_52C >= 6)) {
                var_v0 = arg0->srt.yaw - (arg1->srt.yaw & 0xFFFF & 0xFFFF);
                if (var_v0 >= 0x8001) {
                    var_v0 += 0xFFFF0001;
                }
                if (var_v0 < -0x8000) {
                    var_v0 += 0xFFFF;
                }
                if (var_v0 >= 0x4001) {
                    var_v0 = 0x4000;
                }
                if (var_v0 < -0x4000) {
                    var_v0 = -0x4000;
                }
                arg0->srt.yaw -= (var_v0 * delayByte) >> 3;
                if (*temp_data_52C >= 7) {
                    var_v0 = 0;
                }
                if ((var_v0 < 0x100) && (var_v0 >= -0xFF)) {
                    arg0->curModAnimIdLayered = -1;
                    arg2->unk7A = arg2->unk7C;
                    arg2->unk62 = 0;
                    arg2->unk66 = arg2->unk64 - 1;
                    spC8 = 0;
                } else {
                    temp_fp->unk288 = 0.0f;
                    temp_fp->unk284 = 0.0f;
                    gDLL_18->vtbl->func3(arg0);
                    temp_fp->unk310 = 0;
                    temp_fp->unk30C = 0;
                    arg0->unk0xdc = 0;
                    temp_fp->unk324 = 0;
                    temp_fp->unk25B = 1;
                    temp_fp->unk8B8 = 0;
                    gDLL_18->vtbl->func1(arg0, temp_fp, delayFloat, delayFloat, (u32) _bss_58, (u32) _bss_19C);
                }
            } else {
                spAC = spAC / temp_fv0_4;
                spA8 = spA8 / temp_fv0_4;
                temp_fp->unk288 = -spAC * 40.0f;
                temp_fp->unk284 = spA8 * 40.0f;
                arg0 = arg0;
                arg0->srt.transl.x = arg2->unk4C.x + (sp9C * spAC);
                arg0->srt.transl.z = arg2->unk4C.z + (sp9C * spA8);
                gDLL_18->vtbl->func3(arg0);
                temp_fp->unk310 = 0;
                temp_fp->unk30C = 0;
                arg0->unk0xdc = 0;
                temp_fp->unk324 = 0;
                temp_fp->unk25B = 1;
                temp_fp->unk8B8 = 0;
                gDLL_18->vtbl->func1(arg0, temp_fp, delayFloat, delayFloat, (u32) _bss_58, (u32) _bss_19C);
            }
            *temp_data_528 = sp9C;
        }
        if (arg2->unk62 == 0) {
            gDLL_18->vtbl->func4(arg0, temp_fp, 1);
        }
    } else {
        arg2->unk7A |= arg2->unk7C & 0xFBFF;
        temp_fp->unk340 = 0;
        temp_fp->unk324 = 0;
        temp_fp->unk310 = 0;
        temp_fp->unk30C = 0;
        temp_fp->unk25B = 0;
        temp_fp->unk288 = 0.0f;
        temp_fp->unk284 = 0.0f;
        for (var_s1 = 0; var_s1 < arg2->unk98; var_s1++) {
            switch (arg2->unk8E[var_s1]) {
            case 3:
                objects = obj_get_all_of_type(0xB, &spC0);
                for (var_s1 = 0; var_s1 < spC0; var_s1++) {
                    var_s0 = objects[var_s1];
                    if (
                        var_s0->id == OBJ_IMSnowBike
                        || var_s0->id == OBJ_CRSnowBike
                        || var_s0->id == OBJ_BWLog
                        || var_s0->id == OBJ_DIMSnowHorn1
                        || var_s0->id == OBJ_DR_EarthWarrior
                        || var_s0->id == OBJ_DR_CloudRunner
                    ) {
                        temp_fp->unk858 = var_s0;
                    }
                }
                if (var_s0 != NULL) {
                    var_s0 = temp_fp->unk858;
                    temp_fp->unk728 = 1.0f;
                    temp_fp->unk72C.x = temp_fp->unk7EC.x;
                    temp_fp->unk72C.y = temp_fp->unk7EC.y;
                    temp_fp->unk72C.z = temp_fp->unk7EC.z;
                    ((DLL_Unknown *)var_s0->dll)->vtbl->func[14].withTwoArgs(var_s0, 2);
                    arg0->srt.flags |= 8;
                    arg0->ptr0x64->flags |= 0x1000;
                    arg2->unk7A &= 0xFFFB;
                    temp_v1_8 = var_s0->id;
                    // incorrect branching logic but this helps match for now
                    if (temp_v1_8 != OBJ_BWLog) {
                        if ((temp_v1_8 == OBJ_IMSnowBike) || (temp_v1_8 == OBJ_CRSnowBike)) {
                            temp_fp->unk770 = 3;
                            temp_fp->unk76C = _data_158;
                            func_80023D30(arg0, 0x17, 0.0f, 1U);
                        } else {
                        temp_fp->unk770 = 3;
                        temp_fp->unk76C = _data_188;
                        }
                    } else {
                            temp_fp->unk770 = 4;
                            temp_fp->unk76C = _data_170;
                            func_80023D30(arg0, 0xF5, 0.0f, 1U);
                    }
                    gDLL_18->vtbl->func4(arg0, temp_fp, 0x24);
                }
                break;
            case 2:
                gDLL_2_Camera->vtbl->func8(0, 1);
                gDLL_3_Animation->vtbl->func19(0x54, 4, 0, 0);
                var_s0 = temp_fp->unk858;
                if (var_s0 != NULL) {
                    ((DLL_Unknown *)var_s0->dll)->vtbl->func[14].withTwoArgs(var_s0, 0);
                    arg0->srt.flags &= 0xFFF7;
                    arg0->ptr0x64->flags &= ~0x1000;
                    var_s0 = NULL;
                    arg2->unk7A |= 4;
                    temp_fp->unk858 = NULL;
                    arg0->curModAnimIdLayered = -1;
                    gDLL_18->vtbl->func4(arg0, temp_fp, 1);
                }
                break;
            case 4:
                var_s0 = temp_fp->unk858;
                gDLL_3_Animation->vtbl->func19(0x57, 0, 0, 0);
                temp_fp->unk76C = NULL;
                if ((var_s0 != NULL) && (var_s0->id == 0x22)) {
                    gDLL_18->vtbl->func4(arg0, temp_fp, 0x22);
                } else {
                    gDLL_18->vtbl->func4(arg0, temp_fp, 0x24);
                }
                break;
            case 11:
                var_s0 = temp_fp->unk858;
                if ((var_s0 != NULL) && (var_s0->id == 0x416)) {
                    gDLL_2_Camera->vtbl->func8(0, 0x69);
                    gDLL_3_Animation->vtbl->func19(0x54, 4, 0, 0);
                } else if ((var_s0 != NULL) && (var_s0->id == 0x419)) {
                    gDLL_3_Animation->vtbl->func19(0x65, 0, 0, 0);
                } else {
                    gDLL_2_Camera->vtbl->func8(0, 0x1D);
                    gDLL_3_Animation->vtbl->func19(0x54, 4, 0, 0);
                }
                break;
            case 6:
                gDLL_3_Animation->vtbl->func19(0x56, 0, 0, 0);
                gDLL_18->vtbl->func4(arg0, temp_fp, 0x23);
                break;
            case 7:
                arg2->unk7A &= 0xFFFC;
                dll_210_func_1D4E0(arg0, 1);
                arg0->srt.flags |= 8;
                break;
            case 8:
                arg2->unk7A = arg2->unk7C;
                dll_210_func_1D4E0(arg0, 0);
                arg0->srt.flags &= ~8;
                break;
            case 10:
                temp_fp->unk878 = 2;
                temp_fp->unk8A9 = 2;
                temp_fp->unk8A8 = 1;
                break;
            case 12:
                sp6A = gDLL_22_Subtitles->vtbl->func_214C((s32) arg0->unk0xc4->id);
                gDLL_22_Subtitles->vtbl->func_2248(1U);
                gDLL_22_Subtitles->vtbl->func_368((u16) sp6A);
                arg0->unk0xc4 = NULL;
                break;
            case 13:
                tempObj = arg0->unk0xc4;
                gDLL_3_Animation->vtbl->func30(tempObj->id, tempObj, 0);
                dll_210_func_1DB6C(arg0->unk0xc4, 29.0f);
                gDLL_3_Animation->vtbl->func17(arg0->unk0xdc, arg0, -1);
                break;
            case 14:
                if (*_data_14 == 1) {
                    *_data_18 = -1;
                    tempObj = temp_fp->unk85C;
                    ((DLL_Unknown*)tempObj->dll)->vtbl->func[12].withTwoArgs(tempObj, (s16)arg0->unk_0xe0);
                    arg2->unk9D |= 4;
                    arg0->unk0xc4 = NULL;
                    return 4;
                }
                if ((*_data_14 != 0) || (arg0->unk0xdc < 0)) {
                    arg2->unk9D |= 4;
                    arg0->unk0xc4 = NULL;
                } else {
                    *_data_18 = arg0->unk_0xe0;
                }
                arg0->unk0xdc -= 1;
                break;
            case 17:
                dll_210_func_1D8B8(arg0);
                break;
            case 15:
                func_8005B5B8(arg0, NULL, 1);
                break;
            case 16:
                sp60 = 400.0f;
                tempObj = obj_get_nearest_type_to(7, arg0, &sp60);
                if (tempObj != NULL) {
                    func_8005B5B8(arg0, tempObj, 1);
                }
                break;
            case 23:
                dll_210_func_1D2A8(arg0, NULL);
                break;
            case 20:
                temp_fp->flags |= 0x40000;
                break;
            case 21:
                temp_fp->flags &= 0xFFFBFFFF;
                break;
            case 22:
                temp_fp->flags |= 0x20000;
                break;
            case 18:
                temp_fp->flags |= 0x8000;
                break;
            case 19:
                menu_set(1);
                break;
            case 25:
                dll_210_func_9F1C(arg0, 1);
                break;
            }
        }

        if (temp_fp->unk708 != NULL) {
            if (temp_fp->unk708->def->unkAA >= 0) {
                if (arg2->unk8D == 0x1A) {
                    gDLL_1_UI->vtbl->ui_func_1338.withThreeArgs(temp_fp->unk708->def->unkAA, 0xA0, 0x8C);
                }
            } else {
                gDLL_1_UI->vtbl->ui_func_130c.withThreeArgs(temp_fp->unk708->def->unkA2, 0xA0, 0x8C);
            }
            if (arg2->unk8D == 1) {
                gDLL_3_Animation->vtbl->func19(0x54, 3, 0, 0);
                obj_send_mesg(temp_fp->unk708, 0x7000BU, arg0, NULL);
                temp_fp->unk708 = NULL;
            }
        }
    }
    temp_a0_4 = temp_fp->unk858;
    if ((temp_a0_4 != NULL) && (((DLL_Unknown*)temp_a0_4->dll)->vtbl->func[13].withOneArgS32(temp_a0_4) == 2)) {
        arg2->unk7A &= 0xFFFC;
    }
    ((void (*)(Object*, PlayerState*, f32)) temp_fp->unk3BC)(arg0, temp_fp, delayFloat);
    dll_210_func_1BC0(arg0, temp_fp);
    return spC8;
}
#endif

// offset: 0x60A8 | func: 28
void dll_210_func_60A8(Object* arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {
    PlayerState* sp24;

    sp24 = arg0->state;
    arg0->unk0xb0 &= 0xEFFF;
    func_8002674C(arg0);
    arg0->speed.y = 0.0f;
    dll_210_func_7260(arg0, sp24);
    sp24->unk8B7 = 2;
    sp24->unk834 = 0.0f;
    gDLL_22_Subtitles->vtbl->func_2248(0U);
    if (arg0->modelInstIdx == 1) {
        func_80023A18(arg0, 0);
    }
}

// offset: 0x618C | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_618C.s")

// offset: 0x63F0 | func: 30
f32 dll_210_func_63F0(PlayerState* arg0, f32 arg1) {
    switch (arg0->unk8B7) {
    case 1:
        arg0->unk800 *= 0.85f;
        if (arg0->unk800 < arg0->unk804) {
            arg0->unk800 = arg0->unk804;
        }
        arg1 *= arg0->unk800;
        break;
    case 2:
        arg0->unk800 += 0.02f;
        if (arg0->unk800 >= 1.0f) {
            arg0->unk800 = 1.0f;
            arg0->unk8B7 = 0;
        }
        arg1 *= arg0->unk800;
        break;
    }
    return arg1;
}

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
// Requires these as static:
// dll_210_func_1DE64 (matched)
// dll_210_func_1DE50 (matched)
// dll_210_func_90A0
// dll_210_func_955C (matched)
// dll_210_func_98CC (matched)

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
                gDLL_18->vtbl->func4(obj, state, 1);
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
                        gDLL_3_Animation->vtbl->func28(obj->unk0xb4, temp);
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
                gDLL_3_Animation->vtbl->func28(obj->unk0xb4, temp);
            }
            break;
    }
    state->unk87C = arg2;
}
#endif

// offset: 0x7180 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7180.s")

// offset: 0x7260 | func: 35
static void dll_210_func_7260(Object* arg0, PlayerState* arg1) {
    arg1->unk874 = (s16) arg0->srt.yaw;
    gDLL_27_HeadTurn->vtbl->head_turn_func_fb8(arg0, &arg1->unk4);
    arg0->objhitInfo->unk_0x10.x = arg0->srt.transl.x;
    arg0->objhitInfo->unk_0x10.y = arg0->srt.transl.y;
    arg0->objhitInfo->unk_0x10.z = arg0->srt.transl.z;
    arg0->objhitInfo->unk_0x20.x = arg0->positionMirror.x;
    arg0->objhitInfo->unk_0x20.y = arg0->positionMirror.y;
    arg0->objhitInfo->unk_0x20.z = arg0->positionMirror.z;
}

// offset: 0x7300 | func: 36
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7300.s")
#else
// https://decomp.me/scratch/t5pag
// probably incorrect definition
typedef struct Unk7300 {
    MtxF mtx;
    f32 unk40;
    u8 pad44;
    u8 unk45;
    s8 unk46;
    u8 unk47;
    f32 unk48;
    u32 pad4C;
    s8 unk50;
} Unk7300;
s32 dll_210_func_7300(Object* arg0, PlayerState* arg1, Unk7300* arg2, Unk7300* arg3, Vec3f* arg4, f32 arg5) {
    Vec4f spC;
    f32 temp_fv1;
    s32 var_v0;
    PlayerState *state;

    state = arg0->state;

    arg3->mtx.m[1][3] = arg2->mtx.m[1][3];
    arg3->mtx.m[2][0] = arg2->mtx.m[2][0];
    arg3->mtx.m[2][1] = arg2->mtx.m[2][1];
    arg3->mtx.m[2][2] = arg2->mtx.m[2][2];
    spC.x = -arg3->mtx.m[2][1];
    spC.y = 0.0f;
    spC.z = arg3->mtx.m[1][3];
    spC.w = -(arg2->mtx.m[1][1] * spC.z) + (spC.x * arg2->mtx.m[0][1]);
    temp_fv1 = *_data_70 + 0.5f;
    if ((spC.x * arg4->x + spC.y * arg4->y + arg4->z * spC.z + spC.w) < temp_fv1) {
        return 0;
    }
    spC.x = -spC.x;
    spC.z = -spC.z;
    spC.w = -(arg2->mtx.m[1][2] * spC.z) + (spC.x * arg2->mtx.m[0][2]);
    if ((spC.x * arg4->x + spC.y * arg4->y + arg4->z * spC.z + spC.w) < temp_fv1) {
        return 0;
    }
    arg3->mtx.m[2][3] = arg4->x;
    arg3->mtx.m[3][0] = arg4->y;
    arg3->mtx.m[3][1] = arg4->z;
    arg3->mtx.m[3][3] = 0.0f;
    arg3->mtx.m[3][2] = arg1->unk7EC.x;
    arg3->unk40 = arg1->unk7EC.z;
    temp_fv1 = arg2->mtx.m[3][3];
    arg3->mtx.m[0][1] = ((arg2->unk40 - temp_fv1) * arg2->unk48) + temp_fv1;
    arg3->mtx.m[0][2] = arg0->positionMirror2.y;
    arg3->mtx.m[0][0] = arg3->mtx.m[0][1] - arg3->mtx.m[0][2];
    arg3->unk46 = arg2->unk50;
    if (arg2->unk50 == 6) {
        if (state->unk260 & 0x10) {
            if (arg5 < 9.0f) {
                if ((arg3->mtx.m[0][0] <= 64.0f) && (arg3->mtx.m[0][0] > 32.0f)) {
                    return 2;
                }
                if ((arg3->mtx.m[0][0] <= 32.0f) && (*_bss_1C8 <= arg3->mtx.m[0][0])) {
                    return 3;
                }
                return 0;
            }
            return 0;
        }
        return 6;
    }
    if (*_bss_1DC < arg3->mtx.m[0][0]) {
        arg3->mtx.m[0][0] = *_bss_1DC;
    }
    return 3;
}
#endif

// offset: 0x75B0 | func: 37
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_75B0.s")
#else
// https://decomp.me/scratch/lhw69
s32 dll_210_func_75B0(Object* arg0, Unk7300* arg1, Unk7300* arg2, Vec3f* arg3, f32 arg4, f32 arg5) {
    Vec4f sp4;
    f32 temp_ft0;
    f32 temp_ft2;
    f32 temp_ft3;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_fv1;
    PlayerState *state;

    state = arg0->state;
    arg2->mtx.m[1][3] = arg1->mtx.m[1][3];
    arg2->mtx.m[2][0] = arg1->mtx.m[2][0];
    arg2->mtx.m[2][1] = arg1->mtx.m[2][1];
    arg2->mtx.m[1][3] = -arg2->mtx.m[1][3];
    arg2->mtx.m[2][0] = -arg2->mtx.m[2][0];
    arg2->mtx.m[2][1] = -arg2->mtx.m[2][1];
    arg2->mtx.m[2][2] = -arg1->mtx.m[2][2];
    arg2->mtx.m[2][3] = arg3->x;
    arg2->mtx.m[3][0] = arg3->y;
    arg2->mtx.m[3][1] = arg3->z;
    temp_fv0 = arg1->mtx.m[0][3];
    arg2->mtx.m[0][1] = ((arg1->mtx.m[1][0] - temp_fv0) * arg1->unk48) + temp_fv0;
    temp_ft0 = -arg2->mtx.m[2][1];
    arg2->mtx.m[0][2] = arg1->mtx.m[0][3];
    arg2->mtx.m[0][0] = arg2->mtx.m[0][1] - arg2->mtx.m[0][2];
    arg2->unk46 = arg1->unk50;
    sp4.x = temp_ft0;
    sp4.y = 0.0f;
    sp4.z = arg2->mtx.m[1][3];
    temp_ft2 = (arg1->mtx.m[1][1] * sp4.z) + (temp_ft0 * arg1->mtx.m[0][1]);
    sp4.w = -temp_ft2;
    temp_ft5 = -temp_ft0;
    temp_fv1 = ((arg3->z * sp4.z) + ((temp_ft0 * arg3->x) + (0.0f * arg3->y))) - temp_ft2;
    sp4.x = temp_ft5;
    temp_ft3 = -sp4.z;
    sp4.z = temp_ft3;
    temp_ft4 = (arg1->mtx.m[1][2] * temp_ft3) + (temp_ft5 * arg1->mtx.m[0][2]);
    sp4.w = -temp_ft4;
    if (temp_fv1 < (((arg3->z * temp_ft3) + ((temp_ft5 * arg3->x) + (0.0f * arg3->y))) - temp_ft4)) {
        arg2->unk47 = 0;
    } else {
        arg2->unk47 = 1;
    }
    if (arg4 <= (state->unk278 * arg5) || arg4 <= 3.5f) {
        if ((arg1->unk50 == 2) || (arg1->unk50 == 0x11)) {
            return 4;
        }
        if (state->unk278 >= 0.94f) {
            return 5;
        }
        if (arg1->unk50 != 4) {
            return 4;
        }
        return 5;
    }
    return 0;
}
#endif

// offset: 0x77DC | func: 38
typedef struct UnkArg1 {
    Object *unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u32 pad10;
    f32 unk14;
    f32 unk18;
    Vec4f unk1C;
    u8 pad2C[0x38 - 0x2C];
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x52 - 0x40];
    s8 unk52;
} UnkArg1;

s32 dll_210_func_77DC(s32 arg0, UnkArg1* arg1, UnkArg2* arg2, Vec3f* arg3) {
    arg2->unk0.x = arg1->unk1C.x;
    arg2->unk0.y = arg1->unk1C.y;
    arg2->unk0.z = arg1->unk1C.z;
    arg2->unk0.w = arg1->unk38;
    arg2->unk2D = 0;
    if (arg0 != 0) {
        inverse_transform_point_by_object(arg3->x, arg3->y, arg3->z, &arg2->unk10.x, &arg2->unk10.y, &arg2->unk10.z, arg1->unk0);
        arg2->unk28 = (s32) arg1->unk0;
    } else {
        arg2->unk10.x = arg3->x;
        arg2->unk10.y = arg3->y;
        arg2->unk10.z = arg3->z;
        arg2->unk28 = 0;
    }
    return 1;
}

// offset: 0x78A8 | func: 39
s32 dll_210_func_78A8(Object* arg0, PlayerState* arg1, UnkArg1* arg2, UnkArg3* arg3, s32 arg4) {
    arg3->unk3 = 0;
    arg3->unk48 = ((arg2->unk8 - arg2->unk4) * 0.5f) + arg2->unk4;
    arg3->unk4C = arg2->unkC;
    arg3->unk50 = ((arg2->unk18 - arg2->unk14) * 0.5f) + arg2->unk14;
    if (arg4 != 0) {
        arg3->unk28.x = -arg2->unk1C.x;
        arg3->unk28.y = -arg2->unk1C.y;
        arg3->unk28.z = -arg2->unk1C.z;
        arg3->unk28.w = -arg2->unk1C.w;
    } else {
        arg3->unk28.x = arg2->unk1C.x;
        arg3->unk28.y = arg2->unk1C.y;
        arg3->unk28.z = arg2->unk1C.z;
        arg3->unk28.w = arg2->unk1C.w;
    }
    arg3->unk38.x = -arg2->unk1C.z;
    arg3->unk38.y = 0.0f;
    arg3->unk38.z = arg2->unk1C.x;

    arg3->unk44 = -((arg3->unk38.z * arg3->unk50) + (arg3->unk48 * arg3->unk38.x));

    arg3->unk54.x = arg1->unk7EC.x;
    arg3->unk54.y = 0.0f;
    arg3->unk54.z = arg1->unk7EC.z;

    arg3->unk18 = (arg3->unk38.z * arg3->unk54.z) + (arg3->unk54.x * arg3->unk38.x) + arg3->unk44;
    if ((arg3->unk18 > -9.0f) && (arg3->unk18 < 9.0f)) {
        arg3->unk8 = arg2->unkC;
        arg3->unk4 = arg2->unk3C;
        arg3->unk1 = ((arg3->unk4 - arg3->unk8) + 2.2f) / 8.8f;
        arg3->unkC = (arg3->unk4 - arg3->unk8) / arg3->unk1;
        if ((arg3->unk4 - 10.0f) < arg0->srt.transl.y) {
            arg3->unk0 = arg3->unk1 - 3;
        } else {
            arg3->unk0 = 1;
        }
        return 1;
    }
    return 0;
}

// offset: 0x7AAC | func: 40
s32 dll_210_func_7AAC(Object* arg0, PlayerState* arg1, UnkArg1* arg2, Vec3f* arg3, UnkArg3* arg4, s32 arg5) {
    arg4->unk44 = arg3->x;
    arg4->unk48 = arg2->unkC;
    arg4->unk4C = arg3->z;
    arg4->unk50 = arg1->unk7EC.x;
    arg4->unk54.x = 0.0f;
    arg4->unk54.y = arg1->unk7EC.z;
    if (arg5 != 0) {
        arg4->unk1 = 1;
    } else {
        arg4->unk1 = 0;
    }
    arg4->unk24 = arg2->unk1C.x;
    arg4->unk28.x = arg2->unk1C.y;
    arg4->unk28.y = arg2->unk1C.z;
    arg4->unk28.z = arg2->unk1C.w;
    arg4->unk28.w = -arg2->unk1C.z;
    arg4->unk38.x = 0.0f;
    arg4->unk38.y = arg2->unk1C.x;
    arg4->unk38.z = -((arg4->unk38.y * arg4->unk4C) + (arg4->unk44 * arg4->unk28.w));
    arg4->unk8 = arg2->unkC;
    arg4->unk4 = arg2->unk3C;
    arg4->unk2 = (s8) ((s32) (arg2->unk52 & 0x70) >> 4);
    return 1;
}

// offset: 0x7B98 | func: 41
void dll_210_func_7B98(Object* arg0, UnkArg1* arg1, UnkArg2* arg2) {
    arg2->unk0.x = arg1->unk1C.x;
    arg2->unk0.y = arg1->unk1C.y;
    arg2->unk0.z = arg1->unk1C.z;
    arg2->unk0.w = arg1->unk1C.w;
}

// offset: 0x7BC4 | func: 42
s32 dll_210_func_7BC4(Object* arg0, PlayerState* arg1, u32* arg2, UnkArg4* arg3) {
    Object* temp_a0;
    s32 var_v1;
    s32 temp;

    temp_a0 = arg3->unk38;
    ((DLL_Unknown*)temp_a0->dll)->vtbl->func[7].withTwoArgs(temp_a0, arg3->padC);
    temp_a0 = arg3->unk38;
    ((DLL_Unknown*)temp_a0->dll)->vtbl->func[8].withFiveArgsCustom(temp_a0, arg3->unk48, &arg3->unk1C, &arg3->unk1C.y, &arg3->unk1C.z);
    temp_a0 = arg3->unk38;
    arg3->unk54 = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[12].withOneArgS32(temp_a0);
    arg3->unk46 = 0;
    arg3->unk4C = arg3->unk48;
    arg3->unk0.y = arg3->unk1C.y;
    arg3->unk0.z = arg0->srt.transl.y;
    arg3->unk0.x = arg3->unk1C.y - arg3->unk0.z;
    var_v1 = arg0->srt.yaw - (arg3->unk54 & 0xFFFF);
    if (var_v1 >= 0x8001) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    arg3->unk45 = !(var_v1 >= 0x3FFD) && !(var_v1 < -0x3FFC);
    return 1;
}

// offset: 0x7CF8 | func: 43
void dll_210_func_7CF8(PlayerState* arg0, Vec3f* arg1) {
    s32 sp20;

    sp20 = arctan2_f(arg0->unk288, -arg0->unk284) - arg0->unk324;
    arg1->x = -fsin16_precise(sp20);
    arg1->y = 0.0f;
    arg1->z = -fcos16_precise(sp20);
}

// offset: 0x7DA0 | func: 44
void dll_210_func_7DA0(Object* arg0, PlayerState* arg1, Vec3f* arg2) {
    f32 f0;

    f0 = sqrtf(SQ(arg0->speed.x) + SQ(arg0->speed.z));
    arg2->y = 0.0f;
    if (f0 > 0.01f) {
        arg2->x = arg0->speed.x;
        arg2->z = arg0->speed.z;
        arg2->x /= f0;
        arg2->z /= f0;
        return;
    }
    arg2->x = -arg1->unk274->x;
    arg2->z = -arg1->unk274->z;
}

// offset: 0x7E6C | func: 45
// REquires these as static:
// dll_210_func_7300
// dll_210_func_75B0
// dll_210_func_77DC (matched)
// dll_210_func_78A8 (matched)
// dll_210_func_7AAC (matched)
// dll_210_func_7B98 (matched)
// dll_210_func_7CF8 (matched)
// dll_210_func_7DA0 (matched)
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_7E6C.s")

// offset: 0x8AE0 | func: 46
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_8AE0.s")

// offset: 0x8EA4 | func: 47
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_8EA4.s")

// offset: 0x90A0 | func: 48
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_90A0.s")
#else
void dll_210_func_90A0(Object* arg0, PlayerState* arg1, f32 arg2) {
    f32 pad;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    Camera* mainCam;
    ObjCreateInfo* createInfo;
    Object* temp_a0;
    Object* temp_v0_2;
    Object* temp_v1;
    Object* var_s3;
    f32 temp_fs0;
    f32 temp_fs2;
    Vec3f sp104;
    f32 temp_fs1;
    MtxF spC0;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;
    f32 temp;
    s32 var_s4;
    PlayerState* temp_fp;
    SRT sp90;

    var_s3 = NULL;
    var_s4 = 1;
    temp_fp = arg0->state;
    mainCam = get_main_camera();
    gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x2B8U, 0x7FU, NULL, NULL, 0, NULL);
    while (var_s4) {
        createInfo = obj_alloc_create_info(0x24, 0x14B);
        createInfo->loadParamA = 2;
        createInfo->loadParamB = 1;
        createInfo->loadDistance = 0xFF;
        createInfo->fadeDistance = 0xFF;
        if (arg1->unk2C8 != NULL) {
            createInfo->x = arg0->linkedObject->srt.transl.x;
            createInfo->y = arg0->linkedObject->srt.transl.y;
            createInfo->z = arg0->linkedObject->srt.transl.z;
        } else {
            createInfo->x = mainCam->srt.transl.x;
            createInfo->y = mainCam->srt.transl.y;
            createInfo->z = mainCam->srt.transl.z;
        }
        temp_a0 = arg0->linkedObject;
        ((s8*)createInfo)[0x19] = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[16].withOneArgS32(temp_a0);
        temp_v0_2 = obj_create(createInfo, 5U, -1, -1, NULL);
        if (temp_v0_2 != NULL) {
            temp_v0_2->srt.flags |= 0x2000;
            temp_v1 = arg1->unk2C8;
            if (temp_v1 != NULL) {
                temp_a0 = arg0->linkedObject;
                temp2 = temp_v1->srt.transl.x - temp_a0->srt.transl.x;
                temp_fs0 = temp_v1->srt.transl.y - temp_a0->srt.transl.y;
                temp_fv1 = temp_v1->srt.transl.z - temp_a0->srt.transl.z;
                sp90.transl.x = 0.0f;
                sp90.transl.y = 0.0f;
                sp90.transl.z = 0.0f;
                sp90.scale = 1.0f;
                // @fake
                if (var_s3) {}
                sp90.yaw = arg0->srt.yaw;
                var_s3 = temp_v1;
                sp90.pitch = arctan2_f(temp_fs0, sqrtf(SQ(temp2) + SQ(temp_fv1)));
                sp90.roll = 0;
                if (arg0->parent != NULL) {
                    sp90.yaw += arg0->parent->srt.yaw;
                }
                matrix_from_srt(&spC0, &sp90);
                vec3_transform(&spC0, 0.0f, 0.0f, -5.0f, &temp_v0_2->speed.x, &temp_v0_2->speed.y, &temp_v0_2->speed.z);
                temp_v0_2->positionMirror.x = temp_v0_2->srt.transl.x;
                temp_v0_2->positionMirror.y = temp_v0_2->srt.transl.y;
                temp_v0_2->positionMirror.z = temp_v0_2->srt.transl.z;
                temp_v0_2->srt.yaw = arg0->srt.yaw;
                temp_v0_2->srt.pitch = 0;
            } else {
                temp_v0_2->srt.yaw = mainCam->srt.yaw;
                temp_fs1 = camera_get_fov() * 91.022f;
                temp_fv0 = fsin16_precise(temp_fs1);
                temp_fv0 /= fcos16_precise(temp_fs1);
                temp_fs1 = (100.0f * temp_fv0);
                temp3 = -(((temp_fp->aimX - 0xA0) / 160.0f) * 1.333333f);
                temp_fs1 *= temp3;
                temp_fs2 = (100.0f * temp_fv0);
                temp_fs2 *= ((temp_fp->aimY - 0x78) / 120.0f);
                temp = 100.0f;
                temp_fv0 = sqrtf(SQ(temp_fs1) + SQ(temp_fs2) + SQ(temp));
                sp104.x = temp_fs1 / temp_fv0;
                sp104.y = temp_fs2 / temp_fv0;
                sp104.z = temp / temp_fv0;
                vec3_transform_no_translate(camera_get_viewmtx2(), &sp104, &sp104);
                temp_v0_2->speed.x = sp104.x * -5.0f;
                temp_v0_2->speed.y = sp104.y * -5.0f;
                temp_v0_2->speed.z = sp104.z * -5.0f;

                temp_fv0_2 = mainCam->srt.transl.x;
                temp_v0_2->positionMirror.x = temp_fv0_2;
                temp_v0_2->srt.transl.x = temp_fv0_2;

                temp_fv0_2 = mainCam->srt.transl.y;
                temp_v0_2->positionMirror.y = temp_fv0_2;
                temp_v0_2->srt.transl.y = temp_fv0_2;

                temp_fv0_2 = mainCam->srt.transl.z;
                temp_v0_2->positionMirror.z = temp_fv0_2;
                temp_v0_2->srt.transl.z = temp_fv0_2;

                temp_v0_2->srt.pitch = 0;
            }
            temp_v0_2->unk0xdc = 0xBE;
            temp_v0_2->unk_0xe0 = (s32) var_s3;
        }
        var_s4--;
    }
}

#endif

// offset: 0x955C | func: 49
void dll_210_func_955C(Object* arg0, PlayerState* arg1, f32 arg2) {
    SRT spE8;
    ObjCreateInfo* temp_v0;
    Object* temp_a0;
    Object* temp_v0_2;
    MtxF sp9C;
    Object* var_s2;
    f32 temp_fs0;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 temp_s5;
    s32 var_s4;

    var_s4 = 1;
    gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x2B8U, 0x7FU, NULL, NULL, 0, NULL);
    while (var_s4) {
        temp_v0 = obj_alloc_create_info(0x24, 0x4C9);
        temp_v0->loadParamA = 2;
        temp_v0->loadParamB = 1;
        temp_v0->loadDistance = 0xFF;
        temp_v0->fadeDistance = 0xFF;
        temp_v0->x = arg0->linkedObject->positionMirror.x;
        temp_v0->y = arg0->linkedObject->positionMirror.y;
        temp_v0->z = arg0->linkedObject->positionMirror.z;
        temp_a0 = arg0->linkedObject;
        temp_s5 = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[16].withOneArgS32(temp_a0);
        temp_v0_2 = obj_create(temp_v0, 5U, -1, -1, NULL);
        if (temp_v0_2 != NULL) {
            temp_v0_2->srt.flags |= 0x2000;
            if (arg1->unk2C8 != NULL) {
                var_s2 = arg1->unk2C8;
            } else {
                var_s2 = NULL;
            }
            if (var_s2 != NULL) {
                temp_fv0 = var_s2->srt.transl.x - arg0->srt.transl.x;
                temp_fs0 = var_s2->srt.transl.y - arg0->srt.transl.y;
                temp_fv1 = var_s2->srt.transl.z - arg0->srt.transl.z;
                spE8.yaw = 0;
                spE8.pitch = arctan2_f(temp_fs0, sqrtf((temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1)));
            } else {
                spE8.yaw = func_80034804(arg0, 9)[1];
                spE8.pitch = arg2 * -14336.0f;
            }
            spE8.transl.x = 0.0f;
            spE8.transl.y = 0.0f;
            spE8.transl.z = 0.0f;
            spE8.scale = 1.0f;
            spE8.yaw += arg0->srt.yaw;
            spE8.roll = 0;
            if (arg0->parent != NULL) {
                if (spE8.yaw) {}
                spE8.yaw += arg0->parent->srt.yaw;
            }
            matrix_from_srt(&sp9C, &spE8);
            vec3_transform(&sp9C, 0.0f, 0.0f, temp_s5 * -0.15f, &temp_v0_2->speed.x, &temp_v0_2->speed.y, &temp_v0_2->speed.z);
            temp_v0_2->positionMirror.x = temp_v0_2->srt.transl.x;
            temp_v0_2->positionMirror.y = temp_v0_2->srt.transl.y;
            temp_v0_2->positionMirror.z = temp_v0_2->srt.transl.z;
            temp_v0_2->srt.yaw = arg0->srt.yaw;
            temp_v0_2->srt.pitch = 0;
            temp_v0_2->unk0xdc = 0x15E;
            temp_v0_2->unk_0xe0 = (s32) var_s2;
            gDLL_17->vtbl->func1(temp_v0_2, 0x521, NULL, 4, -1, NULL);
        }
        var_s4 -= 1;
    }
}

// offset: 0x98CC | func: 50
void dll_210_func_98CC(Object* arg0, PlayerState* arg1, f32 arg2) {
    SRT spF8;
    SRT spE0;
    ObjCreateInfo* temp_v0;
    Object* temp_a0;
    Object* spD4;
    MtxF sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    Object* temp_v0_2;
    s32 pad;
    f32 sp70[3];
    Vec3f sp64;
    s32 pad_sp60;
    s32 sp5C;
    f32 var_fv1;
    s32 sp54;
    s32 var_v1;
    s32 sp4C;
    DLL_Unknown* dll;

    gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x2B8U, 0x7FU, NULL, NULL, 0, NULL);
    temp_v0 = obj_alloc_create_info(0x24, 0x434);
    temp_v0->loadParamA = 2;
    temp_v0->loadParamB = 1;
    temp_v0->loadDistance = 0xFF;
    temp_v0->fadeDistance = 0xFF;
    temp_v0->x = arg0->linkedObject->positionMirror.x;
    temp_v0->y = arg0->linkedObject->positionMirror.y;
    temp_v0->z = arg0->linkedObject->positionMirror.z;
    temp_a0 = arg0->linkedObject;
    ((s8*)temp_v0)[0x19] = ((DLL_Unknown *)temp_a0->dll)->vtbl->func[16].withOneArgS32(temp_a0);
    temp_v0_2 = obj_create(temp_v0, 5U, -1, -1, NULL);
    if (temp_v0_2 == NULL) {
        return;
    }

    temp_v0_2->srt.flags |= 0x2000;
    if (arg1->unk2C8 != NULL) {
        spD4 = arg1->unk2C8;
    } else {
        spD4 = NULL;
    }
    if (spD4 != NULL) {
        sp90 = spD4->positionMirror.x - arg0->positionMirror.x;
        sp8C = spD4->positionMirror.y - arg0->positionMirror.y;
        sp88 = spD4->positionMirror.z - arg0->positionMirror.z;
        spF8.yaw = 0;
        spF8.pitch = arctan2_f(sp8C, sqrtf(SQ(sp90) + SQ(sp88)));
    } else {
        spF8.yaw = func_80034804(arg0, 9)[1];
        spF8.pitch = arg2 * -14336.0f;
    }
    spF8.transl.x = 0.0f;
    spF8.transl.y = 0.0f;
    spF8.transl.z = 0.0f;
    spF8.scale = 1.0f;
    if (arg0->parent != NULL) {
        var_v1 = arg0->parent->srt.yaw;
    } else {
        var_v1 = 0;
    }
    spF8.yaw = spF8.yaw + arg0->srt.yaw + var_v1;
    spF8.roll = 0;
    matrix_from_srt(&sp94, &spF8);
    vec3_transform(&sp94, 0.0f, 0.0f, -5.0f, &temp_v0_2->speed.x, &temp_v0_2->speed.y, &temp_v0_2->speed.z);
    temp_v0_2->positionMirror.x = temp_v0_2->srt.transl.x;
    temp_v0_2->positionMirror.y = temp_v0_2->srt.transl.y;
    temp_v0_2->positionMirror.z = temp_v0_2->srt.transl.z;
    temp_v0_2->srt.yaw = arg0->srt.yaw;
    temp_v0_2->unk0xdc = 0xBE;
    temp_v0_2->unk_0xe0 = (s32) spD4;
    sp84 = sqrtf(SQ(temp_v0_2->speed.x) + SQ(temp_v0_2->speed.z));
    spE0.yaw = -0x4000 - arctan2_f(temp_v0_2->speed.z, temp_v0_2->speed.x);
    spE0.pitch = arctan2_f(temp_v0_2->speed.y, sp84) - 0x4000;
    spE0.roll = 0;
    sp64.x = temp_v0_2->speed.x * 300.0f;
    sp64.y = temp_v0_2->speed.y * 300.0f;
    sp64.z = temp_v0_2->speed.z * 300.0f;
    sp64.x += temp_v0_2->srt.transl.x;
    sp64.y += temp_v0_2->srt.transl.y;
    sp64.z += temp_v0_2->srt.transl.z;
    func_80007EE0(&arg0->positionMirror, &sp5C);
    func_80007EE0(&sp64, &sp54);
    if (func_80008048(&sp5C, &sp54, &sp4C, 0, 0) == 0) {
        func_80007E2C(&sp64, &sp4C);
        sp70[0] = sp64.x - temp_v0_2->srt.transl.x;
        sp70[1] = sp64.y - temp_v0_2->srt.transl.y;
        sp70[2] = sp64.z - temp_v0_2->srt.transl.z;
        var_fv1 = sqrtf(SQ(sp70[0]) + SQ(sp70[1]) + SQ(sp70[2]));
    } else {
        var_fv1 = 200.0f;
    }
    spE0.transl.z = 0.0f;
    spE0.transl.y = var_fv1;
    spE0.transl.x = 0.0f;
    spE0.scale = 1.0f;
    dll = dll_load_deferred(0x1043U, 1U);
    dll->vtbl->func[0].withSixArgsCustom(arg0->linkedObject, 1, &spE0, 0x10401, -1, 0);
    dll_unload(dll);
    dll = dll_load_deferred(0x1044U, 1U);
    dll->vtbl->func[0].withSixArgsCustom(arg0->linkedObject, 1, &spE0, 0x10401, -1, 0);
    dll_unload(dll);
}

// offset: 0x9E00 | func: 51
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_9E00.s")
#else
// Matches but has weird bss stuff going on
void dll_210_func_9E00(Object* arg0) {
    static f32 _bss_3C;
    PlayerState* sp2C;
    f32 temp_fv0;

    sp2C = arg0->state;
    temp_fv0 = 40.0f - sp2C->unk844;
    if (_bss_3C < -40.0f) {
        sp2C->unk8BE = 0;
    } else if (temp_fv0 <= 0.0f) {
        _bss_3C -= 0.2f * delayFloat;
    } else {
        _bss_3C = 40.0f;
        gDLL_32->vtbl->func3(arg0, 40.0f - sp2C->unk844, 0x3F4, 0x14, 2);
        gDLL_32->vtbl->func3(arg0, 40.0f - sp2C->unk844, 0x3F7, 0x14, 2);
    }
}
#endif

// offset: 0x9F1C | func: 52
void dll_210_func_9F1C(Object* arg0, s32 arg1) {
    if (arg1 != 0) {
        if (gDLL_29_Gplay->vtbl->restart_is_set() != 0) {
            gDLL_29_Gplay->vtbl->restart_goto();
            return;
        }
        menu_set(9);
        func_80010038(0);
        return;
    }
    func_800267A4(arg0);
    menu_set(9);
    func_80010038(1);
    obj_send_mesg_many(0, 3, arg0, 0xE0000, arg0);
}

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
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_BA38.s")
#else
// Requires dll_210_func_7E6C, dll_210_func_7BC4, dll_210_func_1DAB0, dll_210_func_18E80 as static
// https://decomp.me/scratch/3CYBi
s32 dll_210_func_BA38(Object* arg0, PlayerState* arg1, f32 arg2) {
    PlayerState* spC4;
    u32 sp90[12];
    s8 sp8E;
    s16 sp8C;
    Object* sp88;
    Object* temp_v0_5;
    s32 temp_v0_6;
    f32 sp80 = 200.0f;
    s32 temp_v0;
    u32 sp38[17] = _data_778;
    s8 temp_v0_4;

    spC4 = arg0->state;
    if (spC4->unk834 > 20.0f) {
        if (spC4->unk8BB != 0) {
            temp_v0 = gDLL_2_Camera->vtbl->func3();
            if ((temp_v0 != 0x64) && (temp_v0 != 0x5E)) {
                gDLL_2_Camera->vtbl->func6(0x64, 1, 0, 0, NULL, 0x3C, 0xFF);
                spC4->flags &= 0xFFBFFFFF;
                return 0x3B;
            }
        }
        spC4->unk834 = 20.0f;
    }
    if (spC4->unk708 != NULL && spC4->unk708->id == 0x112) {
        if (arg1->unk310 & 0x8000) {
            return 0x30;
        }
        return 0;
    }
    temp_v0_4 = dll_210_func_7E6C(arg0, spC4, arg1, sp90, arg2, -0x81);
    if (temp_v0_4 == -1) {
        spC4->unk8B5 = -1;
        spC4->unk8B6 = 0U;
    } else if (temp_v0_4 == spC4->unk8B5) {
        spC4->unk8B6++;
        if (spC4->unk8B6 > 0xC8) {
            spC4->unk8B6 = 0xC8;
        }
    } else {
        spC4->unk8B5 = temp_v0_4;
        spC4->unk8B6 = 0U;
    }
    switch (spC4->unk8B5) {
    case 0:
        if (arg1->unk260 & 0x10) {
            return 0x18;
        }
    default:
        break;
    case 12:
        if (arg1->unk260 & 0x10) {
            return 0x1C;
        }
        break;
    case 4:
        _bss_200 = -1;
        return 0x14;
    case 5:
        _bss_200 = -1;
        return 0x13;
    case 8:
        return -0x29;
    case 18:
        return 0x29;
    case 9:
        return -0xB;
    case 10:
        return 0x12;
    case 14:
        if (spC4->unk8B6 >= 0xD) {
            return 0x28;
        }
        break;
    case 15:
        if (spC4->unk8B6 >= 0xD) {
            return 0xB;
        }
        break;
    case 13:
        return 0x23;
    case 7:
        _bss_200 = -1;
        if (dll_210_func_7BC4(arg0, spC4, sp90, &spC4->unk6B0) == 1) {
            return -0x2C;
        }
        break;
    }

    if (gDLL_1_UI->vtbl->func6() != 0) {
        if (gDLL_1_UI->vtbl->func7(0x1EE) != 0) {
            set_button_mask(0, 0x8000U);
            if ((get_gplay_bitstring(BIT_3DC) != 0) && (get_gplay_bitstring(BIT_Tricky_Dug_Up_Horn_of_Truth_Pad) != 0)) {
                set_gplay_bitstring(BIT_Play_Summoning_SnowHorn_with_Horn_of_Truth, 1);
                set_gplay_bitstring(BIT_3D8, 1);
            } else {
                temp_v0_5 = obj_get_nearest_type_to(0x3B, arg0, &sp80);
                if (temp_v0_5 != NULL) {
                    ((DLL_Unknown*)temp_v0_5->dll)->vtbl->func[7].withOneArg(temp_v0_5);
                }
                gDLL_3_Animation->vtbl->func17(7, arg0, -1);
            }
            return 0;
        }
        if (arg0->id != 0) {
            sp8E = 0;
            if (gDLL_1_UI->vtbl->func7(0x13D) != 0) {
                sp8E = 0xA;
            }
        } else {
            sp8E = 0;
            if (gDLL_1_UI->vtbl->func7(0x5D6) != 0) {
                if (get_gplay_bitstring(BIT_7E2) != 0) {
                    sp8E = 0xA;
                } else {
                    set_gplay_bitstring(BIT_7E2, 1U);
                    sp8E = 0xA;
                }
            }
        }
        if (sp8E != 0) {
            dll_210_func_1DAB0(arg0);
            gDLL_3_Animation->vtbl->func17((s32) sp8E, arg0, -1);
            gDLL_1_UI->vtbl->ui_func_1290.asVoid();
            return 0;
        }
    }
    if (gDLL_1_UI->vtbl->ui_func_f40.asVoidS32() == 0x387) {
        sp8C = gDLL_1_UI->vtbl->ui_func_e2c.withTwoArgsS32(&sp38, 0x10);
        if (sp8C != -1 && (arg0->unk0xc4 == NULL)) {
            set_button_mask(0, 0x8000U);
            arg0->unk_0xe0 = sp8C;
            sp8C = ((DLL_Unknown*)spC4->unk85C->dll)->vtbl->func[20].withOneArgS32(sp8C);
            sp88 = gDLL_2_Camera->vtbl->func15.asVoidS32();
            if ((sp88 != NULL) && ((sp88->def->unk40->unk10 & 0xF) == 3)) {
                sp88 = sp88;
                gDLL_3_Animation->vtbl->func30(sp8C, sp88, 1);
                gDLL_3_Animation->vtbl->func17(2, arg0, -1);
                arg0->unk0xc4 = sp88;
                *_data_18 = -1;
                *_data_14 = 0;
                arg0->unk0xdc = 3;
            }
        }
    }
    if (spC4->unk808 != 0.0f) {
        return 0x4A;
    }
    if (spC4->unk8A4 != 0) {
        return 0x4B;
    }
    if (spC4->unk8A8 != 0) {
        if (arg1->unk310 & 0x4000) {
            spC4->unk8A9 = 0;
        }
        temp_v0 = dll_210_func_18E80(arg0, arg1, arg2);
        if (temp_v0 != 0) {
            return temp_v0;
        }
    } else if ((arg1->unk310 & 0x8000) && (spC4->unk708 == NULL)) {
        spC4->unk8A9 = 4;
        return 0x3D;
    }
    return 0;
}
#endif

// offset: 0xC1F4 | func: 66
s32 dll_210_func_C1F4(Object* arg0, PlayerState* arg1, f32 arg2) {
    s8 var_v0;

    if (arg1->unk1AC > 20.0f) {
        var_v0 = _data_7BC + 1;
    } else {
        var_v0 = 0;
    }

    _data_7BC = var_v0;
    if (var_v0 >= 3) {
        return -0xD;
    }

    return 0;
}

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
s32 dll_210_func_142C4(Object* arg0, PlayerState* arg1, f32 arg2) {
    PlayerState* temp_s0;
    Object* sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;

    gDLL_2_Camera->vtbl->func24.withOneArg(2);
    temp_s0 = arg0->state;
    arg1->unk1BC[0x9F] = 0;
    arg1->unk2FC = dll_210_func_14B70;
    func_800267A4(arg0);
    sp48 = temp_s0->unk858;
    if (sp48 == NULL) {
        arg0->curModAnimIdLayered = -1;
        return 0;
    }
    if (arg1->unk272 != 0) {
        if (temp_s0->unk76C == NULL) {
            temp_s0->unk76C = (s16 *)_data_158;
        }
        if (temp_s0->unk770 & 2) {
            sp30 = 8;
            func_80024E50(arg0, *temp_s0->unk76C, 0.0f, 0);
            func_80025140(arg0, 0.0f, 0, 0); // arg2 should be 0.0f
            func_80024E50(arg0, temp_s0->unk76C[1], 0.0f, 0xA);
            func_80025140(arg0, 0.0f, 0, 0); // arg2 should be 0.0f
        }
        func_80023D30(arg0, *temp_s0->unk76C, 0.0f, (u8)sp30);
        func_80024108(arg0, 0.0f, 0.0f, 0);
    }
    if (temp_s0->unk770 & 4) {
        func_800240BC(arg0, sp48->animProgress);
        arg1->unk298 = NULL;
    } else {
        sp3C = ((DLL_Unknown*)sp48->dll)->vtbl->func[16].withTwoArgsF32(sp48, &sp44);
        sp3C = sp3C;
        if (sp44 <= 1.0f) {
            arg1->unk298 = sp44;
        } else {
            arg1->unk298 = ((sp3C * 0.05f) + 0.01f);
        }
    }
    if (temp_s0->unk770 & 1) {
        ((DLL_Unknown*)sp48->dll)->vtbl->func[15].withThreeArgs(sp48, &sp40, &sp34);
        sp38 = (s32) (sp40 * 1023.0f);
        if (sp38 < 0) {
            sp38 = -sp38;
        }
        if (sp34 != 0) {
            func_80025540(arg0, temp_s0->unk76C[3], sp38);
            func_8002559C(arg0, temp_s0->unk76C[5], sp38);
        } else {
            func_80025540(arg0, temp_s0->unk76C[2], sp38);
            func_8002559C(arg0, temp_s0->unk76C[4], sp38);
        }
    }
    if (temp_s0->unk770 & 1) {
        func_80024DD0(arg0, 0, 2, 0);
        func_80024DD0(arg0, 1, 2, 0);
    }
    if (temp_s0->unk770 & 2) {
        func_80024DD0(arg0, 1, 0, (s16) (s32) (sp3C * 1023.0f));
        func_80025140(arg0, arg1->unk298, arg2, 0);
    }
    if (((DLL_Unknown*)sp48->dll)->vtbl->func[10].withTwoArgsS32(sp48, arg0) != 0) {
        return 0x27;
    }
    return 0;
}

// offset: 0x146D8 | func: 98
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_146D8.s")

// offset: 0x14B70 | func: 99
static void dll_210_func_14B70(Object* arg0, u32 arg1) {
    s16* temp_v0_2;

    arg0->ptr0x64->flags &= ~0x1000;
    arg0->srt.flags &= 0xFFF7 ;
    arg0->curModAnimIdLayered = -1;
    temp_v0_2 = func_80034804(arg0, 9);
    if (temp_v0_2 != NULL) {
        temp_v0_2[0] = 0;
        temp_v0_2[1] = 0;
        temp_v0_2[2] = 0;
    }
}

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
s32 dll_210_func_18E80(Object* player, PlayerState* state, f32 arg2) {
    if (state->unk310 & 0x8000) {
        return 0x3D;
    }
    return 0;
}

// offset: 0x18EAC | func: 123
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_18EAC.s")
#else
// Requires these statics:
// dll_210_func_6DD8
// dll_210_func_1A9D4 (matched)
// dll_210_func_1DC48 (matched)
// dll_210_func_1DD94 (matched)
// dll_210_func_1AAD8 (matched)
// dll_210_func_1CEFC (matched)
// https://decomp.me/scratch/TtS1w
s32 dll_210_func_18EAC(Object* arg0, PlayerState* arg1, f32 arg2) {
    s32 sp9C;
    s32 sp98;
    Object* temp_a0;
    f32 temp_fa1;
    f32 sp8C;
    f32 var_fa0;
    f32 var_ft4;
    f32 sp80;
    SRT sp68;
    f32 var_fv0;
    f32 var_fv1;
    s32 temp_v0;
    s8 temp_v0_4;
    s16 temp_ft5;
    u32 temp_a0_4;
    s32* var_v0;
    s32 i;
    PlayerState* temp_s1;

    temp_s1 = arg0->state;
    if (arg1->unk272 != 0) {
        if (arg1->unk2C8 != NULL) {
            func_80023D30(arg0, 0x43E, 0.0f, 0);
            dll_210_func_6DD8(arg0, temp_s1, *_bss_220);
            arg1->unk298 = 0.015f;
        } else if (temp_s1->unk8A8 != 0) {
            func_80023D30(arg0, 0x43D, 0.0f, 0);
            arg1->unk298 = 0.04f;
        } else {
            func_80023D30(arg0, 0x448, 0.0f, 0);
            if (arg0->id == 0x1F) {
                arg1->unk298 = 0.035f;
            } else {
                arg1->unk298 = 0.024f;
            }
        }
        arg1->unk2FC = dll_210_func_1AAD8;
        dll_210_func_A024(arg0, arg1);
        temp_s1->unk830 = 0.0f;
        temp_s1->unk82C = 0.0f;
        temp_a0 = arg0->linkedObject;
        *_bss_34 = 0;
        _bss_30 = 0.0f;
        _bss_28 = 0.0f;
        _bss_20 = 0.0f;
        _bss_2C = 0.0f;
        ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs(temp_a0, 0);
    }
    switch (arg0->curModAnimId) {
    case 0x43D:
        if (arg1->unk32C[0xE] != 0) {
            func_80023D30(arg0, 0x43E, 0.0f, 0);
            dll_210_func_6DD8(arg0, (PlayerState* ) temp_s1, 0x2D);
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xBA4U, 0x7FU, &temp_s1->unk848, NULL, 0, NULL);
            arg1->unk298 = 0.015f;
        }
        break;
    case 0x449:
        if (arg1->unk32C[0xE] != 0) {
            if (arg1->unk2C8 != NULL) {
                return 0x36;
            }
            return -1;
        }
        break;
    case 0x448:
        if ((arg0->animProgress > 0.4f) && (temp_s1->unk8A8 == 0)) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, temp_s1->unk3B8[4], 0x7FU, NULL, NULL, 0, NULL);
            temp_s1->unk8A8 = 2U;
            temp_s1->unk8A9 = 2;
            temp_a0 = arg0->linkedObject;
            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[7].withOneS32OneF32(temp_a0, 0x3E19999A);
        }
        if (arg1->unk32C[0xE] != 0) {
            func_80023D30(arg0, 0x43E, 0.0f, 0);
            dll_210_func_6DD8(arg0, (PlayerState* ) temp_s1, 0x2D);
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xBA4U, 0x7FU, &temp_s1->unk848, NULL, 0, NULL);
            arg1->unk298 = 0.015f;
        }
        break;
    case 0x43E:
        if (arg1->unk2C8 != NULL) {
            if (arg1->unk2C8->unk0x74 != NULL) {
                var_fv0 = ((f32 *)arg1->unk2C8->unk0x74)[3] - arg0->srt.transl.x;
                sp8C = ((f32 *)arg1->unk2C8->unk0x74)[4] - arg0->srt.transl.y;
                var_fv1 = ((f32 *)arg1->unk2C8->unk0x74)[5] - arg0->srt.transl.z;
            } else {
                var_fv0 = arg1->unk2C8->srt.transl.x - arg0->srt.transl.x;
                sp8C = arg1->unk2C8->srt.transl.y - arg0->srt.transl.y;
                var_fv1 = arg1->unk2C8->srt.transl.z - arg0->srt.transl.z;
            }
            var_fv0 = (s16) (arctan2_f(sp8C, sqrtf(SQ(var_fv0) + SQ(var_fv1))) - 0x800) / 5461.0f;
            var_fv0 = var_fv0; // required to match?
            if (var_fv0 < -1.0f) {
                var_fv0 = -1.0f;
            } else if (var_fv0 > 1.0f) {
                var_fv0 = 1.0f;
            }
            var_fv1 = _bss_28;
            var_fv0 -= var_fv1;

            var_fv1 += (var_fv0) * 0.01f * arg2;
            _bss_28 = var_fv1;
        } else {
            if (*_bss_220 == 0x777) {
                var_fa0 = arg1->unk284 / 50.0f;
                if (var_fa0 < -1.45f) {
                    var_fa0 = -1.45f;
                } else if (var_fa0 > 1.45f) {
                    var_fa0 = 1.45f;
                }
            } else {
                var_fa0 = arg1->unk284 / 60.0f;
                if (var_fa0 < -1.0f) {
                    var_fa0 = -1.0f;
                } else if (var_fa0 > 1.0f) {
                    var_fa0 = 1.0f;
                }
            }
            var_fa0 -= temp_s1->unk830;
            temp_s1->unk830 += var_fa0 * 0.1f * arg2;
            var_fv1 = arg1->unk288 / 60.0f;
            if (var_fv1 < -1.0f) {
                var_fv1 = -1.0f;
            } else if (var_fv1 > 1.0f) {
                var_fv1 = 1.0f;
            }
            var_fv1 -= temp_s1->unk82C;
            temp_s1->unk82C += var_fv1 * 0.1f * arg2;
            if (temp_s1->unk82C > 0.0f) {
                var_fv1 = temp_s1->unk82C - 0.75f;
                if (var_fv1 < 0.0f) {
                    var_fv1 = 0.0f;
                }
            } else {
                var_fv1 = temp_s1->unk82C + 0.75f;
                if (var_fv1 > 0.0f) {
                    var_fv1 = 0.0f;
                }
            }
            arg0->srt.yaw = (arg0->srt.yaw + (var_fv1 * -1000.0f));
        }
        sp98 = (s32) temp_s1->stats->magic;
        temp_a0 = arg0->linkedObject;
        sp68.scale = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[16].withOneArgS32(temp_a0);
        if ((temp_s1->unk766 & 0x8000) && (sp98 == 0)) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x912U, 0x7FU, NULL, NULL, 0, NULL);
        }
        if ((temp_s1->unk764 & 0x8000) && (sp98 != 0)) {
            temp_v0_4 = *_bss_34;
            if (temp_v0_4 != 0) {
                var_ft4 = _bss_2C;
                var_ft4 -= arg2;
                *_bss_34 = temp_v0_4;
                if (var_ft4 < 400.0f) {
                    var_ft4 = 400.0f;
                    *_bss_34 = (temp_v0_4 & 0xFF & 0xFF) ^ 1;
                }
            } else {
                var_ft4 = _bss_2C;
                var_ft4 += arg2;
                *_bss_34 = temp_v0_4;
                if (var_ft4 > 420.0f) {
                    var_ft4 = 420.0f;
                    *_bss_34 = (temp_v0_4 & 0xFF & 0xFF) ^ 1;
                }
            }
            if (*_bss_220 != 0x5CE) {
                if ((var_ft4 >= 10.0f) && (temp_s1->unk848 == 0)) {
                    _bss_2C = var_ft4;
                    gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6ABU, 1U, &temp_s1->unk848, NULL, 0, NULL);
                    gDLL_6_AMSFX->vtbl->func_954(temp_s1->unk848, 0.5f);
                    var_ft4 = _bss_2C;
                } else if (var_ft4 < 10.0f) {
                    temp_a0_4 = temp_s1->unk848;
                    if (temp_a0_4 != 0) {
                        _bss_2C = var_ft4;
                        gDLL_6_AMSFX->vtbl->func_A1C(temp_a0_4);
                        temp_s1->unk848 = 0U;
                        var_ft4 = _bss_2C;
                    }
                }
                if (var_ft4 >= 420.0f) {
                    var_fv0 = _bss_30;
                    var_fv0 -= arg2;
                    _bss_30 = var_fv0;
                    if (var_fv0 <= 0.0f) {
                        _bss_2C = var_ft4;
                        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6ADU, rand_next(0x20, 0x60), NULL, NULL, 0, NULL);
                        _bss_30 = rand_next(0x4B0, 0x708);
                        var_ft4 = _bss_2C;
                    }
                } else if (var_ft4 < 0.0f) {
                    var_ft4 = 0.0f;
                }
                if (sp68.scale == 0.0f) {
                    if (var_ft4 > 120.0f) {
                        _bss_2C = var_ft4;
                        if (sp98 >= 3) {
                            temp_a0 = arg0->linkedObject;
                            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs(temp_a0, 1);
                            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6ACU, 0x60U, NULL, NULL, 0, NULL);
                            var_ft4 = _bss_2C;
                        } else {
                            var_ft4 = 120.0f;
                        }
                    }
                } else if (sp68.scale == 1.0f) {
                    arg1->unk298 = 0.02f;
                    if (var_ft4 > 300.0f) {
                        _bss_2C = var_ft4;
                        if (sp98 >= 9) {
                            temp_a0 = arg0->linkedObject;
                            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs(temp_a0, 2);
                            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6ACU, 0x7FU, NULL, NULL, 0, NULL);
                            var_ft4 = _bss_2C;
                        } else {
                            var_ft4 = 300.0f;
                        }
                    }
                } else {
                    arg1->unk298 = 0.027f;
                }
                // missing a _bss_2C = var_ft4; here
                if (temp_s1->unk848 != 0) {
                    sp80 = ((var_ft4 / 420.0f) * 0.5f) + 0.5f;
                    _bss_2C = var_ft4;
                    if (sp80 > 1.0f) {
                        sp80 = 1.0f;
                    }
                    if (*_bss_220 == 0x777) {
                        temp_ft5 = ((sp80 - 0.5f) * 127.0f);
                        diPrintf("throwdist %d\n\0error\n\0 Light Created \0 WARNING: Screen Overlay already used \n\0 WARNING: Screen Overlay already Killed \n", temp_ft5);
                        temp_a0 = arg0->linkedObject;
                        ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs(temp_a0, temp_ft5);
                    }
                    gDLL_6_AMSFX->vtbl->func_954(temp_s1->unk848, sp80);
                    gDLL_6_AMSFX->vtbl->func_860(temp_s1->unk848, 127.0f * sp80);
                }
                temp_fa1 = _bss_20;
                temp_fa1 -= arg2;
                _bss_20 = temp_fa1;
                if (temp_fa1 <= 0.0f) {
                    gDLL_17->vtbl->func1(arg0->linkedObject, 0x3EC, &sp68, 2, -1, NULL);
                    if (sp68.scale == 0.0f) {
                        _bss_20 = 8.0f;
                    } else if (sp68.scale == 1.0f) {
                        _bss_20 = 5.0f;
                    } else {
                        _bss_20 = 3.0f;
                    }
                }
            } else {
                _bss_2C = var_ft4;
                if (*_bss_220 == 0x5CE) {
                    if (temp_s1->unk848 == 0) {
                        gDLL_6_AMSFX->vtbl->play_sound(arg0->linkedObject, 0x95AU, 1U, &temp_s1->unk848, NULL, 0, NULL);
                    }
                    if (*_bss_210 == 0) {
                        dll_210_func_1DC48(arg0);
                    }
                    if (_bss_210[3] == 0) {
                        _bss_210[3] = dll_210_func_1DD94(arg0, 0x249);
                    }
                    var_fv0 = _bss_24;
                    var_fv0 -= arg2;
                    _bss_24 = var_fv0;
                    if (var_fv0 <= 0.0f) {
                        if (_data_7C0 == 0) {
                            _data_7C0 = dll_load_deferred(0x1048U, 1U);
                        }
                        if (_data_7C0 != 0) {
                            _data_7C0->vtbl->func[0].withSixArgs(arg0->linkedObject, arg0->id == 0, 0, 0x10404, -1, 0);
                        }
                        _bss_24 = 35.0f;
                    }
                    if (*_bss_1AA == 0) {
                        var_fv0 = (*_data_7C4 - arg0->linkedObject->srt.pitch);
                        var_fv0 /= 3000.0f;
                        sp80 = 0.7f + var_fv0;
                        gDLL_6_AMSFX->vtbl->func_860(temp_s1->unk848, (127.0f * sp80));
                        if (sp80 > 0.775f) {
                            sp80 = 0.775f;
                        } else if (sp80 < 0.625f) {
                            sp80 = 0.625f;
                        }
                        gDLL_6_AMSFX->vtbl->func_954(temp_s1->unk848, sp80);
                        *_data_7C4 = arg0->linkedObject->srt.pitch;
                    }
                    if (arg0->id == 0) {
                        sp68.pitch = -0x8000;
                    }
                    if (rand_next(0, 2) == 0) {
                        gDLL_17->vtbl->func1(arg0->linkedObject, 0x525, &sp68, 1, -1, NULL);
                    }
                    if (rand_next(0, 2) == 0) {
                        gDLL_17->vtbl->func1(arg0->linkedObject, 0x526, &sp68, 1, -1, NULL);
                    }
                    if (rand_next(0, 2) == 0) {
                        gDLL_17->vtbl->func1(arg0->linkedObject, 0x529, &sp68, 1, -1, NULL);
                    }
                }
            }
        }
        if (temp_s1->unk830 > 0.0f) {
            func_80025540(arg0, 0x441, temp_s1->unk830 * 1023.0f);
        } else {
            func_80025540(arg0, 0x440, -temp_s1->unk830 * 1023.0f);
        }
        func_80034804(arg0, 9)[1] = temp_s1->unk82C * -10240.0f;
        temp_s1->flags = (s32) (temp_s1->flags & ~0x400);
        if ((arg1->unk2C8 == NULL) && (dll_210_func_1A9D4(arg0, &temp_s1->aimX, &temp_s1->aimY, &temp_s1->aimZ, temp_s1->unk82C, temp_s1->unk830) != 0)) {
            temp_s1->flags = (s32) (temp_s1->flags | 0x400);
        }
        if ((((arg1->unk2C8 != NULL) && !(temp_s1->unk764 & 0x8000)) || ((arg1->unk2C8 == NULL) && (temp_s1->unk768 & 0x8000))) && (sp98 != 0) && (*_bss_220 != 0x5CE)) {
            gDLL_13_Expgfx->vtbl->func4.withOneArg(arg0->linkedObject);
            sp68.transl.x = arg0->linkedObject->srt.transl.x;
            sp68.transl.y = arg0->linkedObject->srt.transl.y;
            sp68.transl.z = arg0->linkedObject->srt.transl.z;
            for (sp9C = 0; sp9C < 0x14; sp9C++) {
                gDLL_17->vtbl->func1(arg0->linkedObject, 0x3ED, &sp68, 0x200001, -1, NULL);
            }
            if (sp68.scale == 0.0f) {
                sp98 = 1;
            } else if (sp68.scale == 1.0f) {
                sp98 = 3;
            } else {
                sp98 = 9;
            }
            dll_210_func_1CEFC(arg0, -sp98);
            ((void (*)(Object*, PlayerState*, f32)) *_bss_224)(arg0, arg1, temp_s1->unk830);
            arg1->unk298 = 0.02f;
            func_80023D30(arg0, 0x43F, 0.0f, 0);
            if (temp_s1->unk830 > 0.0f) {
                func_80025540(arg0, 0x44B, temp_s1->unk830 * 1023.0f);
            } else {
                func_80025540(arg0, 0x44A, -temp_s1->unk830 * 1023.0f);
            }
            if (sp68.scale < 2.0f) {
                *_bss_34 = 0;
            } else {
                *_bss_34 = 2;
            }
            if (temp_s1->unk848 != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(temp_s1->unk848);
                temp_s1->unk848 = 0U;
            }
            temp_a0 = arg0->linkedObject;
            _bss_30 = 0.0f;
            _bss_20 = 0.0f;
            _bss_2C = 0.0f;
            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs(temp_a0, 0);
            temp_s1->flags = (s32) (temp_s1->flags & ~0x400);
        } else if ((temp_s1->unk768 & 0x8000) && (sp98 != 0) && (*_bss_220 == 0x5CE)) {
            if (_data_7C0 != 0) {
                dll_unload(_data_7C0);
            }
            _data_7C0 = 0;
            if (temp_s1->unk848 != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(temp_s1->unk848);
                temp_s1->unk848 = 0U;
            }
            for (i = 0; i < 4; i++) {
                temp_a0 = _bss_210[i];
                if (temp_a0 != NULL) {
                    obj_destroy_object(temp_a0);
                    _bss_210[i] = 0;
                }
            }
        }
        break;
    case 0x43F:
        if (temp_s1->unk830 > 0.0f) {
            func_80025540(arg0, 0x44B, temp_s1->unk830 * 1023.0f);
        } else {
            func_80025540(arg0, 0x44A, -temp_s1->unk830 * 1023.0f);
        }
        temp_s1->flags &= ~0x400;
        if ((arg1->unk2C8 == NULL) && (dll_210_func_1A9D4(arg0, &temp_s1->aimX, &temp_s1->aimY, &temp_s1->aimZ, temp_s1->unk82C, temp_s1->unk830) != 0)) {
            temp_s1->flags |= 0x400;
        }
        if (arg1->unk32C[0xE] != 0) {
            temp_v0_4 = *_bss_34;
            temp_v0_4--;
            if ((temp_v0_4 < 0) || (*_bss_34 = temp_v0_4, (*_bss_220 == 0x777))) {
                *_bss_34 = 0;
                if (arg1->unk2C8 != NULL) {
                    return 0x36;
                }
                func_80023D30(arg0, 0x43E, 0.0f, 0);
                if (temp_s1->unk830 > 0.0f) {
                    func_80025540(arg0, 0x441, temp_s1->unk830 * 1023.0f);
                } else {
                    func_80025540(arg0, 0x440, -temp_s1->unk830 * 1023.0f);
                }
                arg1->unk298 = 0.015f;
                gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xBA4U, 0x7FU, &temp_s1->unk848, NULL, 0, NULL);
            } else {
                sp68.transl.x = arg0->linkedObject->srt.transl.x;
                sp68.transl.y = arg0->linkedObject->srt.transl.y;
                sp68.transl.z = arg0->linkedObject->srt.transl.z;
                for (sp9C = 0; sp9C < 0x14; sp9C++) {
                    gDLL_17->vtbl->func1(arg0->linkedObject, 0x3ED, &sp68, 0x200001, -1, NULL);
                }
                ((void (*)(Object*, PlayerState*, f32)) *_bss_224)(arg0, arg1, temp_s1->unk830);
                arg1->unk298 = 0.02f;
                func_80023D30(arg0, 0x43F, 0.0f, 0);
            }
        }
        break;
    default:
        break;
    }

    if (arg1->unk2C8 == NULL && ((temp_s1->flags & 0x400000 && temp_s1->unk766 & 0x4000) || !(temp_s1->flags & 0x401000)) && arg0->curModAnimId != 0x449) {
        func_80023D30(arg0, 0x449, 0.0f, 0);
        arg1->unk298 = 0.04f;
        temp_v0 = gDLL_2_Camera->vtbl->func3();
        if ((temp_v0 != 0x54) && (temp_v0 != 0x5E)) {
            gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0x3C, 0xFE);
        }
    }
    if (arg1->unk2C8 != NULL) {
        gDLL_18->vtbl->func11.withFourArgsCustom(arg0, arg1, arg2, 4);
    }
    return 0;
}
#endif

// offset: 0x1A9D4 | func: 124
s32 dll_210_func_1A9D4(Object* arg0, s32* arg1, s32* arg2, s32* arg3, f32 arg4, f32 arg5) {
    s32 temp_t6;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_v0 = get_some_resolution_encoded();
    temp_t6 = (temp_v0 & 0xFFFF) >> 1;
    *arg1 = temp_t6 + (arg4 * ((temp_t6 >> 1) + 0xA));
    if (arg5 < 0.0f) {
        temp_v1 = temp_v0 >> 0x11;
        *arg2 = ((temp_v1 + 0xC) + (arg5 * (temp_v1 >> 1)));
    } else {
        temp_v1 = temp_v0 >> 0x11;
        *arg2 = ((temp_v1 + 0xC) + (arg5 * (temp_v1 >> 2)));
    }
    return 1;
}

// offset: 0x1AAD8 | func: 125
void dll_210_func_1AAD8(Object* arg0, UNK_TYPE_32 arg1) {
    Object* temp_a0;
    s16* temp_v0;
    s32 i;
    PlayerState* temp_s0;

    temp_s0 = arg0->state;
    temp_s0->unk87C = -1;
    temp_s0->flags &= ~0x400;
    temp_a0 = arg0->linkedObject;
    ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withThreeArgs(temp_a0, 0, arg0);
    if (temp_s0->unk848 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(temp_s0->unk848);
        temp_s0->unk848 = 0;
    }
    if (gDLL_2_Camera->vtbl->func3() != 0x54) {
        gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0x78, 0xFF);
    }
    for (i = 0; i < 4; i++) {
        temp_a0 = _bss_210[i];
        if (temp_a0 != 0) {
            obj_destroy_object(temp_a0);
            _bss_210[i] = 0;
        }
    }
    temp_v0 = func_80034804(arg0, 9);
    temp_v0[1] = 0;
    temp_v0[0] = 0;
}

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
        } else if (arg1 > 0x64) {
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
s32 dll_210_func_1D2A8(Object* arg0, Object* arg1) {
    PlayerState* sp24;
    Object* var_v1;

    sp24 = arg0->state;
    if (arg1 != NULL) {
        sp24->unk868 = arg1;
        gDLL_18->vtbl->func4(arg0, sp24, 5);
    } else {
        var_v1 = sp24->unk868;
        if (var_v1 != NULL) {
            var_v1->unk_0xe0 = 0;
            sp24->unk868 = NULL;
            sp24->unk870 = 0;
            gDLL_18->vtbl->func4(arg0, sp24, 1);
        }
    }

    var_v1 = sp24->unk868;
    return var_v1 != NULL;
}

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
    return state->unk354.unk1E == 1;
}

// offset: 0x1D4E0 | func: 192 | export: 33
void dll_210_func_1D4E0(Object* arg0, s32 arg1) {
    PlayerState* sp24;

    sp24 = arg0->state;
    if (arg1 != 0) {
        gDLL_18->vtbl->func4(arg0, sp24, 0x44);
        sp24->flags |= 1;
    } else {
        gDLL_18->vtbl->func4(arg0, sp24, 1);
        sp24->flags &= ~1;
    }
}

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
u8 dll_210_func_1D5E8(Object* player, s32 arg1, s8* arg2) {
    PlayerState* state = player->state;

    *arg2 = state->unk680.unk2E;
    return (state->unk26C == 0x28 && state->unk680.unk28 == arg1);
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
Unk80032CF8Copy* dll_210_func_1D778(Object* player) {
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
void dll_210_func_1D8EC(Object* arg0, PlayerState* arg1, s32 arg2) {
    s32 i;
    s32 counter;
    s32 var_v1;
    s32* result;
    s32* var_s1;

    counter = 0;
    var_s1 = NULL;
    if (arg1->unk8BF == 1) {
        for (i = 6; i < 0xF; i++) {
            result = func_800348A0(arg0, i, 0);
            if (var_s1 == NULL || *var_s1 >= _data_7C8[i - 6]) {
                var_v1 = *result + (_data_7D4[i - 6] * arg2);
                if (var_v1 > 0xFF) {
                    var_v1 = 0xFF;
                }
                *result = var_v1;
            }
            var_s1 = result;
        }
        return;
    }

    if (arg1->unk8BF == -1) {
        for (i = 6; i < 0xF; i++) {
            result = func_800348A0(arg0, i, 0);
            if (var_s1 == NULL || _data_7C8[i - 6] >= *var_s1) {
                var_v1 = *result - (_data_7D4[i - 6] * arg2);
                if (var_v1 <= 0) {
                    var_v1 = 0;
                }
                *result = var_v1;
                if (var_v1 != 0) {
                    counter++;
                }
            }
            var_s1 = result;
        }
        if (counter == 0) {
            arg1->unk8BF = 0;
            func_80023A18(arg0, 0);
        }
    }
}

// offset: 0x1DAB0 | func: 216
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1DAB0.s")

// offset: 0x1DB6C | func: 217
void dll_210_func_1DB6C(Object* arg0, f32 arg1) {
    f32 sp2C;
    f32 sp28;
    ObjectHitInfo* temp_v0;
    f32 sp20;

    temp_v0 = arg0->objhitInfo;
    if (temp_v0 != NULL) {
        sp28 = (f32) temp_v0->unk_0x52;
    } else {
        sp28 = arg0->unk_0xa8;
    }
    sp2C = (arg0->unk0x74->y - arg0->srt.transl.y) - arg1;
    sp28 = sp28;
    sp20 = fsin16_precise(arg0->srt.yaw);
    gDLL_3_Animation->vtbl->func31(-sp20 * sp28, sp2C, -fcos16_precise(arg0->srt.yaw) * sp28);
}

// offset: 0x1DC48 | func: 218
void dll_210_func_1DC48(Object* obj) {
    s8 i;
    ObjCreateInfo* createInfo;

    for (i = 0; i < 3; i++) {
        if (_bss_210[i] != 0) {
            continue;
        }

        createInfo = obj_alloc_create_info(0x24, OBJ_iceblast);
        if (createInfo == NULL) {
            break;
        }

        createInfo->x = obj->positionMirror.x;
        createInfo->y = obj->positionMirror.y;
        createInfo->z = obj->positionMirror.z;
        createInfo->loadParamA = 2;
        createInfo->loadParamB = 1;
        createInfo->loadDistance = 0xFF;
        createInfo->fadeDistance = 0xFF;
        ((s16* )createInfo)[13] = i * 0xA;
        if (obj->id != 0) {
            ((u16*)createInfo)[14] = 0;
        } else {
            ((u16*)createInfo)[14] = 0x8000;
        }
        _bss_210[i] = obj_create(createInfo, 5, obj->mapID, -1, obj->parent);
    }
}

// offset: 0x1DD94 | func: 219
Object *dll_210_func_1DD94(Object* obj, s32 arg1) {
    ObjCreateInfo* createInfo;

    createInfo = obj_alloc_create_info(0x28, OBJ_LFXEmitter);
    createInfo->loadParamA = 2;
    createInfo->loadParamB = 1;
    createInfo->loadDistance = 0xFF;
    createInfo->fadeDistance = 0xFF;
    createInfo->x = obj->srt.transl.x;
    createInfo->y = obj->srt.transl.y;
    createInfo->z = obj->srt.transl.z;
    ((s16*)createInfo)[16] = 0;
    ((s16*)createInfo)[15] = arg1;
    ((s16*)createInfo)[17] = -1;
    return obj_create(createInfo, 5, obj->mapID, -1, obj->parent);
}

// offset: 0x1DE50 | func: 220
void dll_210_func_1DE50(s32 arg0, s32 arg1, s32 arg2) {
}

// offset: 0x1DE64 | func: 221
void dll_210_func_1DE64(s32 arg0) {
}
