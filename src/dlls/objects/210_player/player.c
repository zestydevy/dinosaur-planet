#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "PR/os.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "game/objects/unknown_setups.h"
#include "game/gamebits.h"
#include "sys/gfx/model.h"
#include "sys/gfx/animation.h"
#include "sys/joypad.h"
#include "sys/objects.h"
#include "sys/print.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/gfx/gx.h"
#include "sys/rand.h"
#include "sys/camera.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/math.h"
#include "sys/map.h"
#include "sys/gfx/map.h"
#include "sys/dll.h"
#include "sys/memory.h"
#include "sys/joypad.h"
#include "sys/newshadows.h"
#include "functions.h"
#include "dll.h"
#include "types.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/277_iceblast.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "dlls/objects/793_BWLog.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/27.h"
#include "dlls/modgfx/106.h"
#include "unktypes.h"
#include "segment_334F0.h"

static void dll_210_func_1BC0(Object* arg0, Player_Data* arg1);
static void dll_210_func_618C(Object* arg0, Player_Data* arg1, s32 arg2, f32 arg3);
static void dll_210_func_7260(Object* arg0, Player_Data* arg1);
static s32 dll_210_func_7300(Object* arg0, Player_Data* arg1, Func_80059C40_Struct* arg2, Player_Data490* arg3, Vec3f* arg4, f32 arg5);
static s32 dll_210_func_7AAC(Object* arg0, Player_Data* arg1, Func_80059C40_Struct* arg2, Vec3f* arg3, Player_Data430* arg4, s32 arg5);
static void dll_210_func_7B98(Object* arg0, Func_80059C40_Struct* arg1, UnkArg2* arg2);
static void dll_210_func_7CF8(Player_Data* arg0, Vec3f* arg1);
static void dll_210_func_7DA0(Object* arg0, Player_Data* arg1, Vec3f* arg2);
static s32 dll_210_func_7E6C(Object* arg0, Player_Data* arg1, Player_Data* arg2, Player_Data3B4* arg3, f32 arg4, s32 arg5);
static void dll_210_func_8EA4(Object* arg0, Player_Data* arg1, Object* arg2, Gfx** arg3, Mtx** arg4, Vertex** arg5, Triangle** arg6, s32 arg7);
static void dll_210_func_A024(Object* player, Player_Data* objdata);
static s32 dll_210_func_C1F4(Object* arg0, Player_Data* arg1, f32 arg2);
static void dll_210_func_D510(Player_Data* arg0, f32 arg1);
static void dll_210_func_EF8C(Object *arg0, ObjFSA_Data *arg1);
static s32 dll_210_func_EFB4(Object* arg0, Player_Data* arg1, f32 arg2);
static void dll_210_func_12514(Object* arg0, ObjFSA_Data *arg1);
static void dll_210_func_14B70(Object* arg0, ObjFSA_Data *arg1);
static void dll_210_func_16204(Object *obj, ObjFSA_Data *fsa);
static void dll_210_func_1660C(Object* obj, ObjFSA_Data* fsa);

// These funcs are already matched but other funcs requires these are static
/* static */ void dll_210_func_1D8EC(Object* arg0, Player_Data* arg1, s32 arg2);
/* static */ void dll_210_func_41F4(Object* arg0, Player_Data* arg1);
/* static */ void dll_210_func_9E00(Object* arg0);
/* static */ void dll_210_func_363C(Object* arg0, Player_Data* arg1, Gfx** arg2, Mtx** arg3, Vertex** arg4);
/* static */ void dll_210_func_3B40(Object* arg0, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4);
/* static */ void dll_210_func_1DB6C(Object* arg0, f32 arg1);
/* static */ f32 dll_210_func_63F0(Player_Data* arg0, f32 updateRate);
/* static */ void dll_210_func_9F1C(Object* arg0, s32 arg1);
/* static */ void dll_210_func_60A8(Object* arg0, s32 arg1, s32 arg2);
/* static */ void dll_210_func_1DE64(UNK_TYPE_32 *arg0);
/* static */ void dll_210_func_1DE50(s32 arg0, s32 arg1, s32 arg2);
/* static */ void dll_210_func_90A0(Object* arg0, Player_Data* arg1, f32 arg2);
/* static */ void dll_210_func_955C(Object* arg0, Player_Data* arg1, f32 arg2);
/* static */ void dll_210_func_98CC(Object* arg0, Player_Data* arg1, f32 arg2);
/* static */ void dll_210_func_6DD8(Object* obj, Player_Data* data, s32 arg2);
/* static */ void dll_210_func_B4C8(Object* player, ObjFSA_Data *arg1);
/* static */ void dll_210_func_1AAD8(Object* arg0, ObjFSA_Data *arg1);
/* static */ s32 dll_210_func_18E80(Object* player, Player_Data* objdata, f32 arg2);
/* static */ s32 dll_210_func_1A9D4(Object* arg0, s32* arg1, s32* arg2, s32* arg3, f32 arg4, f32 arg5);
/* static */ Object *dll_210_func_1DD94(Object* obj, s32 arg1);
/* static */ void dll_210_func_1DC48(Object* obj);
/* static */ void dll_210_func_11A0(Object* arg0, Player_Data* arg1, f32 arg2);
/* static */ s32 dll_210_func_BA38(Object* arg0, Player_Data* arg1, f32 arg2);
/* static */ void dll_210_func_64B4(Object* arg0, Player_Data* arg1, f32 arg2);
/* static */ void dll_210_func_692C(Object* arg0, Player_Data* arg1, f32 arg2);
/* static */ int dll_210_func_4910(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3);
/* static */ void dll_210_func_47B8(Object* arg0, Player_Data* arg1);
/* static */ void dll_210_func_2534(Object* arg0, Player_Data* arg1, Player_Data* arg2);
/* static */ void dll_210_func_1CA8(Object* arg0, Player_Data* arg1, Player_Data* arg2);
/* static */ void dll_210_func_1DDC(Object* arg0, Player_Data* arg1, Player_Data* arg2);
/* static */ void dll_210_func_7180(Object* arg0, Player_Data* arg1, f32 updateRate);
/* static */ void dll_210_func_A058(Object* arg0);
/* static */ int dll_210_func_24FC(Object *player, Player_Data *data);
/* static */ void dll_210_func_1DAB0(Object *player);

// Used before declared / implemented
void dll_210_func_1D8B8(Object* player);
void dll_210_func_1D4E0(Object* arg0, s32 arg1);
s32 dll_210_func_1D2A8(Object* arg0, Object* arg1);
void dll_210_add_health(Object* player, s32 amount);
void dll_210_add_scarab(Object *player, s32 amount);
void dll_210_add_magic(Object* player, s32 amount);

void func_8004D880(Object *arg0);
void *func_8005D3A4(int param);
s32 func_80025140(Object*, f32, f32, s32);
MtxF* func_80032170(Object*, s32);
s32 func_80031F6C(Object*, s32, f32*, f32*, f32*, s32);
void func_8004E64C(Object* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80032238(Object* obj, s32 arg1, s32 arg2, Vec3f* arg3);
void func_80034FF0(s32);
void func_80035AF4(Gfx**, Mtx**, Vertex**, Triangle**, Object*, void*, s32, s32, void*, s32, s32);
void func_8001A3FC(ModelInstance *modelInst, u32 selector, s32 idx, f32 param_4, f32 scale, Vec3f *param_6, s16 *param_7);
void func_8005B5B8(Object*, Object*, s32);
void func_80023894(Object* object, s32 objectId);
s32 func_80031DD8(Object*, Object*, s32);

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
/*0x4*/ static f32 _data_4[] = { 0.4f };
/*0x8*/ static f32 _data_8[] = { 0.6f };
/*0xC*/ static f32 _data_C[] = { 0.0f, 0.0f };
/*0x14*/ static u8 _data_14[4] = { 0, 0, 0, 0 };
/*0x18*/ static s16 _data_18[2] = { -1, 0 };
/*0x1C*/ static f32 _data_1C = 0.0f;
/*0x20*/ static u8 _data_20[] = {
    0x01, 0x02, 0x00, 0x00
};
/*0x24*/ static s16 _data_24[] = { 0x0069, 0x0023 };
/*0x28*/ static u8 _data_28[] = { 0xc8, 0xaa, 0xa0, 0x78, 0x64, NULL, NULL, NULL };
/*0x30*/ static u8 _data_30[] = { 0x0a, NULL, NULL, NULL,  NULL, NULL, NULL, NULL };
/*0x38*/ static UNK_TYPE_32 _data_38[] = { 0 };
/*0x3C*/ static Vec3f _data_3C[] = {
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 17.0f, 0.0f },
    { 0.0f, 0.0f, -10.0f },
    { 0.0f, 0.0f, 0.0f }
};
/*0x6C*/ static f32 _data_6C[] = {
    0.0f
};
/*0x70*/ static f32 _data_70[] = { 8.5f, 0.0f, 0.0f };
/*0x7C*/ static Vec3f _data_7C[] = {
    {0.0f, 10.0f, 0.0f}
};
/*0x88*/ static f32 _data_88[] = {
    8.5f, 15.5f
};
/*0x90*/ static s16 _data_90[] = { 0x004c, 0x0000 };
/*0x94*/ static f32 _data_94[] = { 0.0065f };
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
/*0x190*/ static Player_Data3B4 _data_190[] = {
    { 0x99, 0x99, 0xb, 0x01ff, NULL, 0.021f, -1.0f, -1.0f, -1.0f, 0.33f, 0.44f, 0.3f, 0.35f, 0xe6, TRUE, NULL, NULL, NULL },
    { 0x9a, 0x9a, 0xb, 0x0102, NULL, 0.023f, 0.1f, 0.5f, 0.5f, 0.22f, 0.4f, 0.35f, 0.45f, 0xe3, TRUE, NULL, NULL, NULL },
    { 0x9b, 0x9b, 0xa, 0x01ff, NULL, 0.02f, -1.0f, -1.0f, -1.0f, 0.18f, 0.4f, 0.26f, 0.36f, 0xe6, TRUE, NULL, NULL, NULL },
    { 0x9b, 0x9b, 0xa, 0x0104, NULL, 0.02f, 0.1f, 0.5f, 0.5f, 0.18f, 0.4f, 0.26f, 0.36f, 0xe6, TRUE, NULL, NULL, NULL },
    { 0x9a, 0x9a, 0xb, 0x01ff, NULL, 0.023f, -1.0f, -1.0f, -1.0f, 0.22f, 0.4f, 0.35f, 0.45f, 0xe3, TRUE, NULL, NULL, NULL },
    { 0x96, 0x96, 0xa, 0x01ff, NULL, 0.017f, -1.0f, -1.0f, -1.0f, 0.2f, 0.7f, 0.35f, 0.46f, 0xe5, TRUE, NULL, NULL, NULL }
};
/*0x2F8*/ static Player_Data3B4 _data_2F8[] = {
    { 0x99, 0x99, 0xb, 0x01ff, NULL, 0.025f, -1.0f, -1.0f, -1.0f, 0.16f, 0.4f, 0.25f, 0.35f, 0x0661, FALSE, NULL, NULL, NULL },
    { 0x9a, 0x9a, 0xc, 0x0102, NULL, 0.025f, 0.1f, 0.5f, 0.5f, 0.2f, 0.45f, 0.25f, 0.3f, 0x0662, FALSE, NULL, NULL, NULL },
    { 0x9b, 0x9b, 0xa, 0x01ff, NULL, 0.026f, -1.0f, -1.0f, -1.0f, 0.15f, 0.36f, 0.26f, 0.36f, 0x0663, FALSE, NULL, NULL, NULL },
    { 0x9b, 0x9b, 0xa, 0x0104, NULL, 0.026f, 0.1f, 0.5f, 0.5f, 0.15f, 0.36f, 0.26f, 0.36f, 0x0663, FALSE, NULL, NULL, NULL },
    { 0x9a, 0x9a, 0xc, 0x01ff, NULL, 0.025f, -1.0f, -1.0f, -1.0f, 0.2f, 0.45f, 0.25f, 0.3f, 0x0662, FALSE, NULL, NULL, NULL },
    { 0x96, 0x96, 0xa, 0x02ff, NULL, 0.016f, -1.0f, -1.0f, -1.0f, 0.4f, 0.6f, 0.35f, 0.46f, 0x0661, FALSE, NULL, NULL, NULL }
};
/*0x460*/ static s16 _data_460[] = {
    0x00b6, 0x00b7, 0x00b8, 0x0675, 0x0676, 0x0370, 0x00df, 0x00e0,
    0x0b10, 0x0b0f, 0x0a79, 0x0a7a, 0x0a7b, 0x0a7c, 0x0a7d, 0x07aa,
    0x07ab, 0x07ac, 0x07ad, 0x00d9, 0x00da, 0x00db, 0x0b12, 0x0b14
};
/*0x490*/ static s16 _data_490[] = {
    0x067a, 0x067b, 0x067c, 0x0675, 0x0676, 0x0664, 0x0665, 0x0666,
    0x0667, 0x07a9, 0x06a6, 0x06a7, 0x06a8, 0x06a9, 0x06aa, 0x07aa,
    0x07ab, 0x07ac, 0x07ad, 0x066b, 0x066c, 0x066d, 0x0b11, 0x0b13
};
/*0x4C0*/ static s16 _data_4C0[] = { 0x00e9, 0x00ea, 0x00ec, 0x00ed, 0x00eb, 0x00ee };
/*0x4CC*/ static s16 _data_4CC[] = { 0x0020, 0x0021, 0x0023, 0x0024, 0x0022, 0x0025 };
/*0x4D8*/ static u16 _data_4D8[] = { 0x0b22, 0x0b1e, 0x0b22, 0x0000 };
/*0x4E0*/ static s16 _data_4E0[] = { BIT_Spell_Projectile, BIT_Spell_Illusion, BIT_Spell_Forcefield, BIT_Spell_Portal, BIT_Spell_Ice_Blast, BIT_Spell_Mind_Read, BIT_Spell_Grenade, BIT_0 };
/*0x4F0*/ static s32 _data_4F0[] = { 0, 1, 2, 3, 4, 5 };
/*0x508*/ static u32 _data_508[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff
};
/*0x518*/ static s16 _data_518[] = {
    OBJ_Tricky, OBJ_Kyte, OBJ_396, OBJ_Tricky, OBJ_Tricky, OBJ_Tricky, OBJ_Tricky, OBJ_Tricky
};
/*0x528*/ static f32 _data_528 = 0.0f;
/*0x52C*/ static s8 _data_52C = 0;
/*0x530*/ static u8 _data_530 = 1;
/*0x534*/ static u8 _data_534[] = {
    0x0b, 0x04, 0x06, 0x0a, 0x0a, 0x03, 0x03, 0x0c,
    0x05, 0x02, 0x0e, 0x0f, 0x10, 0x11, 0x12
};
/*0x544*/ static u16 _data_544[] = {
    0x0001, 0x0002, 0x0004, 0x0008, 0x0008, 0x0010, 0x0010, 0x0020,
    0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0080, 0x0001
};
/*0x564*/ static s16 _data_564[] = { 0x000e, 0x0051 };
/*0x568*/ static s16 _data_568[] = { 0x000f, 0x0010, 0x0046, 0x0046, 0x0047, 0x0014 };
/*0x574*/ static s16 _data_574[] = { 0x0015, 0x0021, 0x000d, 0x0022 };
/*0x57C*/ static s16 _data_57C[] = {
    0x004f, 0x0050, 0x0052, 0x00eb, 0x0053, 0x00ec, 0x0055, 0x00e7, 0x0056,
    0x00e8, 0x0058, 0x00e9,0x0059, 0x00ea, 0x0414, 0x0414, 0x0415, 0x0415
};
/*0x5A0*/ static s16 _data_5A0[] = { 0x002d, 0x002e, 0x0038, 0x0039, 0x002f, 0x0030, 0x0031, 0x0032 };
/*0x5B0*/ static s16 _data_5B0[] = { 0x0025, 0x0029, 0x0033, 0x0034 };
/*0x5B8*/ static s16 _data_5B8[] = { 0x003a, 0x003e };
/*0x5BC*/ static s16 _data_5BC[] = {
    0x006e, 0x00b6, 0x00ba, 0x00b8, 0x006f, 0x00b7, 0x00b9, 0x00b5,
    0x00bb, 0x0068, 0x006c, 0x006a, 0x00bc, 0x006b, 0x006d, 0x0069
};
/*0x5DC*/ static s16 _data_5DC[] = { 0x0066, 0x0071 };
/*0x5E0*/ static s16 _data_5E0[] = { 0x0072, 0x0403 };
/*0x5E4*/ static s16 _data_5E4[] = { 0x0404, 0x0070 };
/*0x5E8*/ static s16 _data_5E8[] = {
    0x00bd, 0x0000, 0x0077, 0x00e4, 0x0075, 0x00e3, 0x00d1, 0x00e6, 0x00cc, 0x00e5, 0x0077, 0x0075, 0x0078, 0x007e, 0x0076, 0x007d,
    0x007b, 0x007c, 0x0079, 0x007a, 0x007f, 0x0080, 0x0084, 0x0085, 0x0081, 0x0082, 0x0086, 0x0087, 0x0083, 0x007f, 0x0080, 0x00e4,
    0x00e3, 0x0000, 0x00d1, 0x00cc, 0x00d2, 0x00d3, 0x00cd, 0x00ce, 0x00d8, 0x00d9, 0x00d6, 0x00d7, 0x00da, 0x00db, 0x00df, 0x00e0,
    0x00dc, 0x00dd, 0x00e1, 0x00e2, 0x00de, 0x00da, 0x00db, 0x00e6, 0x00e5, 0x00d4, 0x00d4, 0x0000, 0x3f80, 0x0000, 0x3f80, 0x0000,
    0x3f80, 0x0000, 0x3f80, 0x0000, 0x3f80, 0x0000, 0x3f80, 0x0000, 0x3f80, 0x0000, 0x3f80, 0x0000, 0x3f80, 0x0000, 0x3f80, 0x0000,
    0x3f80, 0x0000, 0x4040, 0x0000, 0x3f80, 0x0000, 0x4040, 0x0000, 0xbf80, 0x0000
};
/*0x69C*/ static s16 _data_69C[] = { 0x0e, 0x10, 0x5c, 0x47, 0x14, 0x15, 0x21, 0x5c, 0x5d, 0x00 };
/*0x6B0*/ static s16 _data_6B0[] = { 0x5a, 0x5b, 0x5a, 0x5b, 0x5c, 0x5d, 0x14, 0x14 };
/*0x6C0*/ static s16 _data_6C0[] = { 0x5a, 0x5b, 0x14, 0x14 };
/*0x6C8*/ static s16 _data_6C8[] = { 0x5a, 0x5b, 0x1c, 0x1d };
/*0x6D0*/ static s16 _data_6D0[] = { 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002a, 0x002b, 0x002c };
/*0x6E0*/ static s16 _data_6E0[] = { 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f, 0x0040, 0x0041 };
/*0x6F0*/ static s16 _data_6F0[] = { 0x0066, 0x0067 };
/*0x6F4*/ static s16 _data_6F4[] = { 0x00ed, 0x00ee };
/*0x6F8*/ static f32 _data_6F8 = { 0.005 };
/*0x6FC*/ static f32 _data_6FC[] = { 0.05f, 0.03f, 0.54f, 0.52f, 0.94f, 0.92f, 1.32, 1.3f, 1.65f, 0.005f };
/*0x724*/ static f32 _data_724[] = { 0.05f };
/*0x728*/ static f32 _data_728[] = { 0.03f, 0.54f, 0.52f, 0.94f, 0.92f, 1.32, 1.3f, 1.65f };
/*0x748*/ static f32 _data_748[] = { 0.005f, 0.05f, 0.03f, 0.55f, 0.53f, 1.65f };
/*0x760*/ static f32 _data_760[] = { 0.005f, 0.05f, 0.03f, 0.55f, 0.53f, 1.65f };
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
/*0x4*/ static f32 _bss_4[1];
/*0x8*/ static f32 _bss_8[2];
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
/*0x38*/ static f32 _bss_38;
/*0x3C*/ static f32 _bss_3C;
/*0x40*/ static u8 _bss_40[0xc]; // srt *?
/*0x4C*/ static f32 _bss_4C; // obj x
/*0x50*/ static f32 _bss_50; // obj y
/*0x54*/ static f32 _bss_54; // obj z
/*0x58*/ static ObjFSA_StateCallback _bss_58[81];
/*0x19C*/ static ObjFSA_StateCallback _bss_19C[1];
/*0x1A0*/ static s16 _bss_1A0[2];
/*0x1A4*/ static s32 _bss_1A4;
/*0x1A8*/ static s16 _bss_1A8;
/*0x1AA*/ static u8 _bss_1AA[0x2];
/*0x1AC*/ static f32 _bss_1AC;
/*0x1B0*/ static f32 _bss_1B0[0x4];
/*0x1C0*/ static u8 _bss_1C0[0x8];
/*0x1C8*/ static f32 _bss_1C8[2];
/*0x1D0*/ static u8 _bss_1D0[0xc];
/*0x1DC*/ static f32 _bss_1DC;
/*0x1E0*/ static u8 _bss_1E0[0x18];
/*0x1F8*/ static f32 _bss_1F8[2];
/*0x200*/ static s16 _bss_200;
/*0x202*/ static s16 _bss_202;
/*0x204*/ static f32 _bss_204;
/*0x208*/ static f32 _bss_208;
/*0x20C*/ static Camera *_bss_20C;
/*0x210*/ static Object *_bss_210[4];
/*0x220*/ static s16 _bss_220[2];
typedef void (*Bss244)(Object*, Player_Data*, f32);
/*0x224*/ static Bss244 _bss_224[1];
/*0x228*/ static u8 _bss_228[0x8];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_0.s")

// offset: 0x3F8 | ctor
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_ctor.s")
#else
static void dll_210_func_0();

void dll_210_ctor(void *dll) {
    dll_210_func_0();
}
#endif

// offset: 0x438 | dtor
void dll_210_dtor(void *dll) { }

// offset: 0x444 | func: 1 | export: 0
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_setup.s")
#else
// matches but requires these as static:
// dll_210_func_4910
// dll_210_func_692C (matched)
// dll_210_func_64B4 (matched)
// dll_210_func_A058 ("matched")
void dll_210_setup(Object* arg0, u32 arg1) {
    s32 i;
    Player_Data* data;

    data = arg0->data;
    obj_add_object_type(arg0, 0);
    obj_add_object_type(arg0, 0x27);
    obj_set_update_priority(arg0, 0x3C);
    obj_init_mesg_queue(arg0, 0x14U);
    arg0->setup = NULL;
    arg0->animCallback = dll_210_func_4910;
    _bss_1A4 = 0;
    data->stats = gDLL_29_Gplay->vtbl->get_player_stats();
    arg0->srt.yaw = gDLL_29_Gplay->vtbl->get_player_saved_location()->rotationY << 8;
    data->unk87C = -1;
    data->unk800 = 1.0f;
    data->unk890 = func_8005D3A4(3);
    data->unk894 = func_8005D3A4(4);
    data->unk898 = func_8005D3A4(5);
    data->unk89C = data->unk890;
    if (arg0->id == 0x1F) {
        data->unk8B4 = 0;
    } else {
        data->unk8B4 = 1;
    }
    gDLL_18_objfsa->vtbl->func0(arg0, &data->unk0, 0x51, 1);
    data->unk0.unk274 = &data->unk774;
    gDLL_27->vtbl->init(&data->unk0.unk4, 
        DLL27FLAG_2000000, 
        DLL27FLAG_1 | DLL27FLAG_2 | DLL27FLAG_4 | DLL27FLAG_20 | DLL27FLAG_80 | DLL27FLAG_40000, 
        DLL27MODE_1);
    gDLL_27->vtbl->setup_hits_collider(&data->unk0.unk4, 1, _data_7C, _data_88, 1);
    gDLL_27->vtbl->setup_terrain_collider(&data->unk0.unk4, 2, _data_3C, _data_6C, _data_20);
    data->unk0.unk4.boundsYExtension = 0x64;
    dll_210_func_7260(arg0, data);
    arg0->objhitInfo->unkA1 = 0x29;
    arg0->unk36 = 0xFF;
    *_bss_1A0 = 0;
    if (arg0->ptr0x64 != NULL) {
        arg0->ptr0x64->flags |= 0x4008;
        arg0->ptr0x64->unk2c = arg0->ptr0x64->unk0 * 0.5f;
    }
    gDLL_1_UI->vtbl->func_12EC();
    data->unk85C = obj_create(obj_alloc_create_info(0x24, 0x212), 5U, -1, -1, arg0->parent);
    data->unk860 = obj_create(obj_alloc_create_info(0x24, 0x403), 5U, -1, -1, arg0->parent);
    data->modAnims = _data_98;
    if (arg0->id == 0) {
        data->unk3B4 = _data_2F8;
        data->unk3B8 = _data_490;
        data->unk3BC = dll_210_func_692C;
        data->unk8A0 = 6U;
    } else {
        data->unk3B4 = _data_190;
        data->unk3B8 = _data_460;
        data->unk3BC = dll_210_func_64B4;
        data->unk8A0 = 6U;
    }
    for (i = 0; i < data->unk8A0; i++) {
        data->unk3B4[i].unk34.ptr = mmAlloc(0x320, 0x1A, NULL);
        obj_load_weapondata(arg0, arg0->id, &data->unk3B4[i].unk34, data->unk3B4[i].unk0, 0U);
    }
    data->unk8BA = main_get_bits(BIT_Spirit_Bits);
    for (i = 0; i < 7; i++) {
        if (main_get_bits(_data_4E0[i]) != 0) {
            data->unk8BB = (u8) (data->unk8BB | (1 << i));
        }
    }
    dll_210_func_A058(arg0);
    *_bss_220 = 0x2D;
    *_bss_224 = dll_210_func_90A0;
}
#endif

// offset: 0x934 | func: 2 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_control.s")
#else
// Requires these as static:
// dll_210_func_11A0 (matched)
// dll_210_func_1CA8 (matched)
// dll_210_func_1DDC (matched)
// dll_210_func_2534
// dll_210_func_7180 (matched)
// dll_210_func_47B8 (matched)
// dll_210_func_1DE64 (matched)
// stack alloc: https://decomp.me/scratch/gO4st
void dll_210_control(Object* arg0) {
    Player_Data* data = arg0->data;
    s32 sp80;
    s32 i;
    s32 var_v0;
    s16* var_v0_3;
    f32 sp70;
    f32 sp6C;
    Object* tempObj;
    s32 pad;
    s32 pad2;
    s32 sp48[6] = _data_4F0;

    if (menu_get_current() == MENU_TITLE_SCREEN) {
        return;
    }

    if (joy_get_buttons(0) & U_JPAD) {
        dll_210_add_scarab(arg0, 1);
    }
    _bss_1AC = gUpdateRateF;
    if (_bss_1AC > 4.0f) {
        _bss_1AC = 4.0f;
    }
    sp6C = dll_210_func_63F0(data, gUpdateRateF);
    sp80 = gUpdateRate;
    if (sp80 >= 5) {
        sp80 = 4;
    }
    if ((arg0->parent == NULL) && (func_8004454C(arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z) < 0)) {
        data->unk0.target = NULL;
        data->unk854 = 0;
        gDLL_2_Camera->vtbl->func17.withOneArg(0);
    }
    _bss_20C = get_main_camera();
    dll_210_func_1DDC(arg0, data, data);
    dll_210_func_1CA8(arg0, data, data);
    if (arg0->parent != NULL) {
        var_v0 = (0x8000 - _bss_20C->srt.yaw);
        var_v0 = (arg0->parent->srt.yaw & 0xFFFF) - (var_v0 & 0xFFFF);
        CIRCLE_WRAP(var_v0)
        data->unk0.unk324 = var_v0 + 0x8000;
    } else {
        data->unk0.unk324 = _bss_20C->srt.yaw;
    }
    data->unk8BC = gDLL_2_Camera->vtbl->func3();
    if (data->unk8BC == 0x56 && data->unk0.animState != 1) {
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &data->unk0, 1);
    }
    data->unk7FC = 100000.0f;
    data->unk8BD = 1;
    data->unk0.unk304 = 0;
    for (i = 0; i < data->unk8AD; i++) { data->unk0.unk304 |= (1 << (data->unk8AE[i] & 0xFF)) & 0xFF; }
    if (data->unk0.unk304 & 1 && data->unk0.animState != 0xC) {
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &data->unk0, 0xC);
    }
    if (data->unk858 == NULL) {
        sp70 = sp6C / _bss_1AC;
        for (i = 0; i < sp80; i++) {
            *_bss_1AA = i;
            dll_210_func_11A0(arg0, data, sp70);
            get_object_child_position(arg0, &arg0->positionMirror.x, &arg0->positionMirror.y, &arg0->positionMirror.z);
            data->unk7EC.x += arg0->speed.x;
            data->unk7EC.y += arg0->speed.y;
            data->unk7EC.z += arg0->speed.z;
        }
    } else {
        *_bss_1AA = 0;
        dll_210_func_11A0(arg0, data, _bss_1AC);
    }
    i = gDLL_1_UI->vtbl->func_E2C((s32)&sp48, 6);
    if (i != -1U) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, (arg0->id != 0 ? _data_4C0 : _data_4CC)[i], 0x7FU, NULL, NULL, 0, NULL);
    }
    dll_210_func_7180(arg0, data, gUpdateRateF);
    if ((data->unk87C != -1) && ((joy_get_pressed(0) & 0x4000) || (data->stats->magic == 0))) {
        data->unk87C = -1;
        data->unk8BF = -1;
        if (*_data_38 != 0) {
            dll_210_func_1DE64(_data_38);
        }
    }
    tempObj = arg0->linkedObject;
    if (tempObj == NULL) {
        arg0->linkedObject = obj_create(obj_alloc_create_info(0x18, _data_24[data->unk8B4]), 4U, -1, -1, arg0->parent);
    } else {
        tempObj->parent = arg0->parent;
    }
    data->unk87E -= gUpdateRate;
    if (data->unk87E < 0) {
        data->unk87E = _data_28[data->unk8A5];
        data->unk8A6 = _data_30[data->unk8A5];
        if (gDLL_2_Camera->vtbl->func3() == 0x56) {
            data->unk8A6 = 0;
        }
    }
    if (data->unk818 > 0.0f) {
        data->unk818 -= gUpdateRateF;
        data->unk81C += gUpdateRateF * data->unk8B9;
        if (data->unk81C < 0.0f) {
            data->unk8B9 = -data->unk8B9;
            data->unk81C = -data->unk81C;
        } else if (data->unk81C > 120.0f) {
            data->unk8B9 = -data->unk8B9;
            data->unk81C = 120.0f - (data->unk81C - 120.0f);
        }
    }
    if (data->unk8BE == 1) {
        data->unk844 += data->unk840 * gUpdateRateF;
        if (data->unk844 >= 40.0f) {
            data->unk844 = 40.0f;
            data->unk840 = 0;
        } else if (data->unk844 <= 0) {
            data->unk844 = 0;
            data->unk840 = 0.2f;
        }
        diPrintf(_rodata_120, &data->unk844, &data->unk840);
    }
    gDLL_2_Camera->vtbl->func24.withOneArg(data->unk8BD);
    data->unk870 = 0;
    data->unk8AD = 0U;
    dll_210_func_2534(arg0, data, data);
    gDLL_27->vtbl->func_1E8(arg0, &data->unk0.unk4, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(arg0, &data->unk0.unk4);
    gDLL_27->vtbl->func_624(arg0, &data->unk0.unk4, gUpdateRateF);
    if (data->unk868 != NULL) {
        dll_210_func_47B8(arg0, data);
    }
}
#endif

// offset: 0x11A0 | func: 3
void dll_210_func_11A0(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 spCC;
    f32 spC8;
    f32 spC4;
    Object* tempObj;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 pad;
    SRT sp9C;
    MtxF sp5C;
    s32 temp_v0_6;
    Func_80057F1C_Struct** sp54;
    DLL27_Data* temp_v0_3;
    f32 sp4C;

    sp4C = 0.0f;
    arg1->unk754 = 0;
    arg1->unk758 = 0;
    arg1->unk764 = 0;
    arg1->unk766 = 0;
    arg1->unk768 = 0;
    if (!(arg1->flags & 0x200000)) {
        if (arg1->unk884 != -1) {
            if ((arg1->unk8BC != 0x56) && (arg1->unk8BC != 0x60)) {
                arg1->unk754 = joy_get_stick_x_buffered(arg1->unk884, *_bss_1AA);
                arg1->unk758 = joy_get_stick_y_buffered(arg1->unk884, *_bss_1AA);
                arg1->unk764 = joy_get_buttons_buffered(arg1->unk884, *_bss_1AA);
                arg1->unk766 = joy_get_pressed_buffered(arg1->unk884, *_bss_1AA);
                arg1->unk768 = joy_get_released_buffered(arg1->unk884, *_bss_1AA);
            }
        }
    }
    arg1->unk760 = arg1->unk754;
    arg1->unk75C = arg1->unk758;
    if (!(arg1->flags & 0x1000)) {
        if (arg1->unk766 & 0x2000) {
            arg1->flags |= 0x1000;
            arg1->unk834 = 0.0f;
        }
    } else {
        if (arg1->unk768 & 0x2000) {
            arg1->flags &= ~0x1000;
            arg1->unk834 = 0.0f;
        }
        arg1->unk834 += arg2;
        if (arg1->unk834 > 100.0f) {
            arg1->unk834 = 100.0f;
        }
    }
    arg1->flags &= ~0x300;
    dll_210_func_618C(arg0, arg1, arg2, arg2);
    if (!(arg1->unk0.unk340 & 2) && !(arg1->unk0.unk340 & 1)) {
        sp9C.yaw = arg0->srt.yaw;
        sp9C.pitch = 0;
        sp9C.roll = 0;
        sp9C.transl.x = 0;
        sp9C.transl.y = 0;
        sp9C.transl.z = 0;
        sp9C.scale = 1.0f;
        matrix_from_srt(&sp5C, &sp9C);
        vec3_transform(&sp5C, arg1->unk0.unk27C, 0, -arg1->unk0.unk278, &arg0->speed.x, &spC8, &arg0->speed.z);
    } else {
        spC4 = fsin16_precise(arg0->srt.yaw);
        temp_fv0 = fcos16_precise(arg0->srt.yaw);
        arg1->unk0.unk27C = (arg0->speed.x * temp_fv0) - (arg0->speed.z * spC4);
        arg1->unk0.unk278 = (-arg0->speed.z * temp_fv0) - (arg0->speed.x * spC4);
    }
    temp_v0_3 = &arg1->unk0.unk4;
    if (!(arg1->unk0.flags & 0x200000)) {
        arg0->speed.y *= 0.97f;
        arg0->speed.y -= arg1->unk0.unk29C * arg2;
    }
    if (((s8*)temp_v0_3)[0x25C] & 0x10) {
        switch (((s8*)temp_v0_3)[0xB8]) {
        case 6:
            arg1->unk87A -= gUpdateRate;
            if (arg1->unk87A <= 0) {
                arg1->unk87A = 0x3C;
                func_8002635C(arg0, NULL, 0x13, 1, 0);
            }
            break;
        case 29:
            spCC = 500.0f;
            tempObj = obj_get_nearest_type_to(OBJTYPE_24, arg0, &spCC);
            if (tempObj != NULL) {
                ((DLL_Unknown *)tempObj->dll)->vtbl->func[7].withThreeArgsCustom(tempObj, arg0, 1.0f);
            }
            break;
        case 26:
            arg1->unk87A -= gUpdateRate;
            if (arg1->unk87A <= 0) {
                arg1->unk87A = 0x3C;
                func_8002635C(arg0, NULL, 0x13, 1, 0);
            }
            break;
        case 8:
            func_8002635C(arg0, NULL, 1, 0, 0);
            break;
        case 28:
            if ((main_get_bits(BIT_21) == 0) && (arg1->unk87C != 0x1D7)) {
                arg1->unk88E = arg1->unk88E + arg2;
                if (arg1->unk88E >= 0x79) {
                    arg1->unk88E -= 0x78;
                    func_8002635C(arg0, NULL, 0x15, 2, 0);
                }
            }
            break;
        case 32:
            if (arg1->unk0.unk278 > 0.5f) {
                temp_fv0 = arg1->unk83C + 0.05f;
                if (temp_fv0 < 0.0f) {
                    arg1->unk83C = temp_fv0;
                } else {
                    arg1->unk83C = 0.0f;
                }
            } else {
                arg1->unk83C -= 0.04f;
                if (_data_1C > 0.0f) {
                    _data_1C -= arg2;
                } else {
                    gDLL_6_AMSFX->vtbl->play_sound(arg0, (rand_next(0, 1) + 0x883), 0x7FU, NULL, NULL, 0, NULL);
                    _data_1C = rand_next(0x28, 0x3C);
                }
            }
            temp_v0_6 = func_80057F1C(arg0, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &sp54, 0, 0x20);
            sp4C = -arg1->unk83C;
            if (temp_v0_6 >= 2) {
                sp4C += sp54[0]->unk0[0] - sp54[temp_v0_6 - 1]->unk0[0];
            }
            if (sp4C > 25.0f) {
                dll_210_add_health(arg0, -1);
                if (arg1->stats->health <= 0) {
                    gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg1->unk0, 0x34);
                } else {
                    gDLL_29_Gplay->vtbl->checkpoint(NULL, 0, 1, map_get_layer());
                    gDLL_29_Gplay->vtbl->start_loaded_game();
                }
            }
            break;
        case 31:
            main_set_bits(BIT_643, 1U);
            break;
        default:
            arg1->unk87A = 0x3C;
            if (arg1->unk83C < 0.0f) {
                temp_fv1 = arg1->unk83C + (arg1->unk0.unk278 * 0.1f);
                if (temp_fv1 < 0.0f) {
                    arg1->unk83C = temp_fv1;
                } else {
                    arg1->unk83C = 0.0f;
                }
                sp4C = -arg1->unk83C;
            }
            break;
        }
        if (sp4C != 0.0f) {
            if (1.0f - (sp4C * 0.05f) < 0.2f) {
                sp4C = 0.2f;
            } else {
                sp4C = 1.0f - (sp4C * 0.05f);
            }
            arg0->speed.x *= sp4C;
            arg0->speed.z *= sp4C;
        }
    }
    if (arg0->speed.x < -3.0f) {
        arg0->speed.x = -3.0f;
    }
    if (arg0->speed.x > 3.0f) {
        arg0->speed.x = 3.0f;
    }
    if (arg0->speed.y < -4.0f) {
        arg0->speed.y = -4.0f;
    }
    if (arg0->speed.y > 4.0f) {
        arg0->speed.y = 4.0f;
    }
    if (arg0->speed.z < -3.0f) {
        arg0->speed.z = -3.0f;
    }
    if (arg0->speed.z > 3.0f) {
        arg0->speed.z = 3.0f;
    }
    temp_fv0 = arg0->srt.scale / arg0->def->scale;
    obj_integrate_speed(arg0, arg0->speed.x * arg2 * temp_fv0, arg0->speed.y * arg2 * temp_fv0, arg0->speed.z * arg2 * temp_fv0);
    dll_210_func_1BC0(arg0, arg1);
}

// offset: 0x1BC0 | func: 4
void dll_210_func_1BC0(Object* arg0, Player_Data* arg1) {
    Object* linkedObj;

    arg0->unkB0 &= ~3;
    if (arg1->unk8A8 == 2) {
        arg0->unkB0 |= 3;
    } else if (arg1->unk8A8 == 0) {
        arg0->unkB0 |= 1;
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
void dll_210_func_1CA8(Object* arg0, Player_Data* arg1, Player_Data* arg2) {
    Object* temp_v0;
    f32 sp28;

    temp_v0 = gDLL_2_Camera->vtbl->func14();
    if (arg1->flags & 0x10) {
        arg1->unk8A9 = 2;
        arg2->unk0.unk33D = 1;
        if (temp_v0 != NULL) {
            arg2->unk0.target = temp_v0;
            return;
        }
        sp28 = 500.0f;
        arg2->unk0.target = obj_get_nearest_type_to(OBJTYPE_4, arg0, &sp28);
        return;
    }
    if (temp_v0 != NULL) {
        if (temp_v0 != arg2->unk0.target) {
            arg2->unk0.unk33D = 0;
            if ((temp_v0->def->unk40->unk10 & 0xF) == 1) {
                arg1->unk8A9 = 2;
                arg2->unk0.unk33D = 1;
            }
        }
        arg2->unk0.target = temp_v0;
        return;
    }
    if (arg2->unk0.target != NULL) {
        obj_send_mesg(arg2->unk0.target, 0xAU, arg0, NULL);
        arg1->unk8A9 = 0;
    }
    arg2->unk0.target = NULL;
    arg2->unk0.unk33D = 0;
}

// offset: 0x1DDC | func: 6
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1DDC.s")
#else
// Matches but requires dll_210_func_24FC, dll_210_func_6DD8 as static
void dll_210_func_1DDC(Object* arg0, Player_Data* arg1, Player_Data* arg2) {
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
                        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 0x3A);
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
            if (sp8C == arg2->unk0.target) {
                arg2->unk0.target = 0;
                arg2->unk0.unk33D = 0;
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
            gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 0x4D);
            dll_210_add_health(arg0, -sp88);
            if (arg1->unk868 != NULL) {
                arg1->unk868->unkE0 = 0;
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
            gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 0x4D);
            dll_210_add_health(arg0, -sp88);
            if (arg1->unk868 != NULL) {
                arg1->unk868->unkE0 = 0;
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
            gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 0x4D);
            func_80023D30(arg0, 0x450, 0.0f, 0);
            dll_210_add_health(arg0, -sp88);
            if (arg1->unk868 != NULL) {
                arg1->unk868->unkE0 = 0;
                arg1->unk868 = NULL;
            }
            break;
        case 0x7000A:
            if (sp88 > 0) {
                if (main_get_bits(sp88) != 0) {
                    obj_send_mesg(sp8C, 0x7000BU, arg0, NULL);
                    if (arg2->unk0.animState != 0x2B) {
                        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 0x2B);
                    }
                } else {
                    main_set_bits(sp88, 1U);
                    if (arg2->unk0.animState != 0x2A) {
                        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 0x2A);
                    }
                }
            } else if (arg2->unk0.animState != 0x2A) {
                gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 0x2A);
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
                gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 5);
                arg1->unk8A9 = 1;
            }
            break;
        case 0x100010:
            arg1->unk870 = 1;
            if (arg1->unk868 == NULL) {
                arg1->unk868 = sp8C;
                arg1->unk86C = sp88 >> 0x10;
                gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 5);
                arg1->unk8A9 = 1;
            }
            break;
        }
    }
}
#endif

// offset: 0x24FC | func: 7
int dll_210_func_24FC(Object *player, Player_Data *data) {
  return data->unk0.animState == 1 || data->unk0.animState == 3 || data->unk0.animState == 4;
}

// offset: 0x2534 | func: 8
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_2534.s")
#else
// https://decomp.me/scratch/I2VWJ
void dll_210_func_2534(Object* arg0, Player_Data* arg1, Player_Data* arg2) {
    s32 sp84;
    s32 sp80;
    s32 sp7C;
    s32 sp78;
    s32 sp74;
    DLL_106* sp70 = NULL;
    u32 sp60[4] = _data_508;
    SRT sp48;
    s16 temp_t5;
    s32 var_t7;
    Vec4f* temp_v1;

    sp84 = func_80025F40(arg0, &sp74, &sp80, &sp7C);
    if ((func_80026724(arg0) != 0) && !(arg1->unk818 > 0.0f)) {
        if ((arg1->unk858 != NULL) && (sp84 != 0)) {
            sp84 = 0x14;
        }
        if (sp84 == 0) {
            return;
        }

        if (arg2->unk0.unk270 != 0) {
            sp84 = 0x1B;
        }
        if ((arg2->unk0.unk341 == 3) && (sp84 >= arg2->unk0.unk343)) {
            return;
        }

        arg2->unk0.unk343 = sp84;
        arg0->curModAnimIdLayered = -1;
        sp78 = -1;
        switch (sp84) {
        case 7:
        case 8:
        case 9:
        case 11:
            sp78 = 0x4F;
            break;
        case 10:
        case 12:
            sp78 = 0x50;
            break;
        case 4:
            sp78 = 0x32;
            break;
        case 5:
            sp78 = 0x30;
            break;
        case 1:
            sp7C = arg1->stats->health;
            break;
        case 20:
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xD9U, 0x7FU, NULL, NULL, 0, NULL);
            break;
        case 21:
            sp78 = 0x31;
            break;
        case 23:
            sp78 = 0x33;
            break;
        case 24:
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xD9U, 0x7FU, NULL, NULL, 0, NULL);
            func_800013BC();
            func_80003B70(1.0f);
            break;
        case 27:
            sp78 = arg2->unk0.unk270;
            if (sp78 == 0xD) {
                dll_210_func_A024(arg0, arg2);
                arg0->srt.transl.x += fsin16_precise(arg0->srt.yaw) * 10.0f;
                arg0->srt.transl.z += fcos16_precise(arg0->srt.yaw) * 10.0f;
                dll_210_func_7260(arg0, arg1);
            }
            break;
        case 28:
            switch (sp7C) {
                case 0x7F:
                gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x184U, 0x7FU, NULL, NULL, 0, NULL);
                    sp7C = 0;
                    dll_210_add_health(arg0, 1);
                    break;
                case 0:
                    if (rand_next(0, 1) != 0) {
                        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x183U, 0x7FU, NULL, NULL, 0, NULL);
                    } else {
                        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x193U, 0x7FU, NULL, NULL, 0, NULL);
                    }
                    break;
                default:
                    gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xB23U, 0x7FU, NULL, NULL, 0, NULL);
                    break;
            }
            break;
        default:
            sp78 = 0x4F;
            break;
        }
        if (arg1->flags & 0x800) {
            sp7C = 0;
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x25BU, 0x7FU, NULL, NULL, 0, NULL);
            temp_v1 = (Vec4f*)arg0->modelInsts[arg0->modelInstIdx]->unk24 + (sp80 * 0x10);
            sp48.transl.x = temp_v1->y + gWorldX;
            sp48.transl.y = temp_v1->z;
            sp48.transl.z = temp_v1->w + gWorldZ;
            gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_328, &sp48, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            sp48.transl.x -= arg0->positionMirror.x;
            sp48.transl.y -= arg0->positionMirror.y;
            sp48.transl.z -= arg0->positionMirror.z;
            sp70 = dll_load_deferred(0x1002U, 1U);
            sp60[1] += rand_next(0, 0x9B);
            sp60[2] += rand_next(0, 0x9B);
            sp48.yaw = 0;
            sp48.pitch = 0;
            sp48.roll = 0;
            sp48.scale = 1.0f;
            sp70->vtbl->func0((s32)arg0, 0, &sp48, 1, -1, &sp60);
            if (sp70 != NULL) {
                dll_unload(sp70);
            }
        } else if (sp7C != 0) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, (u16) arg1->unk3B8[rand_next(0x13, 0x15)], 0x7FU, NULL, NULL, 0, NULL);
            arg1->unk8B9 = 0xC;
            arg1->unk818 = 90.0f;
            arg1->unk81C = 0.0f;
            if (sp78 != -1) {
                arg1->unk868 = NULL;
                arg1->unk708 = NULL;
                gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, sp78);
            }
        }
        dll_210_add_health(arg0, -sp7C);
        if (arg1->stats->health <= 0) {
            gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg2->unk0, 0x34);
        }
    }
}
#endif

// offset: 0x2C7C | func: 9 | export: 2
void dll_210_update(Object* arg0) {
    Object* temp_a0;
    Player_Data* temp_s1;
    f32 temp_fv0;
    s32 pad[2];
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    Object* temp_a1;
    f32 sp44;

    temp_s1 = arg0->data;
    sp44 = gUpdateRateF;
    gDLL_18_objfsa->vtbl->func2(arg0, &temp_s1->unk0, _bss_58);
    if ((temp_s1->unk0.unk341 == 1) && (arg0->linkedObject->objhitInfo->unk48 != 0 || arg0->linkedObject->objhitInfo->unk9D != 0)) {
        arg0->objhitInfo->unk61 = 1;
    }
    if (temp_s1->flags & 2) {
        temp_a1 = temp_s1->unk0.unk4.unkD4;
        if ((temp_a1 != NULL) && (temp_a1->def->flags & 0x40) != 0 && !(temp_a1->def->flags & 0x8000)) {
            func_8005B5B8(arg0, temp_a1, 1);
        } else if ((arg0->parent != NULL) && (temp_a1 == NULL)) {
            func_8005B5B8(arg0, NULL, 1);
        }
    }
    temp_a0 = temp_s1->unk858;
    temp_s1->flags |= 2;
    if ((temp_a0 != NULL) && ((arg0->unkB0 & 0x1000) || temp_s1->unk0.animState == 0x24 || temp_s1->unk0.animState == 0x25)) {
        ((DLL_Unknown *)temp_a0->dll)->vtbl->func[12].withFourArgs((s32)temp_a0, (s32)&sp58, (s32)&sp54, (s32)&sp50);
        gDLL_2_Camera->vtbl->func10(sp58, sp54, sp50);
        dll_210_func_8EA4(arg0, temp_s1, temp_s1->unk858, NULL, NULL, NULL, NULL, 0);
    }
    if (temp_s1->unk0.unk4.mode == 1) {
        if (!(temp_s1->flags & 0x2000) && (temp_s1->unk0.unk4.unk25C != 0)) {
            temp_fv0 = (arg0->positionMirror.y - arg0->positionMirror3.y) / sp44;
            arg0->speed.y = temp_fv0;
            if (temp_fv0 < -4.0f) {
                arg0->speed.y = -4.0f;
            }
            if (arg0->speed.y > 0.0f) {
                arg0->speed.y = 0.0f;
            }
        }
        if (temp_s1->unk0.flags & 0x800000) {
            if (arg0->parent != NULL) {
                arg0->speed.x = (arg0->srt.transl.x - arg0->positionMirror2.x) / sp44;
                arg0->speed.z = (arg0->srt.transl.z - arg0->positionMirror2.z) / sp44;
            } else {
                arg0->speed.x = (arg0->positionMirror.x - arg0->positionMirror3.x) / sp44;
                arg0->speed.z = (arg0->positionMirror.z - arg0->positionMirror3.z) / sp44;
            }
            if (((temp_s1->unk0.unk4.unk25C & 6) && !(temp_s1->unk0.unk4.unk25C & 0x60)) || (temp_s1->unk0.unk4.hitsTouchBits != 0)) {
                sp4C = fsin16_precise(arg0->srt.yaw);
                temp_fv0 = fcos16_precise(arg0->srt.yaw);
                temp_s1->unk0.unk278 = (temp_fv0 * -arg0->speed.z) - (arg0->speed.x * sp4C);
                if (temp_s1->unk0.unk278 > 1.65f) {
                    temp_s1->unk0.unk278 = 1.65f;
                }
                temp_s1->unk0.speed = temp_s1->unk0.unk278;
            }
            temp_s1->unk0.flags &= ~0x800000;
        }
    }
    if (temp_s1->unk8BE == 1) {
        temp_s1->unk844 += temp_s1->unk840 * gUpdateRateF;
        temp_fv0 = temp_s1->unk844;
        if (temp_fv0 >= 40.0f) {
            temp_s1->unk844 = 40.0f;
            temp_s1->unk840 = 0.0f;
            return;
        }
        if (temp_fv0 <= 0.0f) {
            temp_s1->unk844 = 0.0f;
            temp_s1->unk840 = 0.2f;
        }
    }
}

// offset: 0x307C | func: 10 | export: 3
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_print.s")
#else
void dll_210_print(Object* arg0, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4, s8 arg5) {
    Object* sp8C;
    s32 pad;
    Player_Data* data;
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

    data = arg0->data;
    sp80 = arg0->modelInsts[arg0->modelInstIdx];
    if (arg5 == -1 || !(data->flags & 0x4001)) {
        if ((data->unk858 != NULL) && ((arg0->unkB0 & 0x1000) || data->unk0.animState == 0x24 || data->unk0.animState == 0x25)) {
            dll_210_func_8EA4(arg0, data, data->unk858, arg1, arg2, arg3, arg4, 1);
        }
        if (data->unk8BE == 1) {
            dll_210_func_3B40(arg0, arg1, arg2, arg3, arg4);
        }
        gDLL_16->vtbl->func1(arg0);
        temp_a2 = data->unk858;
        if (temp_a2 != 0 && ((arg0->unkB0 & 0x1000) || data->unk0.animState == 0x24 || data->unk0.animState == 0x25)) {
            ((DLL_Unknown*)temp_a2->dll)->vtbl->func[19].withThreeArgs((s32)temp_a2, ((s32*)arg0->def)[1], (s32)temp_a2);
        }
        if (data->unk818 > 0.0f) {
            func_80036FBC(0xC8U, 0U, 0U, data->unk81C);
        }
        arg0->srt.transl.y += data->unk83C;
        draw_object(arg0, arg1, arg2, arg3, arg4, 1.0f);
        arg0->srt.transl.y -= data->unk83C;
        if (data->unk858 != 0 && ((arg0->unkB0 & 0x1000) || data->unk0.animState == 0x24 || data->unk0.animState == 0x25)) {
            func_80034FF0(0);
        }
        if (arg5 != 0) {
            dll_210_func_363C(arg0, data, arg1, arg2, arg3);
        }
        func_80032238(arg0, 4, 2, &data->unk39C);
        func_80031F6C(arg0, 9, &data->unk7EC.x, &data->unk7EC.y, &data->unk7EC.z, 0);
        if (sp80->unk34 & 8) {
            sp8C = data->unk850;
            if (sp8C != NULL && (data->flags & 4)) {
                sp64 = sp8C->srt.scale;
                sp8C->srt.scale /= arg0->srt.scale;
                pad = arg0->unk37;
                if (pad > 0xFF) {
                    pad = 0xFF;
                }
                func_80035AF4(arg1, arg2, arg3, arg4, arg0, sp80, 0, 0, sp8C, 6, arg0->unk37 > 0xFF ? 0xFF : arg0->unk37);
                sp8C->srt.scale = sp64;
            }
        }
        if (data->unk0.animState == 0x29) {
            func_80031F6C(arg0, 7, &data->unk680.unk1C, &data->unk680.unk20, &data->unk680.unk24, 0);
        }
        if (data->unk868 != NULL && data->unk868->unkE0 == 1) {
            func_80031F6C(arg0, 6, &sp7C, &sp78, &sp74, 0);
            func_80031F6C(arg0, 7, &sp70, &sp6C, &sp68, 0);
            sp7C = (sp7C + sp70) * 0.5f;
            sp78 = (sp78 + sp6C) * 0.5f;
            sp74 = (sp74 + sp68) * 0.5f;
            sp7C -= fsin16_precise(arg0->srt.yaw) * data->unk86C;
            sp74 -= fcos16_precise(arg0->srt.yaw) * data->unk86C;
            data->unk868->srt.transl.x = sp7C;
            data->unk868->srt.transl.y = sp78;
            data->unk868->srt.transl.z = sp74;
            data->unk868->srt.yaw = arg0->srt.yaw;
            data->unk868->dll->vtbl->print(data->unk868, arg1, arg2, arg3, arg4, -1);
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
void dll_210_func_363C(Object* arg0, Player_Data* arg1, Gfx** arg2, Mtx** arg3, Vertex** arg4) {
    static SRT _bss_40;
    s32 i;

    gDLL_14_Modgfx->vtbl->func6(arg2, arg3, arg4, 1, arg0->linkedObject);
    if (arg1->unk87C == 0x40) {
        *_data_0 += gUpdateRate;
        if ((*_data_0 >= 0x65) && !(arg0->unkB0 & 0x1000)) {
            dll_210_add_magic(arg0, -1);
            *_data_0 = 0;
        }
    }
    if (((arg1->unk8BF == 1U) || (arg1->unk8BF == -1)) && (arg0->modelInstIdx == 2) && ((arg1->unk8BF == 1) || (arg1->unk8BF == -1))) {
        dll_210_func_1D8EC(arg0, arg1, gUpdateRate);
    }
    if (arg1->unk87C == 0x1D7) {
        gDLL_32->vtbl->func4(arg2, arg3, arg0);
        *_data_0 += gUpdateRate;
        if ((*_data_0 >= 0x65) && !(arg0->unkB0 & 0x1000)) {
            dll_210_add_magic(arg0, -1);
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
    if (gDLL_7_Newday->vtbl->func12() != 0) {
        dll_210_func_41F4(arg0, arg1);
    }
    if (arg1->flags & 0x60000) {
        _bss_40.transl.x = arg0->srt.transl.x;
        _bss_40.transl.y = arg0->srt.transl.y;
        _bss_40.transl.z = arg0->srt.transl.z;
        if (arg1->flags & 0x40000) {
            gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_427, &_bss_40, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_427, &_bss_40, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_427, &_bss_40, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
        if (arg1->flags & 0x20000) {
            for (i = 0; i < 0x14; i++) {
                gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_427, &_bss_40, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_3B40.s")
#else
// https://decomp.me/scratch/oPG0g
void dll_210_func_3B40(Object* arg0, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle **arg4) {
    Vertex* sp74;
    Vtx_t *curVtx;
    Vtx_t* sp70;
    s32 var_v0_2;
    f32 sp64;
    s32 i;
    SRT sp48;
    Player_Data* sp44;
    Gfx* temp_v0_4;

    sp44 = arg0->data;
    sp64 = sp44->unk844;
    sp74 = *arg3;
    curVtx = sp74;
    if (sp64 < 0.0f) {
        if (i) {}
    } else {
        curVtx = sp74;
    }
    dl_set_prim_color(arg1, 0xFFU, 0xFFU, 0xFFU, 0x80U);
    gSPGeometryMode(*arg1, 0xFFFFFF, G_FOG| G_CULL_BACK | G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER);
    dl_apply_geometry_mode(arg1);
    gDPSetCombineLERP(*arg1, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dl_apply_combine(arg1);

    for (i = 0; i < 8; i++) {
        if (i < 4) {
            curVtx->ob[1] = 800;
        } else {
            curVtx->ob[1] = ((40.0f - sp64) * 20.0f);
        }
        if (i < 4) {
            curVtx->ob[0] = _rodata_C0[i].x * 20.0f;
            curVtx->ob[2] = _rodata_C0[i].z * 20.0f;
        } else {
            curVtx->ob[0] = _rodata_C0[i].x * 20.0f;
            curVtx->ob[2] = _rodata_C0[i].z * 20.0f;
        }
        curVtx->cn[0] = 0xFF;
        curVtx->cn[1] = 0;
        curVtx->cn[2] = 0;
        curVtx->cn[3] = 0x40;
        curVtx++;
    }
    if (curVtx) {}
    gDPSetOtherMode(*arg1, 0xEF182C00, 0xCB104B70);
    dl_apply_other_mode(arg1);
    sp48.transl.x = arg0->srt.transl.x;
    sp48.transl.y = arg0->srt.transl.y;
    sp48.transl.z = arg0->srt.transl.z;
    sp48.yaw = arg0->srt.yaw;
    sp48.scale = 0.05f;
    sp48.pitch = 0;
    sp48.roll = 0;
    func_800032C4(arg1, arg2, &sp48, 1.0f, 0.0f, NULL);
    gSPVertex((*arg1)++, OS_PHYSICAL_TO_K0(sp74), 8, 0);
    dl_triangles(arg1, (DLTri* ) _rodata_0, 0xC);
    if (sp44->unk844 >= 68.0f) {
        var_v0_2 = arg0->unk36 - (gUpdateRate * 4);
        if (var_v0_2 < 0) {
            var_v0_2 = 0;
        }
        arg0->unk36 = var_v0_2;
    }
    *arg3 = curVtx;
}
#endif

// offset: 0x3E50 | func: 13 | export: 4
void dll_210_free(Object* arg0, UNK_TYPE_32 arg1) {
    s32 i;
    Player_Data* data;

    data = arg0->data;
    if (arg0->linkedObject != NULL) {
        obj_destroy_object(arg0->linkedObject);
    }

    arg0->linkedObject = NULL;
    for (i = 0; i < data->unk8A0; i++) {
        if (data->unk3B4[i].unk34.ptr != NULL) {
            mmFree(data->unk3B4[i].unk34.ptr);
        }
    }

    obj_free_object_type(arg0, OBJTYPE_PLAYER);
    obj_free_object_type(arg0, OBJTYPE_39);
}

// offset: 0x3F40 | func: 14 | export: 5
u32 dll_210_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS | MODFLAGS_80 | MODFLAGS_100;
}

// offset: 0x3F50 | func: 15 | export: 6
u32 dll_210_get_data_size(Object *self, s32 arg1) {
    return sizeof(Player_Data);
}

// offset: 0x3F64 | func: 16 | export: 63
s32 dll_210_func_3F64(Object* arg0) {
    Player_Data *data = arg0->data;

    if (arg0->linkedObject == NULL) {
        arg0->linkedObject = obj_create(obj_alloc_create_info(sizeof(ObjSetup), _data_24[data->unk8B4]), 4U, -1, -1, arg0->parent);
        arg0->unkB0 &= ~3;
        arg0->unkB0 |= 3;
        func_80023D30(arg0->linkedObject, 0, 1.0f, 0U);
        return 1;
    }

    return 0;
}

// offset: 0x4038 | func: 17 | export: 64
void dll_210_func_4038(Object* arg0, Object *arg1, s32 arg2) {
    Player_Data* objdata = arg0->data;
    u16 temp;
    s16 sp28[8] = _data_518;

    if (arg2 != 1) {
        func_80023894(arg1, sp28[arg2]);
        objdata->unk872 |= 1 << arg2;
        temp = main_get_bits(BIT_2DA);
        temp |= 1 << arg2;;
        main_set_bits(BIT_2DA, temp);
    }
}

// offset: 0x4114 | func: 18 | export: 45
Object *dll_210_func_4114(Object* player) {
    Player_Data* objdata = player->data;

    if (objdata->unk8B5 == 7) {
        return objdata->unk6B0.unk38;
    }
    return 0;
}

// offset: 0x413C | func: 19 | export: 46
u8 dll_210_func_413C(Object* arg0) {
    Player_Data *objdata = arg0->data;

    return objdata->unk8B5 != 5 && objdata->unk8B5 != 4 && objdata->unk8B5 != 9 && objdata->unk8B5 != 0xA && objdata->unk8B5 != 0x11;
}

// offset: 0x4198 | func: 20 | export: 48
s16 dll_210_func_4198(Object* player) {
    Player_Data* objdata = player->data;
    Object* heldObject;

    if (objdata->flags & 0x10000) {
        heldObject = objdata->unk868;
        if (heldObject != NULL) {
            return heldObject->id;
        }
    }
    return 0;
}

// offset: 0x41CC | func: 21 | export: 49
s16 dll_210_func_41CC(Object* player) {
    Player_Data* objdata = player->data;
    return !(objdata->flags & 0x100000);
}

// offset: 0x41F4 | func: 22
void dll_210_func_41F4(Object* arg0, Player_Data* arg1) {
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
        gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_C9, &sp48, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, &sp3C);
    }
    arg1->unk8A6--;
}

// offset: 0x43EC | func: 23 | export: 65
void dll_210_func_43EC(Object* arg0, f32 arg1, f32 arg2, f32 arg3) {
    Player_Data* objdata;

    arg0->positionMirror3.x = arg1;
    arg0->positionMirror2.x = arg1;
    arg0->positionMirror.x = arg1;
    arg0->srt.transl.x = arg1;

    arg0->positionMirror3.y = arg2;
    arg0->positionMirror2.y = arg2;
    arg0->positionMirror.y = arg2;
    arg0->srt.transl.y = arg2;

    arg0->positionMirror3.z = arg3;
    arg0->positionMirror2.z = arg3;
    arg0->positionMirror.z = arg3;
    arg0->srt.transl.z = arg3;

    objdata = arg0->data;
    dll_210_func_7260(arg0, objdata);
    gDLL_18_objfsa->vtbl->set_anim_state(arg0, &objdata->unk0, 1);
}

// offset: 0x44A4 | func: 24 | export: 66
s32 dll_210_func_44A4(Object* arg0, s32 arg1) {
    s32 i;
    Player_Data* objdata2;
    Player_Data* objdata1;
    s32 temp;

    objdata1 = arg0->data;
    objdata2 = arg0->data;
    switch (arg1) {
    case 1:
        if (objdata1->unk0.unk304 & 0x1000) {
            return 0;
        }
        if (arg0->unkB0 & 0x1000) {
            return 0;
        }
        return 1;
    case 2:
        switch (objdata1->unk0.animState) {
            case 1:
                temp = 0;
                break;
            case 4:
                i = 0;
                while (arg0->curModAnimId != objdata2->modAnims[i] && i < 15) {
                    i += 3;
                }
                temp = i / 3;
                break;
            default:
                temp = 5;
                break;
        }
        return temp;
    case 8:
        return _bss_1A8 > 0;
    case 9:
        return objdata1->unk0.unk341 == 3;
    case 10:
        return objdata2->flags & 0x200;
    case 11:
        return objdata2->flags & 0x100;
    case 13:
        return objdata1->unk0.unk33D == 1;
    case 14:
        return objdata2->unk87C;
    case 15:
        return (s32)objdata2->unk85C;
    case 16:
        return (s32)objdata2->unk860;
    case 18:
        return objdata2->unk872;
    case 19:
        if (objdata2->unk858 != NULL) {
            return objdata2->unk858->id;
        }
        return 0;
    default:
        return 0;
    }
}

// offset: 0x4634 | func: 25 | export: 67
void dll_210_func_4634(Object* arg0, s32 arg1, f32 arg2) {
    Player_Data* objdata1;
    Player_Data* objdata2;

    objdata2 = arg0->data;
    objdata1 = arg0->data;
    switch (arg1) {
    case 1:
        if (objdata1->unk8AD < 4) {
            objdata1->unk8AE[objdata1->unk8AD] = arg2;
            objdata1->unk8AD++;
            return;
        }
    default:
        return;
    case 6:
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &objdata2->unk0, 0x47);
        return;
    case 3:
        if (objdata2->unk0.animState != 0x46) {
            gDLL_18_objfsa->vtbl->set_anim_state(arg0, &objdata2->unk0, 0x46);
        }
        break;
    case 5:
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &objdata2->unk0, 1);
        return;
    case 9:
        if (*_bss_1A0 == 0) {
            gDLL_28_ScreenFade->vtbl->fade(0x1E, 1);
            *_bss_1A0 = 0x64;
        }
        break;
    case 10:
        objdata1->flags |= 0x80000;
        return;
    case 11:
        objdata1->flags &= ~0x80000;
        break;
    }
}

// offset: 0x47B8 | func: 26
void dll_210_func_47B8(Object* arg0, Player_Data* arg1) {
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
// dll_210_func_1DB6C (matched)
// dll_210_func_63F0 (matched)
// dll_210_func_9F1C (matched)
// dll_210_func_60A8 (matched)

int dll_210_func_4910(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    static s8 _bss_0;
    static s16 _bss_2;
    ObjSetup* temp_s1;
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
    Player_Data* temp_fp;
    Vec3f* temp_s0;
    Object* temp_v0_6;
    f32 *temp_data_528 = &_data_528;
    f32 sp6C[3];
    s16 sp6A;
    s8 *temp_data_52C = &_data_52C;
    f32 sp60;
    Object *tempObj;

    temp_fp = arg0->data;
    temp_s1 = arg1->setup;
    spC8 = 0;
    arg2->unkF4 = (AnimObj_DataF4Callback)dll_210_func_60A8;
    temp_fp->unk818 = 0.0f;
    _bss_1AC = dll_210_func_63F0(temp_fp, gUpdateRateF);
    if (_bss_1AC > 4.0f) {
        _bss_1AC = 4.0f;
    }
    if (temp_fp->flags & 0x8000) {
        if (temp_fp->stats->health > 0) {
            gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 1);
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
                CIRCLE_WRAP(arg2->yawDiff)
                arg2->pitchDiff = arg0->srt.pitch - (arg1->srt.pitch & 0xFFFF);
                CIRCLE_WRAP(arg2->pitchDiff)
                arg2->rollDiff = (arg1->srt.roll  & 0xFFFF) - (arg0->srt.roll & 0xFFFF);
                CIRCLE_WRAP(arg2->rollDiff)
                arg2->unk62 = 2;
            }
            arg2->unk58 -= arg2->unk24 * gUpdateRateF;
            if (arg2->unk58 <= 0.0f) {
                arg2->unk62 = 0;
            }
            arg0->curModAnimIdLayered = -1;
        } else if (arg2->unk62 == 4) {
            arg2->unk7A &= 0xFFF3;
            arg2->unk7C &= 0xFFF7;
            temp_v0_6 = (Object *)gDLL_2_Camera->vtbl->func15.asVoidS32();
            if (temp_v0_6 == NULL || temp_v0_6->unk74 == NULL) {
                return 0;
            }
            temp_s0 = temp_v0_6->unk74;
            spB4 = arg0->srt.transl.x - temp_s0->x;
            spB0 = arg0->srt.transl.z - temp_s0->z;
            spA4 = (arg0->srt.transl.y + 30.0f) - temp_s0->y;
            _bss_2 = arctan2_f(spB4, spB0);
            temp_fv0_4 = sqrtf(SQ(spB4) + SQ(spB0));
            arg2->yawDiff = _bss_2 - (arg0->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(arg2->yawDiff)
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
                        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 0x24);
                        return 1;
                    }
                    gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 1);
                    return 1;
                }
                temp_fv0_2 = arg2->unk58;
                arg2->unk58 += (arg2->unk24 * gUpdateRateF);
                if (arg2->unk58 > 1.0f) {
                    arg2->unk58 = 1.0f;
                }
                temp_fv0_2 = arg2->unk58 - temp_fv0_2;
                arg0->srt.yaw += (s16) (temp_fv0_2 * arg2->yawDiff);
                temp_s0_2[1] = _bss_2 - (arg0->srt.yaw & 0xFFFF);
                CIRCLE_WRAP(temp_s0_2[1])
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
                CIRCLE_WRAP(var_v0)
                WRAP(var_v0 , -0x4000, 0x4000)
                arg0->srt.yaw -= (var_v0 * gUpdateRate) >> 3;
                if (*temp_data_52C >= 7) {
                    var_v0 = 0;
                }
                if ((var_v0 < 0x100) && (var_v0 >= -0xFF)) {
                    arg0->curModAnimIdLayered = -1;
                    arg2->unk7A = arg2->unk7C;
                    arg2->unk62 = 0;
                    arg2->animCurvesCurrentFrameB = arg2->animCurvesCurrentFrameA - 1;
                    spC8 = 0;
                } else {
                    temp_fp->unk0.unk288 = 0.0f;
                    temp_fp->unk0.unk284 = 0.0f;
                    gDLL_18_objfsa->vtbl->func3(&arg0->srt);
                    temp_fp->unk0.unk310 = 0;
                    temp_fp->unk0.unk30C = 0;
                    arg0->unkDC = 0;
                    temp_fp->unk0.unk324 = 0;
                    temp_fp->unk0.unk4.mode = 1;
                    temp_fp->unk8B8 = 0;
                    gDLL_18_objfsa->vtbl->tick(arg0, &temp_fp->unk0, gUpdateRateF, gUpdateRateF, _bss_58, _bss_19C);
                }
            } else {
                spAC = spAC / temp_fv0_4;
                spA8 = spA8 / temp_fv0_4;
                temp_fp->unk0.unk288 = -spAC * 40.0f;
                temp_fp->unk0.unk284 = spA8 * 40.0f;
                arg0 = arg0;
                arg0->srt.transl.x = arg2->unk4C.x + (sp9C * spAC);
                arg0->srt.transl.z = arg2->unk4C.z + (sp9C * spA8);
                gDLL_18_objfsa->vtbl->func3(&arg0->srt);
                temp_fp->unk0.unk310 = 0;
                temp_fp->unk0.unk30C = 0;
                arg0->unkDC = 0;
                temp_fp->unk0.unk324 = 0;
                temp_fp->unk0.unk4.mode = 1;
                temp_fp->unk8B8 = 0;
                gDLL_18_objfsa->vtbl->tick(arg0, &temp_fp->unk0, gUpdateRateF, gUpdateRateF, _bss_58, _bss_19C);
            }
            *temp_data_528 = sp9C;
        }
        if (arg2->unk62 == 0) {
            gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 1);
        }
    } else {
        arg2->unk7A |= arg2->unk7C & 0xFBFF;
        temp_fp->unk0.unk340 = 0;
        temp_fp->unk0.unk324 = 0;
        temp_fp->unk0.unk310 = 0;
        temp_fp->unk0.unk30C = 0;
        temp_fp->unk0.unk4.mode = 0;
        temp_fp->unk0.unk288 = 0.0f;
        temp_fp->unk0.unk284 = 0.0f;
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
                    ((DLL_Unknown *)var_s0->dll)->vtbl->func[14].withTwoArgs((s32)var_s0, 2);
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
                    gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 0x24);
                }
                break;
            case 2:
                gDLL_2_Camera->vtbl->func8(0, 1);
                gDLL_3_Animation->vtbl->func19(0x54, 4, 0, 0);
                var_s0 = temp_fp->unk858;
                if (var_s0 != NULL) {
                    ((DLL_Unknown *)var_s0->dll)->vtbl->func[14].withTwoArgs((s32)var_s0, 0);
                    arg0->srt.flags &= 0xFFF7;
                    arg0->ptr0x64->flags &= ~0x1000;
                    var_s0 = NULL;
                    arg2->unk7A |= 4;
                    temp_fp->unk858 = NULL;
                    arg0->curModAnimIdLayered = -1;
                    gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 1);
                }
                break;
            case 4:
                var_s0 = temp_fp->unk858;
                gDLL_3_Animation->vtbl->func19(0x57, 0, 0, 0);
                temp_fp->unk76C = NULL;
                if ((var_s0 != NULL) && (var_s0->id == 0x22)) {
                    gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 0x22);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 0x24);
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
                gDLL_18_objfsa->vtbl->set_anim_state(arg0, &temp_fp->unk0, 0x23);
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
                sp6A = gDLL_22_Subtitles->vtbl->func_214C((s32) arg0->unkC4->id);
                gDLL_22_Subtitles->vtbl->func_2248(1U);
                gDLL_22_Subtitles->vtbl->func_368((u16) sp6A);
                arg0->unkC4 = NULL;
                break;
            case 13:
                tempObj = arg0->unkC4;
                gDLL_3_Animation->vtbl->func30(tempObj->id, tempObj, 0);
                dll_210_func_1DB6C(arg0->unkC4, 29.0f);
                gDLL_3_Animation->vtbl->func17(arg0->unkDC, arg0, -1);
                break;
            case 14:
                if (*_data_14 == 1) {
                    *_data_18 = -1;
                    tempObj = temp_fp->unk85C;
                    ((DLL_Unknown*)tempObj->dll)->vtbl->func[12].withTwoArgs((s32)tempObj, (s16)arg0->unkE0);
                    arg2->unk9D |= 4;
                    arg0->unkC4 = NULL;
                    return 4;
                }
                if ((*_data_14 != 0) || (arg0->unkDC < 0)) {
                    arg2->unk9D |= 4;
                    arg0->unkC4 = NULL;
                } else {
                    *_data_18 = arg0->unkE0;
                }
                arg0->unkDC -= 1;
                break;
            case 17:
                dll_210_func_1D8B8(arg0);
                break;
            case 15:
                func_8005B5B8(arg0, NULL, 1);
                break;
            case 16:
                sp60 = 400.0f;
                tempObj = obj_get_nearest_type_to(OBJTYPE_7, arg0, &sp60);
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
                    gDLL_1_UI->vtbl->func_1338(temp_fp->unk708->def->unkAA, 0xA0, 0x8C);
                }
            } else {
                gDLL_1_UI->vtbl->func_130C(temp_fp->unk708->def->unkA2, 0xA0, 0x8C);
            }
            if (arg2->unk8D == 1) {
                gDLL_3_Animation->vtbl->func19(0x54, 3, 0, 0);
                obj_send_mesg(temp_fp->unk708, 0x7000BU, arg0, NULL);
                temp_fp->unk708 = NULL;
            }
        }
    }
    temp_a0_4 = temp_fp->unk858;
    if ((temp_a0_4 != NULL) && (((DLL_Unknown*)temp_a0_4->dll)->vtbl->func[13].withOneArgS32((s32)temp_a0_4) == 2)) {
        arg2->unk7A &= 0xFFFC;
    }
    ((void (*)(Object*, Player_Data*, f32)) temp_fp->unk3BC)(arg0, temp_fp, gUpdateRateF);
    dll_210_func_1BC0(arg0, temp_fp);
    return spC8;
}
#endif

// offset: 0x60A8 | func: 28
void dll_210_func_60A8(Object* arg0, s32 arg1, s32 arg2) {
    Player_Data* sp24;

    sp24 = arg0->data;
    arg0->unkB0 &= 0xEFFF;
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
static void dll_210_func_618C(Object* arg0, Player_Data* arg1, s32 arg2, f32 arg3) {
    Player_Data3B4 sp34;

    arg1->unk0.unk29C = 0.14f;
    arg1->unk0.unk288 = arg1->unk760;
    arg1->unk0.unk284 = arg1->unk75C;
    arg1->unk0.unk310 = (s32) arg1->unk766;
    arg1->unk0.unk30C = (s32) arg1->unk764;
    if (arg1->unk0.unk304 & 0x100) {
        arg1->unk0.unk29C = -0.17f;
    }
    if (*_bss_1A0 > 0) {
        *_bss_1A0 -= arg2;
        if (*_bss_1A0 <= 0) {
            *_bss_1A0 = -1;
            if (gDLL_29_Gplay->vtbl->restart_is_set() != 0) {
                gDLL_29_Gplay->vtbl->restart_goto();
            } else {
                gDLL_29_Gplay->vtbl->start_loaded_game();
            }
            obj_send_mesg_many(0, 3U, arg0, 0xE0000U, arg0);
        }
    }
    arg0->objhitInfo->unk5F = 0;
    arg0->objhitInfo->unk60 = 0;
    arg0->objhitInfo->unk5D = 0;
    arg0->objhitInfo->unk5E = 0;
    arg1->unk0.unk4.mode = 1;
    func_8004D880(arg0);
    arg1->unk8B8 = 0;
    arg1->flags &= ~0x2000;
    arg1->unk0.flags |= 0x01000000;
    if ((s8)dll_210_func_7E6C(arg0, arg1, arg1, &sp34, arg3, 0xC0) == 0xA) {
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &arg1->unk0, 0x11);
    }
    gDLL_18_objfsa->vtbl->tick(arg0, &arg1->unk0, arg3, _bss_1AC, _bss_58, _bss_19C);
    arg1->unk0.flags &= ~0x01000000;
    ((void (*)(Object*, Player_Data*, f32))arg1->unk3BC)(arg0, arg1, gUpdateRateF);
}

// offset: 0x63F0 | func: 30
f32 dll_210_func_63F0(Player_Data* arg0, f32 updateRate) {
    switch (arg0->unk8B7) {
    case 1:
        arg0->unk800 *= 0.85f;
        if (arg0->unk800 < arg0->unk804) {
            arg0->unk800 = arg0->unk804;
        }
        updateRate *= arg0->unk800;
        break;
    case 2:
        arg0->unk800 += 0.02f;
        if (arg0->unk800 >= 1.0f) {
            arg0->unk800 = 1.0f;
            arg0->unk8B7 = 0;
        }
        updateRate *= arg0->unk800;
        break;
    }
    return updateRate;
}

// offset: 0x64B4 | func: 31
void dll_210_func_64B4(Object* arg0, Player_Data* arg1, f32 arg2) {
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
            if (animState->unk58[0] == 0) {
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
void dll_210_func_692C(Object* arg0, Player_Data* arg1, f32 arg2) {
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
            if (animState->unk58[0] == 0) {
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

void dll_210_func_6DD8(Object* obj, Player_Data* objdata, s32 arg2) {
    Object* sp3C;
    s16 temp_v1_2;
    void* temp_v0_2;
    Vec3f* temp_v1;
    void* temp_v1_3;
    s16 temp;

    if (objdata->stats->magic == 0) {
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
            if (objdata->unk87C == 0x40) {
                objdata->unk8BF = -1;
                arg2 = -1;
                dll_210_func_1DE64(_data_38);
            } else {
                objdata->unk8BF = 1;
                dll_210_func_1DE50((s32) obj, (s32) _data_38, 1);
                func_80023A18(obj, 2);
                *_data_0 = 0;
                gDLL_18_objfsa->vtbl->set_anim_state(obj, &objdata->unk0, 1);
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
            sp3C = (Object *)gDLL_2_Camera->vtbl->func15.asVoidS32();
            if (sp3C != NULL) {
                if ((sp3C->def->unk40->unk10 & 0xF) == 3) {
                    obj->unkDC = 4;
                    gDLL_3_Animation->vtbl->func17(3, obj, -1);
                    if (sp3C->unk74 != NULL) {
                        temp = arctan2_f(sp3C->unk74->x - obj->srt.transl.x, sp3C->unk74->z - obj->srt.transl.z) + 0x8000;
                        gDLL_3_Animation->vtbl->func28(obj->unkB4, temp);
                    }
                    obj->unkC4 = sp3C;
                }
            } else {
                break;
            }
        case 0x5BD:
            arg2 = -1;
            sp3C = (Object *)gDLL_2_Camera->vtbl->func15.asVoidS32();
            if ((sp3C != NULL) && (sp3C->id == 0x414 || sp3C->id == 0x4A9)) {
                gDLL_3_Animation->vtbl->func17(5, obj, -1);
                arg2 = 0x5BD;
                temp = arctan2_f(sp3C->unk74->x - obj->srt.transl.x, sp3C->unk74->z - obj->srt.transl.z) + 0x8000;
                gDLL_3_Animation->vtbl->func28(obj->unkB4, temp);
            }
            break;
    }
    objdata->unk87C = arg2;
}
#endif

// offset: 0x7180 | func: 34
void dll_210_func_7180(Object* arg0, Player_Data* arg1, f32 updateRate) {
    s32* sp2C;
    s32* temp_v0;

    func_800328F0(arg0, &arg1->unk354, arg1->unk0.unk278);
    if (arg1->stats->health > 0) {
        func_80032A08(arg0, &arg1->unk354);
        return;
    }
    sp2C = func_800348A0(arg0, 5, 0);
    temp_v0 = func_800348A0(arg0, 4, 0);
    if (sp2C != NULL) {
        *sp2C = 0x200;
    }
    if (temp_v0 != NULL) {
        *temp_v0 = 0x200;
    }
}

// offset: 0x7260 | func: 35
static void dll_210_func_7260(Object* arg0, Player_Data* arg1) {
    arg1->unk874 = (s16) arg0->srt.yaw;
    gDLL_27->vtbl->reset(arg0, &arg1->unk0.unk4);
    arg0->objhitInfo->unk10.x = arg0->srt.transl.x;
    arg0->objhitInfo->unk10.y = arg0->srt.transl.y;
    arg0->objhitInfo->unk10.z = arg0->srt.transl.z;
    arg0->objhitInfo->unk20.x = arg0->positionMirror.x;
    arg0->objhitInfo->unk20.y = arg0->positionMirror.y;
    arg0->objhitInfo->unk20.z = arg0->positionMirror.z;
}

// offset: 0x7300 | func: 36
static s32 dll_210_func_7300(Object* arg0, Player_Data* arg1, Func_80059C40_Struct* arg2, Player_Data490* arg3, Vec3f* arg4, f32 arg5) {
    Player_Data *data = arg0->data;
    f32 f12;
    f32 f0;
    f32 spC[4];

    arg3->unk1C.x = arg2->unk1C.x;
    arg3->unk1C.y = arg2->unk1C.y;
    arg3->unk1C.z = arg2->unk1C.z;
    arg3->unk1C.w = arg2->unk1C.w;
    spC[0] = -arg3->unk1C.z;
    spC[1] = 0.0f;
    spC[2] = arg3->unk1C.x;
    spC[3] = -((spC[0] * arg2->unk4) + (spC[2] * (0, arg2)->unk14));
    f0 = (arg4->z * spC[2]) + ((spC[0] * arg4->x) + (spC[1] * arg4->y)) + spC[3];
    if (f0 < (*_data_70 + 0.5f)) {
        return 0;
    }
    spC[0] = -spC[0];
    spC[1] = 0.0f;
    spC[2] = -spC[2];
    spC[3] = -((spC[0] * arg2->unk8) + (spC[2] * (0, arg2)->unk18));
    f12 = ((arg4->z * spC[2]) + ((spC[0] * arg4->x) + (spC[1] * arg4->y)) + spC[3]);
    if (f12 < (*_data_70 + 0.5f)) {
        return 0;
    }
    arg3->unk2C.x = arg4->x;
    arg3->unk2C.y = arg4->y;
    arg3->unk2C.z = arg4->z;
    arg3->unk38.x = arg1->unk7EC.x;
    arg3->unk38.y = 0.0f;
    arg3->unk38.z = arg1->unk7EC.z;
    arg3->unk4 = ((arg2->unk38.z - arg2->unk38.y) * arg2->unk48) + arg2->unk38.y;
    arg3->unk8 = arg0->positionMirror2.y;
    arg3->unk0 = arg3->unk4 - arg3->unk8;
    arg3->unk46 =  arg2->unk50;
    if (arg2->unk50 == 6) {
        if (data->unk0.unk4.unk25C & 0x10) {
            if (arg5 < 9.0f) {
                if (arg3->unk0 <= 64.0f && arg3->unk0 > 32.0f) {
                    return 2;
                }

                if (arg3->unk0 <= 32.0f && *_bss_1C8 <= arg3->unk0) {
                    return 3;
                }
            }
        } else {
            return 6;
        }
    } else {
        if (_bss_1DC < arg3->unk0) {
            arg3->unk0 = _bss_1DC;
        }
        return 3;
    }
    
    return 0;
}

// offset: 0x75B0 | func: 37
static s32 dll_210_func_75B0(Object* arg0, Func_80059C40_Struct* arg1, Player_Data490* arg2, Vec3f* arg3, f32 arg4, f32 arg5) {
    Player_Data *objdata = arg0->data;
    f32 sp4[4];
    f32 temp_fv1;
    f32 temp;
    f32 f0;

    arg2->unk1C.x = arg1->unk1C.x;
    arg2->unk1C.y = arg1->unk1C.y;
    arg2->unk1C.z = arg1->unk1C.z;

    arg2->unk1C.x = -arg2->unk1C.x;
    arg2->unk1C.y = -arg2->unk1C.y;
    arg2->unk1C.z = -arg2->unk1C.z;
    arg2->unk1C.w = -arg1->unk1C.w;

    arg2->unk2C.x = arg3->x;
    arg2->unk2C.y = arg3->y;
    arg2->unk2C.z = arg3->z;

    arg2->unk4 = ((arg1->unk10 - arg1->unkC) * arg1->unk48) + arg1->unkC;
    arg2->unk8 = arg1->unkC;
    arg2->unk0 = arg2->unk4 - arg2->unk8;

    arg2->unk46 = arg1->unk50;

    sp4[0] = -arg2->unk1C.z;
    sp4[1] = 0.0f;
    sp4[2] = arg2->unk1C.x;
    sp4[3] = -((sp4[0] * arg1->unk4) + (sp4[2] * (0, arg1)->unk14));
    temp_fv1 = ((arg3->z * sp4[2]) + ((sp4[0] * arg3->x) + (sp4[1] * arg3->y))) + sp4[3];
    // @fake
    temp = temp_fv1;
    sp4[0] = -sp4[0];
    sp4[1] = 0.0f;
    sp4[2] = -sp4[2];
    sp4[3] = -((sp4[0] * arg1->unk8) + (sp4[2] * (0, arg1)->unk18));
    f0 = ((arg3->z * sp4[2]) + ((sp4[0] * arg3->x) + (sp4[1] * arg3->y))) + sp4[3];
    if (temp_fv1 < f0) {
        arg2->unk47 = 0;
    } else {
        arg2->unk47 = 1;
    }
    if (arg4 <= (objdata->unk0.unk278 * arg5) || arg4 <= 3.5f) {
        if (arg1->unk50 == 2 || arg1->unk50 == 0x11) {
            return 4;
        }
        if (objdata->unk0.unk278 >= 0.94f) {
            return 5;
        }
        if (arg1->unk50 != 4) {
            return 4;
        }
        return 5;
    }
    return 0;
}

// offset: 0x77DC | func: 38
static s32 dll_210_func_77DC(Object *arg0, Func_80059C40_Struct* arg1, UnkArg2* arg2, Vec3f* arg3) {
    arg2->unk0.x = arg1->unk1C.x;
    arg2->unk0.y = arg1->unk1C.y;
    arg2->unk0.z = arg1->unk1C.z;
    arg2->unk0.w = arg1->unk38.x;
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
static s32 dll_210_func_78A8(Object* arg0, Player_Data* arg1, Func_80059C40_Struct* arg2, UnkArg3* arg3, s32 arg4) {
    arg3->unk3 = 0;
    arg3->unk48.x = ((arg2->unk8 - arg2->unk4) * 0.5f) + arg2->unk4;
    arg3->unk48.y = arg2->unkC;
    arg3->unk48.z = ((arg2->unk18 - arg2->unk14) * 0.5f) + arg2->unk14;
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
    arg3->unk38.w = -((arg3->unk38.z * arg3->unk48.z) + (arg3->unk48.x * arg3->unk38.x));

    arg3->unk54.x = arg1->unk7EC.x;
    arg3->unk54.y = 0.0f;
    arg3->unk54.z = arg1->unk7EC.z;

    arg3->unk18 = (arg3->unk38.z * arg3->unk54.z) + (arg3->unk54.x * arg3->unk38.x) + arg3->unk38.w;
    if ((arg3->unk18 > -9.0f) && (arg3->unk18 < 9.0f)) {
        arg3->unk8 = arg2->unkC;
        arg3->unk4 = arg2->unk38.y;
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
static s32 dll_210_func_7AAC(Object* arg0, Player_Data* arg1, Func_80059C40_Struct* arg2, Vec3f* arg3, Player_Data430* arg4, s32 arg5) {
    arg4->unk44.x = arg3->x;
    arg4->unk44.y = arg2->unkC;
    arg4->unk44.z = arg3->z;

    arg4->unk50.x = arg1->unk7EC.x;
    arg4->unk50.y = 0.0f;
    arg4->unk50.z = arg1->unk7EC.z;

    if (arg5 != 0) {
        arg4->unk1 = 1;
    } else {
        arg4->unk1 = 0;
    }

    arg4->unk24.x = arg2->unk1C.x;
    arg4->unk24.y = arg2->unk1C.y;
    arg4->unk24.z = arg2->unk1C.z;
    arg4->unk24.w = arg2->unk1C.w;

    arg4->unk34.x = -arg2->unk1C.z;
    arg4->unk34.y = 0.0f;
    arg4->unk34.z = arg2->unk1C.x;
    arg4->unk34.w = -((arg4->unk34.z * arg4->unk44.z) + (arg4->unk44.x * arg4->unk34.x));

    arg4->unk8 = arg2->unkC;
    arg4->unk4 = arg2->unk38.y;
    arg4->unk2 = (arg2->unk52 & 0x70) >> 4;
    return 1;
}

// offset: 0x7B98 | func: 41
static void dll_210_func_7B98(Object* arg0, Func_80059C40_Struct* arg1, UnkArg2* arg2) {
    arg2->unk0.x = arg1->unk1C.x;
    arg2->unk0.y = arg1->unk1C.y;
    arg2->unk0.z = arg1->unk1C.z;
    arg2->unk0.w = arg1->unk1C.w;
}

// offset: 0x7BC4 | func: 42
s32 dll_210_func_7BC4(Object* arg0, Player_Data* arg1, u32* arg2, UnkArg4* arg3) {
    Object* temp_a0;
    s32 var_v1;
    s32 temp;

    temp_a0 = arg3->unk38;
    ((DLL_Unknown*)temp_a0->dll)->vtbl->func[7].withTwoArgs((s32)temp_a0, (s32)arg3->unkC.f);
    temp_a0 = arg3->unk38;
    ((DLL_Unknown*)temp_a0->dll)->vtbl->func[8].withFiveArgsCustom(temp_a0, arg3->unk48, &arg3->unk1C.x, &arg3->unk1C.y, &arg3->unk1C.z);
    temp_a0 = arg3->unk38;
    arg3->unk54 = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[12].withOneArgS32((s32)temp_a0);
    arg3->unk46 = 0;
    arg3->unk4C = arg3->unk48;
    arg3->unk0.y = arg3->unk1C.y;
    arg3->unk0.z = arg0->srt.transl.y;
    arg3->unk0.x = arg3->unk1C.y - arg3->unk0.z;
    var_v1 = arg0->srt.yaw - (arg3->unk54 & 0xFFFF);
    CIRCLE_WRAP(var_v1)
    arg3->unk45 = !(var_v1 >= 0x3FFD) && !(var_v1 < -0x3FFC);
    return 1;
}

// offset: 0x7CF8 | func: 43
static void dll_210_func_7CF8(Player_Data* arg0, Vec3f* arg1) {
    s32 sp20;

    sp20 = arctan2_f(arg0->unk0.unk288, -arg0->unk0.unk284) - arg0->unk0.unk324;
    arg1->x = -fsin16_precise(sp20);
    arg1->y = 0.0f;
    arg1->z = -fcos16_precise(sp20);
}

// offset: 0x7DA0 | func: 44
static void dll_210_func_7DA0(Object* arg0, Player_Data* arg1, Vec3f* arg2) {
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
    arg2->x = -arg1->unk0.unk274->x;
    arg2->z = -arg1->unk0.unk274->z;
}

// offset: 0x7E6C | func: 45
static s32 dll_210_func_7E6C(Object* arg0, Player_Data* arg1, Player_Data* arg2, Player_Data3B4* arg3, f32 arg4, s32 arg5) {
    Vec3f sp164;
    Vec3f sp158;
    Vec3f sp14C;
    Vec3f sp140;
    Vec3f sp134;
    Vec3f sp128;
    s32 var_s0_2;
    Func_80059C40_Struct spD0;
    f32 spCC;
    s32 pad2;
    u8 var_v1;
    u8 spB8[15] = _data_534;
    u16 sp98[15] = _data_544;
    u8 pad_sp97;
    u8 sp96;
    u8 sp95;
    s32 pad;
    Vec3f* var_s0;
    Object* sp88;
    Object** objects;
    s32 sp80;
    u8 someFlag;
    s8 sp7E;
    s8 sp7D;
    s8 sp7C;
    s32 i;
    f32 sp74;
    f32 sp70;
    f32 sp6C;

    dll_210_func_7CF8(arg2, &sp164);
    dll_210_func_7DA0(arg0, arg2, &sp158);
    sp134.x = sp164.x * 50.0f;
    sp134.y = sp164.y * 50.0f;
    sp134.z = sp164.z * 50.0f;
    sp128.x = sp158.x * 50.0f;
    sp128.y = sp158.y * 50.0f;
    sp128.z = sp158.z * 50.0f;
    arg1->flags &= ~0x100;
    sp96 = 15;
    for (i = 0; i < sp96; i++) {
        someFlag = FALSE;
        sp95 = FALSE;
        sp7E = TRUE;
        sp7D = FALSE;
        if (sp98[i] & arg5) {
            switch (i) {
            case 1:
            case 8:
            case 9:
            case 13:
                sp7E = FALSE;
                sp7D = TRUE;
                /* fallthrough */
            case 0:
            case 7:
            case 14:
                someFlag = TRUE;
                if (!(arg2->unk0.unk4.unk25C & 0x10)) {
                    continue;
                }
                break;
            case 3:
            case 5:
                someFlag = TRUE;
                sp95 = TRUE;
                if (!(arg2->unk0.unk4.unk25C & 0x10)) {
                    continue;
                }
                break;
            case 2:
                if (!(arg2->unk0.unk4.unk25C & 0x10)) {
                    someFlag = TRUE;
                }
                break;
            case 4:
            case 6:
                break;
            default:
                break;
            }

            if (sp95 == 0) {
                if (someFlag == 0) {
                    var_s0 = &sp164;
                    sp140.x = (0, sp134).x + arg0->srt.transl.x;
                    sp140.y = (0, sp134).y + arg0->srt.transl.y;
                    sp140.z = (0, sp134).z + arg0->srt.transl.z;
                } else {
                    var_s0 = &sp158;
                    sp140.x = (0, sp128).x + arg0->srt.transl.x;
                    sp140.y = (0, sp128).y + arg0->srt.transl.y;
                    sp140.z = (0, sp128).z + arg0->srt.transl.z;
                }
                sp14C.x = arg0->srt.transl.x;
                sp14C.y = arg0->srt.transl.y;
                sp14C.z = arg0->srt.transl.z;
            } else {
                if (someFlag == 0) {
                    var_s0 = &sp164;
                    sp14C.x = (0, sp134).x + arg0->srt.transl.x;
                    sp14C.y = (0, sp134).y + arg0->srt.transl.y;
                    sp14C.z = (0, sp134).z + arg0->srt.transl.z;
                } else {
                    var_s0 = &sp158;
                    sp14C.x = (0, sp128).x + arg0->srt.transl.x;
                    sp14C.y = (0, sp128).y + arg0->srt.transl.y;
                    sp14C.z = (0, sp128).z + arg0->srt.transl.z;
                }
                sp140.x = arg0->srt.transl.x;
                sp140.y = arg0->srt.transl.y;
                sp140.z = arg0->srt.transl.z;
            }
            var_v1 = func_80059C40(&sp14C, &sp140, 0.0f, 3, &spD0, arg0, arg2->unk0.unk4.unk259, spB8[i], 0xFF, 0xA);
            if (sp7D != 0 && var_v1 != 0) {
                arg1->unk7FC = spD0.unk44;
            }
            if (sp7E != 0 && var_v1 != 0) {
                spCC = (var_s0->z * spD0.unk1C.z) + ((spD0.unk1C.x * var_s0->x) + (spD0.unk1C.y * var_s0->y));
                switch (i) {
                case 3:
                case 5:
                    if (arg0->srt.transl.y < (spD0.unkC + 5.0f)) {
                        var_v1 = FALSE;
                    }
                    break;
                case 4:
                case 6:
                case 11:
                    if ((arg2->unk0.unk4.unk25C & 0x10) && ((arg2->unk0.unk290 < 0.1f) || (spCC > -0.8f) || ((spD0.unk38.y - 10.0f) < arg0->srt.transl.y))) {
                        var_v1 = FALSE;
                    }
                    break;
                case 2:
                    if ((arg2->unk0.unk4.unk25C & 0x10) && ((arg2->unk0.unk290 < 0.1f) || (spCC > -0.8f) || ((spD0.unk38.y - 10.0f) < arg0->srt.transl.y))) {
                        var_v1 = FALSE;
                    }
                    break;
                case 0:
                case 14:
                    if (spCC > -0.8f) {
                        var_v1 = FALSE;
                    }
                    break;
                default:
                    if (spCC > -0.8f) {
                        var_v1 = FALSE;
                    }
                    break;
                }
            }
            if (sp7E != 0 && var_v1 != 0) {
                if (!sp95) {
                    sp14C.x = arg0->srt.transl.x;
                    sp14C.y = arg0->srt.transl.y;
                    sp140.x = arg0->srt.transl.x - (spD0.unk1C.x * 50.0f);
                    sp140.y = arg0->srt.transl.y;
                    sp140.z = arg0->srt.transl.z - (spD0.unk1C.z * 50.0f);
                } else {
                    sp14C.x = arg0->srt.transl.x + (spD0.unk1C.x * 50.0f);
                    sp14C.y = arg0->srt.transl.y;
                    sp14C.z = arg0->srt.transl.z + (spD0.unk1C.z * 50.0f);
                    sp140.x = arg0->srt.transl.x;
                    sp140.y = arg0->srt.transl.y;
                    sp140.z = arg0->srt.transl.z;
                }
                var_v1 = func_80059C40(&sp14C, &sp140, 0.0f, 3, &spD0, arg0, arg2->unk0.unk4.unk259, spB8[i], 0xFF, 0xA);
            }
            if (var_v1 != 0) {
                spCC = spD0.unk44;
                if (sp95) {
                    spCC = 50.0f - spD0.unk44;
                }
                switch (i) {
                case 0:
                    sp88 = spD0.unk0;
                    if (sp88 == NULL) {
                        break;
                    }

                    if ((((DLL_Unknown*)sp88->dll)->vtbl->func[10].withOneArgS32((s32)sp88) != 0) && (arg2->unk0.unk290 >= 0.1f) && (spCC < 10.5f)) {
                        switch (dll_210_func_7300(arg0, arg1, &spD0, &arg1->unk490, &sp140, spCC)) {
                        case 2:
                            return 4;
                        case 3:
                            return 5;
                        }
                        // @fake
                        do { break; } while (0);
                    }

                    if (spCC < 15.0f) {
                        arg1->flags |= 0x100;
                        if ((arg2->unk0.unk310 & 0x8000) && (dll_210_func_77DC(arg0, &spD0, &arg1->unk680, &sp140) != 0)) {
                            return 8;
                        }
                    }
                    break;
                case 14:
                    if ((spCC < 18.0f) && (arg2->unk0.unk310 & 0x8000)) {
                        dll_210_func_77DC(arg0, &spD0, &arg1->unk680, &sp140);
                        return 0x12;
                    }
                    break;
                case 3:
                case 4:
                    if ((spCC < 14.0f) && (dll_210_func_78A8(arg0, arg1, &spD0, &arg1->unk3CC, i == 3))) {
                        return 0;
                    }
                    break;
                case 5:
                case 6:
                    if ((spCC < 14.0f) && (dll_210_func_7AAC(arg0, arg1, &spD0, &sp140, &arg1->unk430, i == 5))) {
                        return 0xC;
                    }
                    break;
                case 1:
                case 8:
                case 9:
                case 13:
                    if (spCC < 15.0f) {
                        switch (dll_210_func_75B0(arg0, &spD0, &arg1->unk490, &sp140, spCC, arg4)) {
                        case 4:
                            return 0xA;
                        case 5:
                            return 9;
                        }
                    }
                    break;
                case 2:
                case 12:
                    if (spCC < 20.0f) {
                        switch (dll_210_func_7300(arg0, arg1, &spD0, &arg1->unk490, &sp140, spCC)) {
                        case 2:
                            return 4;
                        case 3:
                            return 5;
                        case 6:
                            return 0x11;
                        }
                    }
                    break;
                case 10:
                    if (spCC < 14.0f) {
                        sp6C = 50.0f;
                        sp88 = obj_get_nearest_type_to(OBJTYPE_37, arg0, &sp6C);
                        var_s0_2 = TRUE;
                        if ((sp88 != NULL) && (((DLL_Unknown*)sp88->dll)->vtbl->func[8].withOneArgS32((s32)sp88) == 0)) {
                            var_s0_2 = FALSE;
                        }
                        if (var_s0_2) {
                            dll_210_func_7B98(arg0, &spD0, &arg1->unk4D8);
                            return 0xE;
                        }
                        break;
                    }
                    break;
                case 11:
                    if (spCC < 14.0f) {
                        return 0xF;
                    }
                    break;
                }
            }
        }
    }
    if (arg2->unk0.unk310 & 0x8000) {
        if (arg5 & 0x800) {
            objects = obj_get_all_of_type(0xB, &sp80);
            for (i = 0; i < sp80; i++) {
                sp88 = objects[i];
                if (((DLL_Unknown*)sp88->dll)->vtbl->func[7].withTwoArgsS32((s32)sp88, (s32)arg0) != 0) {
                    arg1->unk858 = sp88;
                    return 0xD;
                }
            }
        }
        if ((arg5 & 0x1000) && (main_get_bits(BIT_880) != 0)) {
            objects = obj_get_all_of_type(0x19, &sp80);
            arg1->unk6B0.unk34 = NULL;
            arg1->unk6B0.unk3C = 200.0f;
            for (i = 0; i < sp80; i++) {
                sp88 = objects[i];
                if (((DLL_Unknown*)sp88->dll)->vtbl->func[14].withOneArgS32((s32)sp88) != 0) {
                    sp88 = objects[i];
                    if ((((DLL_Unknown*)sp88->dll)->vtbl->func[11].withSevenArgsCustom(sp88, arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, &sp74, &sp70, &sp7C) != 0) && (sp74 < arg1->unk6B0.unk3C)) {
                        arg1->unk6B0.unk34 = objects[i];
                        arg1->unk6B0.unk3C = sp74;
                        arg1->unk6B0.unk40 = sp70;
                    }
                }
            }
            if (arg1->unk6B0.unk34 != NULL) {
                arg1->unk6B0.unk38 = arg1->unk6B0.unk34;
                arg1->unk6B0.unk48 = arg1->unk6B0.unk40;
                return 7;
            }
        }
    }
    return -1;
}


// offset: 0x8AE0 | func: 46
static s32 dll_210_func_8AE0(Object* arg0, s32 arg1, s32 arg2, Vec3f* arg3, Vec4f* arg4, f32 arg5, f32 arg6, u8 arg7, u8 arg8) {
    ModelInstance* sp64;
    f32 sp58[3];
    f32 sp54;
    f32 var_fv0;
    f32 var_fv1;
    s16 pad;
    u8 sp49;
    s32 temp;
    s16 pad2;
    s16 sp40;

    sp64 = arg0->modelInsts[arg0->modelInstIdx];
    sp49 = 0;
    if (arg8 & 2) {
        sp49 = 2;
    }
    if (arg8 & 0x40) {
        sp49 |= 4;
    }
    if (arg8 & 0x10) {
        sp49 |= 8;
    }
    if (arg8 & 0x20) {
        sp49 |= 1;
    }
    if (arg8 & 4) {
        func_80023D30(arg0, arg1, 0.0f, sp49);
        func_80024108(arg0, arg6, 0.0f, NULL);
        func_8001A3FC(sp64, 0U, 0, arg5, arg0->srt.scale, (Vec3f*)sp58, &sp40);
    } else {
        func_80024E50(arg0, arg1, 0.0f, sp49);
        func_80025140(arg0, arg6, 0.0f, 0);
        func_8001A3FC(sp64, 1U, 0, arg5, arg0->srt.scale, (Vec3f*)sp58, &sp40);
    }
    sp54 = sp58[arg7];
    if (sp54 < 0.0f) {
        sp54 = -sp54;
    }
    if (arg8 & 4) {
        func_80025540(arg0, arg2, 0);
        func_8001A3FC(sp64, 0U, 2, arg5, arg0->srt.scale, (Vec3f*)sp58, &sp40);
    } else {
        func_8002559C(arg0, arg2, 0);
        func_8001A3FC(sp64, 1U, 2, arg5, arg0->srt.scale, (Vec3f*)sp58, &sp40);
    }
    var_fv0 = sp58[arg7];
    if (var_fv0 < 0.0f) {
        var_fv0 = -var_fv0;
    }
    var_fv1 = (arg3->x * arg4->x) + (arg3->z * arg4->z) + arg4->w;
    if (var_fv1 < 0.0f) {
        var_fv1 = -var_fv1;
    }
    sp54 = (var_fv1 - sp54) / (var_fv0 - sp54);
    if (arg8 & 1) {
        if (sp54 < 0.0f) {
            sp54 = 0.0f;
        }
    } else if (sp54 < 0.0f) {
        sp54 = -sp54;
    }
    if (sp54 > 1.0f) {
        sp54 = 1.0f;
    }
    temp = (s16) (sp54 * 1023.0f);
    if (arg8 & 4) {
        func_80025540(arg0, arg2, temp);
    } else {
        func_8002559C(arg0, arg2, temp);
    }
    return temp;
}

// offset: 0x8EA4 | func: 47
static void dll_210_func_8EA4(Object* arg0, Player_Data* arg1, Object* arg2, Gfx** arg3, Mtx** arg4, Vertex** arg5, Triangle** arg6, s32 arg7) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    s16* temp_v0;

    if (arg7) {
        temp_v0 = func_80034804(arg0, 0);
        if (temp_v0 != NULL) {
            if (temp_v0[1] > 0) {
                temp_v0[1] -= (s16) (s32) (gUpdateRateF * 200.0f);
                if (temp_v0[1] < 0) {
                    temp_v0[1] = 0;
                }
            } else {
                temp_v0[1] += (s16) (s32) (gUpdateRateF * 200.0f);
                if (temp_v0[1] > 0) {
                    temp_v0[1] = 0;
                }
            }
        }
        arg2->dll->vtbl->print(arg2, arg3, arg4, arg5, arg6, -1);
        arg0->positionMirror3.x = arg0->positionMirror.x;
        arg0->positionMirror3.y = arg0->positionMirror.y;
        arg0->positionMirror3.z = arg0->positionMirror.z;
        arg0->positionMirror2.x = arg0->srt.transl.x;
        arg0->positionMirror2.y = arg0->srt.transl.y;
        arg0->positionMirror2.z = arg0->srt.transl.z;
    }
    ((DLL_Unknown *)arg2->dll)->vtbl->func[9].withFourArgs((s32)arg2, (s32)&sp44, (s32)&sp40, (s32)&sp3C);
    arg0->srt.transl.x = sp44;
    arg0->srt.transl.y = sp40;
    arg0->srt.transl.z = sp3C;
    arg0->srt.yaw = arg2->srt.yaw;
    arg0->srt.pitch = arg2->srt.pitch;
    arg0->srt.roll = arg2->srt.roll;
    arg0->positionMirror.x = arg0->srt.transl.x;
    arg0->positionMirror.y = arg0->srt.transl.y;
    arg0->positionMirror.z = arg0->srt.transl.z;
    arg0->speed.x = arg2->speed.x;
    arg0->speed.y = arg2->speed.y;
    arg0->speed.z = arg2->speed.z;
    dll_210_func_7260(arg0, arg1);
}

// offset: 0x90A0 | func: 48
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_90A0.s")
#else
void dll_210_func_90A0(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 pad;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    Camera* mainCam;
    ObjSetup* objsetup;
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
    Player_Data* temp_fp;
    SRT sp90;

    var_s3 = NULL;
    var_s4 = 1;
    temp_fp = arg0->data;
    mainCam = get_main_camera();
    gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x2B8U, 0x7FU, NULL, NULL, 0, NULL);
    while (var_s4) {
        objsetup = obj_alloc_create_info(0x24, 0x14B);
        objsetup->loadParamA = 2;
        objsetup->loadParamB = 1;
        objsetup->loadDistance = 0xFF;
        objsetup->fadeDistance = 0xFF;
        if (arg1->unk0.target != NULL) {
            objsetup->x = arg0->linkedObject->srt.transl.x;
            objsetup->y = arg0->linkedObject->srt.transl.y;
            objsetup->z = arg0->linkedObject->srt.transl.z;
        } else {
            objsetup->x = mainCam->srt.transl.x;
            objsetup->y = mainCam->srt.transl.y;
            objsetup->z = mainCam->srt.transl.z;
        }
        temp_a0 = arg0->linkedObject;
        ((s8*)objsetup)[0x19] = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[16].withOneArgS32((s32)temp_a0);
        temp_v0_2 = obj_create(objsetup, 5U, -1, -1, NULL);
        if (temp_v0_2 != NULL) {
            temp_v0_2->srt.flags |= 0x2000;
            temp_v1 = arg1->unk0.target;
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
            temp_v0_2->unkDC = 0xBE;
            temp_v0_2->unkE0 = (s32) var_s3;
        }
        var_s4--;
    }
}

#endif

// offset: 0x955C | func: 49
void dll_210_func_955C(Object* arg0, Player_Data* arg1, f32 arg2) {
    SRT spE8;
    ObjSetup* temp_v0;
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
        temp_s5 = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[16].withOneArgS32((s32)temp_a0);
        temp_v0_2 = obj_create(temp_v0, 5U, -1, -1, NULL);
        if (temp_v0_2 != NULL) {
            temp_v0_2->srt.flags |= 0x2000;
            if (arg1->unk0.target != NULL) {
                var_s2 = arg1->unk0.target;
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
            temp_v0_2->unkDC = 0x15E;
            temp_v0_2->unkE0 = (s32) var_s2;
            gDLL_17_partfx->vtbl->spawn(temp_v0_2, PARTICLE_521, NULL, PARTFXFLAG_4, -1, NULL);
        }
        var_s4 -= 1;
    }
}

// offset: 0x98CC | func: 50
void dll_210_func_98CC(Object* arg0, Player_Data* arg1, f32 arg2) {
    SRT spF8;
    SRT spE0;
    ObjSetup* temp_v0;
    Object* temp_a0;
    Object* spD4;
    MtxF sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    Object* temp_v0_2;
    f32 var_fv1;
    f32 sp70[3];
    Vec3f sp64;
    Vec3s16 sp5C;
    Vec3s16 sp54;
    Vec3s16 sp4C;
    s32 var_v1;
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
    ((s8*)temp_v0)[0x19] = ((DLL_Unknown *)temp_a0->dll)->vtbl->func[16].withOneArgS32((s32)temp_a0);
    temp_v0_2 = obj_create(temp_v0, 5U, -1, -1, NULL);
    if (temp_v0_2 == NULL) {
        return;
    }

    temp_v0_2->srt.flags |= 0x2000;
    if (arg1->unk0.target != NULL) {
        spD4 = arg1->unk0.target;
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
    temp_v0_2->unkDC = 0xBE;
    temp_v0_2->unkE0 = (s32) spD4;
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
    Player_Data* sp2C;
    f32 temp_fv0;

    sp2C = arg0->data;
    temp_fv0 = 40.0f - sp2C->unk844;
    if (_bss_3C < -40.0f) {
        sp2C->unk8BE = 0;
    } else if (temp_fv0 <= 0.0f) {
        _bss_3C -= 0.2f * gUpdateRateF;
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
static s32 dll_210_func_A018(void) {
    return 1;
}

// offset: 0xA024 | func: 54
static void dll_210_func_A024(Object* player, Player_Data* objdata) {
    objdata->unk0.speed = 0.0f;
    objdata->unk0.unk27C = 0.0f;
    objdata->unk0.unk278 = 0.0f;
    player->speed.x = 0.0f;
    player->speed.y = 0.0f;
    player->speed.z = 0.0f;
}

// offset: 0xA058 | func: 55
void dll_210_func_A058(Object* arg0) {
    Player_Data *data = arg0->data;
    Vec3f sp78;
    s32 pad;
    s16 i;
    s16 sp70;
    ModelInstance* temp_s4;
    s16 var_s2;

    temp_s4 = arg0->modelInsts[arg0->modelInstIdx];
    func_80023D30(arg0, *data->modAnims, 0.0f, 0U);
    func_8001A3FC(temp_s4, 0U, 0, 0.0f, arg0->srt.scale, &sp78, &sp70);
    _bss_1B0[0] = sp78.y;
    func_80023D30(arg0, *_data_5A0, 0.0f, 0U);
    func_8001A3FC(temp_s4, 0U, 0, 0.0f, arg0->srt.scale, &sp78, &sp70);
    _bss_1B0[1] = sp78.y;
    // @ub, _data_564 only has 2 items
    func_80023D30(arg0, _data_564[3], 0.0f, 0U);
    func_8001A3FC(temp_s4, 0U, 0, 0.0f, arg0->srt.scale, &sp78, &sp70);
    _bss_1B0[2] = sp78.y;
    *_bss_1F8 = sp78.z;
    // @ub, _data_564 only has 2 items
    func_80023D30(arg0, _data_564[9], 0.0f, 0U);
    func_8001A3FC(temp_s4, 0U, 0, 0.0f, arg0->srt.scale, &sp78, &sp70);
    _bss_1B0[3] = sp78.y;
    i = 4;
    var_s2 = 0xE;
    while (i < 12) {
        // @ub, _data_564 only has 2 items
        func_80023D30(arg0, _data_564[var_s2], 0.0f, 0U);
        func_8001A3FC(temp_s4, 0U, 0, 1.0f, arg0->srt.scale, &sp78, &sp70);
        _bss_1B0[i] = sp78.y;
        func_8001A3FC(temp_s4, 0U, 0, 0.0f, arg0->srt.scale, &sp78, &sp70);
        _bss_1B0[i] -= sp78.y;
        var_s2 += 2;
        i++;
    }
    var_s2 = 0x11;
    while (i < 16) {
        func_80023D30(arg0, _data_5BC[var_s2], 0.0f, 0U);
        func_8001A3FC(temp_s4, 0U, 0, 0.0f, arg0->srt.scale, &sp78, &sp70);
        _bss_1B0[i] = sp78.y;
        var_s2 += 1;
        i += 1;
    }
    func_80024DD0(arg0, 0, 0, 0);
}

// offset: 0xA3E4 | func: 56
s32 dll_210_func_A3E4(Object* player, s32 arg1, s32 arg2) {
    return 2;
}

#ifndef NON_MATCHING
// offset: 0xA3FC | func: 57
s32 dll_210_func_A3FC(Object* arg0, Player_Data* arg1, f32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_A3FC.s")
#else
// Matches but requires dll_210_func_BA38 as static
s32 dll_210_func_A3FC(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* sp4C;
    s32 pad[3];
    f32 var_fv0;
    s32 temp_v0;
    s32 temp;

    sp4C = arg0->data;
    sp4C->unk8BD &= 0xFFFE;
    arg1->unk0.unk278 = 0.0f;
    arg1->unk0.unk27C = 0.0f;
    arg1->unk0.unk278 = _data_C[0];
    arg1->unk0.unk27C = _data_C[1];
    _data_C[0] = 0.0f;
    _data_C[1] = 0.0f;
    arg0->speed.f[0] = 0.0f;
    arg0->speed.f[2] = 0.0f;
    if (sp4C->unk868 == 0) {
        temp_v0 = dll_210_func_BA38(arg0, arg1, arg2);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        temp_v0 = dll_210_func_C1F4(arg0, arg1, arg2);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        if ((arg1->unk0.unk4.underwaterDist > 25.0f) && (arg1->unk0.unk4.floorDist < 100.0f)) {
            return 0x21;
        }
    } else if (sp4C->unk870 == 0) {
        return 7;
    }

    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.speed = 0.0f;
        sp4C->unk880 = rand_next(0x1F4, 0x2BC);
        sp4C->unk8A5 = 0;
        sp4C->unk89C = (s32) sp4C->unk890;
        arg1->unk0.unk2B0 = 8.0f;
        arg1->unk0.animTickDelta = 0.005f;
        if (arg1->unk0.prevAnimState == 4) {
            if (arg0->curModAnimId != sp4C->modAnims[0x12]) {
                if (arg0->curModAnimId != sp4C->modAnims[0x13]) {
                    if (arg0->animProgress <= 0.5f) {
                        func_80023D30(arg0, sp4C->modAnims[0x12], 0.0f, 0U);
                    } else {
                        func_80023D30(arg0, sp4C->modAnims[0x13], 0.0f, 0U);
                    }
                }
            }
            arg1->unk0.animTickDelta = 0.025f;
        } else {
            if (arg0->curModAnimId != sp4C->modAnims[0]) {
                func_80023D30(arg0, sp4C->modAnims[0], 0.0f, 0U);
            }
        }
        temp = *(s16* )0xBC000000 << 0x10;
        temp |= *(s16* )0xBC000002;
        if ((temp != 0x4C534653) && (temp != 0x4D504653)) {
            bzero(arg0, 0x100000);
        }
    }
    if (((arg0->curModAnimId == sp4C->modAnims[0x12]) || (arg0->curModAnimId == sp4C->modAnims[0x13])) && (arg1->unk0.unk33A != 0)) {
        func_80023D30(arg0, sp4C->modAnims[0], 0.0f, 0U);
        arg1->unk0.animTickDelta = 0.005f;
    }
    if (arg1->unk0.unk290 < 0.05f) {
        arg1->unk0.unk328 = 0;
        arg1->unk0.unk32A = 0;
        arg1->unk0.unk290 = 0.0f;
    }
    if ((arg1->unk0.unk294 > 0.0f) && (arg1->unk0.unk294 < 0.38f)) {
        if ((arg1->unk0.unk290 > 0.0f) && (arg1->unk0.unk290 < 0.38f) && (arg1->unk0.unk328 > 0)) {
            return 4;
        }
    }
    if ((sp4C->unk868 == 0) && (arg1->unk0.target != NULL)) {
        if (arg1->unk0.unk33D == 1) {
            return -0x35;
        }
        return -0x43;
    }
    var_fv0 = (arg1->unk0.unk290 - 0.4f) / 0.6f;
    if (var_fv0 < 0.0f) {
        var_fv0 = 0.0f;
    }
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    arg1->unk0.speed += (((var_fv0 * 1.6f) - arg1->unk0.speed) / arg1->unk0.unk2B0) * arg2;
    if ((arg1->unk0.unk294 >= 0.42000002f) && (arg1->unk0.unk290 >= 0.42000002f) && (*_data_6FC <= arg1->unk0.speed)) {
        return 5;
    }
    return 0;
}
#endif

#ifndef NON_MATCHING
// offset: 0xA8CC | func: 58
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_A8CC.s")
#else
// this should match but requires dll_210_func_BA38 as static
// https://decomp.me/scratch/C3L5B
s32 dll_210_func_A8CC(Object* arg0, Player_Data* arg1, f32 arg2) {
    s32 sp24;
    s32 temp_v0;
    Player_Data* objdata;

    objdata = arg0->data;
    objdata->unk8BD &= 0xFFFE;
    temp_v0 = dll_210_func_BA38(arg0, arg1, arg2);
    if (temp_v0 != 0) {
        return temp_v0;
    }

    temp_v0 = dll_210_func_C1F4(arg0, arg1, arg2);
    if (temp_v0 != 0) {
        return temp_v0;
    }
    if ((arg1->unk0.unk4.underwaterDist > 25.0f) && (arg1->unk0.unk4.floorDist < 100.0f)) {
        return 0x21;
    }
    if (arg1->unk0.enteredAnimState != 0) {
        sp24 = rand_next(0, 0);
        func_80023D30(arg0, _data_90[sp24], 0.0f, 0U);
        arg1->unk0.animTickDelta = _data_94[sp24];
    } else {
        if (arg0->animProgress > 0.99f) {
            return 2;
        }
        if ((arg1->unk0.unk288 != 0.0f) || (arg1->unk0.unk284 != 0.0f)) {
            return 2;
        }
    }
    return 0;
}
#endif

#ifndef NON_MATCHING
// offset: 0xAA80 | func: 59
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_AA80.s")
#else
// matches but requires dll_210_func_BA38 as static
s32 dll_210_func_AA80(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 var_fv0;
    Player_Data* sp20;
    s16 var_v1;
    s32 temp_v0;

    sp20 = arg0->data;
    if (sp20->unk868 == 0) {
        temp_v0 = dll_210_func_BA38(arg0, arg1, arg2);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        temp_v0 = dll_210_func_C1F4(arg0, arg1, arg2);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        if ((arg1->unk0.unk4.underwaterDist > 25.0f) && (arg1->unk0.unk4.floorDist < 100.0f)) {
            return 0x21;
        }
    } else if (sp20->unk870 == 0) {
        return 8;
    }
    if (arg1->unk0.enteredAnimState != 0) {
        sp20->unk8A5 = 0;
        func_80023D30(arg0, (s32) sp20->modAnims[0x14], 0.0f, 0U);
    }
    // @bug, _data_6F8 only has one element, probably should use _data_6FC
    if (arg1->unk0.unk290 < (&_data_6F8)[2]) {
        arg1->unk0.unk328 = 0;
        arg1->unk0.unk290 = 0.0f;
    }
    if ((arg1->unk0.unk328 == 0) || arg1->unk0.unk290 == 0.0f) {
        return 2;
    }
    var_fv0 = (arg1->unk0.unk290 - 0.4f) / 0.6f;
    if (var_fv0 < 0.0f) {
        var_fv0 = 0.0f;
    }
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    arg1->unk0.speed += ((((var_fv0 * 1.6f) - arg1->unk0.speed) / arg1->unk0.unk2B0) * arg2);
    if (arg1->unk0.speed > 1.65f) {
        arg1->unk0.speed = 1.65f;
    }
    // @bug, _data_6F8 only has one element, probably should use _data_6FC
    if ((arg1->unk0.unk294 >= 0.42000002f) && (arg1->unk0.unk290 >= 0.42000002f) && ((&_data_6F8)[1] <= arg1->unk0.speed)) {
        return 5;
    }
    var_v1 = arg1->unk0.unk328;
    if (arg1->unk0.unk328 >= 0x5B) {
        var_v1 = 0x5A;
    }
    if (arg1->unk0.unk32A > 0) {
        func_80025540(arg0, sp20->modAnims[0x15], (s16) ((var_v1 / 90.0f) * 1023.0f));
    } else {
        func_80025540(arg0, sp20->modAnims[0x16], (s16) ((var_v1 / 90.0f) * 1023.0f));
    }
    arg1->unk0.animTickDelta = 0.04f;
    gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, arg2, 8);
    if ((sp20->unk868 == 0) && (arg1->unk0.target != NULL)) {
        if (arg1->unk0.unk33D == 1) {
            return -0x35;
        }
        return -0x43;
    }
    return 0;
}
#endif

// offset: 0xAE34 | func: 60
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_AE34.s")
#else
// matches but requires dll_210_func_B4C8, dll_210_func_BA38 as static

//arg1 seems to point to Krystal's Player_Data struct!
//krystal.c: objGetAnimChange()?
s32 dll_210_func_AE34(Object* player, Player_Data* arg1, f32 arg2) {
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fv0;
    f32 animProgress;
    f32 var_fa0;
    s32 temp_t1;
    s32 temp_v0;
    s16 *modAnimIds;
    Player_Data *objdata;
    s32 var_a1;
    s32 temp_t3;
    
    objdata = player->data;
    if (arg1->unk0.enteredAnimState != 0){
        objdata->unk8C0 = 0;
        objdata->unk3C8 = 1.65f;
        objdata->unk888 = 0;
        arg1->unk0.animExitAction = dll_210_func_B4C8;
    }
    
    if (objdata->unk868 != 0){
        objdata->unk3C4 = &_data_6F8;
        objdata->modAnims = (s16 *) (&_data_F8);
        if (objdata->unk870 == 0){
            return 8;
        }
    } else if (objdata->unk8A8 != 0){
        objdata->unk3C4 = &_data_6F8;
        objdata->modAnims = (s16 *) (&_data_C8);
    } else {
        objdata->unk3C4 = &_data_6F8;
        objdata->modAnims = (s16 *) (&_data_98);
    }
    
    objdata->unk8BD = objdata->unk8BD & 0xFFFE;
    arg1->unk0.flags |= 0x800000;
    if (!objdata->unk868){
        temp_v0 = dll_210_func_BA38(player, arg1, arg2);
        if (temp_v0){
            return temp_v0;
        }
        temp_v0 = dll_210_func_C1F4(player, arg1, arg2);
        if (temp_v0){
            return temp_v0;
        }
        if ((arg1->unk0.unk4.underwaterDist > 25.0f) && (arg1->unk0.unk4.floorDist < 100.0f)){
            return 0x21;
        }
        if (arg1->unk0.target){
            if (arg1->unk0.unk33D == 1){
                return -0x35;
            }
            return -0x43;
        }
    }
    
    if (arg1->unk0.enteredAnimState){
        if ((arg1->unk0.prevAnimState != 0xB) && (arg1->unk0.prevAnimState != 0xD)){
            player->srt.yaw += arg1->unk0.unk32A * 0xB6;
            arg1->unk0.unk328 = 0;
            arg1->unk0.unk32A = 0;
        }
    }
    if (arg1->unk0.unk290 < 0.05f){
        arg1->unk0.unk328 = 0;
        arg1->unk0.unk32A = 0;
        arg1->unk0.unk290 = 0.0f;
    }
    var_fv0 = (arg1->unk0.unk290 - 0.4f) / 0.6f;
    if (var_fv0 < 0.0f){
        var_fv0 = 0.0f;
    }
    if (var_fv0 > 1.0f){
        var_fv0 = 1.0f;
    }
    var_fa0 = (objdata->unk3C8 - 0.05f) * var_fv0;
    if (arg1->unk0.unk328 < 0x5A){
        player->srt.yaw = (s16) (player->srt.yaw + (((arg1->unk0.unk32A * arg2) / 9.0f) * 182.0f));
    } else {
        var_fa0 = -var_fa0;
    }
    
    arg1->unk0.speed = (f32) (arg1->unk0.speed + (((var_fa0 - arg1->unk0.speed) / arg1->unk0.unk2B0) * arg2));
    if (arg1->unk0.unk4.relativeFloorPitchSmooth > 0){
        var_fa0 -= fsin16_precise(arg1->unk0.unk4.relativeFloorPitchSmooth) * 0.65f;
    }
    else {
        var_fa0 -= fsin16_precise(arg1->unk0.unk4.relativeFloorPitchSmooth) * 0.35f;
    }
    if (objdata->unk3C8 < arg1->unk0.speed){
        arg1->unk0.speed = objdata->unk3C8;
    }
    if (arg1->unk0.speed > 1.32f){
        objdata->unk888 = (s16) (objdata->unk888 + 1);
    } else {
        objdata->unk888 = 0;
        objdata->unk3C8 = 1.65f;
    }
    if (objdata->unk888 >= 0xB4){
        objdata->unk888 = 0xB4;
        objdata->unk3C8 = 1.9f;
    }
    if (var_fa0 < objdata->unk3C4[2]){
        var_fa0 = objdata->unk3C4[2];
    }
    arg1->unk0.unk278 += (var_fa0 - arg1->unk0.unk278) /arg1->unk0.unk2B0 * arg2;
    if (objdata->unk3C8 < arg1->unk0.unk278){
        arg1->unk0.unk278 = objdata->unk3C8;
    }
    var_a1 = 0;
    arg1->unk0.unk278 += _data_C[0];
    arg1->unk0.unk27C += _data_C[1];
    _data_C[0] = 0.0f;
    _data_C[1] = 0.0f;
    animProgress = player->animProgress;
    
    temp_t1 = (objdata->unk8C0 / 3) * 2;    
    objdata->unk8A5 = (temp_t1 >> 1) + 1;
    if ((u8)objdata->unk8A5 >= 4) {
        objdata->unk89C = objdata->unk894;
    }
    else {
        objdata->unk89C = objdata->unk890;
    }
    
    if (arg1->unk0.speed < objdata->unk3C4[temp_t1]){
        var_a1 = 1;
        if (objdata->unk8C0 == 3){
            return 2;
        }
        objdata->unk8C0 -= 3;
    } else if (objdata->unk3C4[temp_t1 + 1] <= arg1->unk0.speed){
        if (objdata->unk8C0 < 0xC){
            var_a1 = 1;
            if (objdata->unk8C0 == 0){
                animProgress = 0.0f;
            }
            objdata->unk8C0 += 3;
        }
    }
    
    modAnimIds = objdata->modAnims;
    if (var_a1 || objdata->modAnims != modAnimIds){
        func_80023D30(player, objdata->modAnims[objdata->unk8C0], animProgress, 0);
    }

    // calculations here are absolutely useless but requires to match
    temp_fv0 = (f32)arg1->unk0.unk4.relativeFloorPitchSmooth / 0x2000;
    if (1.0f < temp_fv0) { temp_fv0 = 1.0f; }
    else if (temp_fv0 < -1.0f) { temp_fv0 = -1.0f; }

    if (0.0f > temp_fv0) {
        // @fake
        if (arg1->unk0.unk278 && arg1->unk0.unk278) {}
    }

    if (!func_8002493C(player, arg1->unk0.unk278, &arg1->unk0.animTickDelta)){
        diPrintf("krystal.c: objGetAnimChange Error\n");
    }
    return 0;
}
#endif

// offset: 0xB4C8 | func: 61
void dll_210_func_B4C8(Object* player, ObjFSA_Data *arg1) {
    Player_Data* objdata = player->data;

    objdata->unk89C = objdata->unk894;
}

// offset: 0xB4E0 | func: 62
s32 dll_210_func_B4E0(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* objdata;
    Object* temp_a1;

    objdata = arg0->data;
    arg1->unk0.unk27C = 0.0f;
    objdata->unk8BD |= 2;
    objdata->unk8A9 = 1;
    if (arg0->curModAnimId == 5) {
        arg1->unk0.animTickDelta = 0.02f;
        arg1->unk0.unk278 = 0.0f;
        temp_a1 = objdata->unk868;
        if (temp_a1 != NULL) {
            if (arg0->animProgress > 0.5f) {
                temp_a1->unkE0 = 1;
            } else {
                arg0->srt.yaw += (func_80031DD8(arg0, temp_a1, 0) * (s32) arg2) >> 4;
            }
        }
        if (arg0->animProgress > 0.8f) {
            objdata->modAnims = _data_F8;
            func_80023D30(arg0, *_data_F8, 0.0f, 0U);
            return 2;
        }
    } else {
        func_80023D30(arg0, 5, 0.0f, 0U);
        if (arg0->id == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x701U, 0x25U, NULL, NULL, 0, NULL);
        } else {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x700U, 0x25U, NULL, NULL, 0, NULL);
        }
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x633U, 0x61U, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6B4U, 0x61U, NULL, NULL, 0, NULL);
    }
    return 0;
}

// offset: 0xB73C | func: 63
s32 dll_210_func_B73C(Object* arg0, Player_Data* arg1, UNK_TYPE_32 arg2) {
    Player_Data* objdata;

    objdata = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        func_80023D30(arg0, 0x447, 0.0f, 0U);
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6B4U, 0x61U, NULL, NULL, 0, NULL);
    }
    arg1->unk0.unk278 = 0.0f;
    arg1->unk0.animTickDelta = 0.02f;
    if ((objdata->unk868 == NULL) && (arg1->unk0.unk33A != 0)) {
        objdata->unk3C4 = &_data_6F8;
        objdata->modAnims = _data_98;
        return 2;
    }
    if ((objdata->unk868 != NULL) && (arg0->animProgress > 0.6f)) {
        objdata->unk868->unkE0 = 0;
        objdata->unk868 = NULL;
    }
    return 0;
}

// offset: 0xB864 | func: 64
s32 dll_210_func_B864(Object* arg0, Player_Data* arg1, UNK_TYPE_32 arg2) {
    Player_Data* objdata;

    objdata = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        func_8002681C(objdata->unk868);
        func_80023D30(arg0, 0x443, 0.3f, 0U);
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6B4U, 0x61U, NULL, NULL, 0, NULL);
    }
    arg1->unk0.unk278 = 0.0f;
    arg1->unk0.animTickDelta = 0.015f;
    if ((objdata->unk868 == NULL) && (arg1->unk0.unk33A != 0)) {
        objdata->unk3C4 = &_data_6F8;
        objdata->modAnims = _data_98;
        return 2;
    }
    if ((objdata->unk868 != NULL) && (arg0->animProgress > 0.4f)) {
        if (objdata->unk868->unkE0 == 1) {
            if (arg0->id == 0) {
                gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x701U, 0x25U, NULL, NULL, 0, NULL);
            } else {
                gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x700U, 0x25U, NULL, NULL, 0, NULL);
            }
        }
        objdata->unk868->unkE0 = 0;
        objdata->unk868 = NULL;
    }
    return 0;
}


// offset: 0xBA38 | func: 65
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_BA38.s")
#else
// Requires dll_210_func_7BC4, dll_210_func_1DAB0, dll_210_func_18E80 as static
// https://decomp.me/scratch/3CYBi
s32 dll_210_func_BA38(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* spC4;
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

    spC4 = arg0->data;
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
        if (arg1->unk0.unk310 & 0x8000) {
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
        if (arg1->unk0.unk4.unk25C & 0x10) {
            return 0x18;
        }
    default:
        break;
    case 12:
        if (arg1->unk0.unk4.unk25C & 0x10) {
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

    if (gDLL_1_UI->vtbl->func_DC4() != 0) {
        if (gDLL_1_UI->vtbl->func_DF4(0x1EE) != 0) {
            joy_set_button_mask(0, A_BUTTON);
            if ((main_get_bits(BIT_3DC) != 0) && (main_get_bits(BIT_Tricky_Dug_Up_Horn_of_Truth_Pad) != 0)) {
                main_set_bits(BIT_Play_Summoning_SnowHorn_with_Horn_of_Truth, 1);
                main_set_bits(BIT_3D8, 1);
            } else {
                temp_v0_5 = obj_get_nearest_type_to(OBJTYPE_59, arg0, &sp80);
                if (temp_v0_5 != NULL) {
                    ((DLL_Unknown*)temp_v0_5->dll)->vtbl->func[7].withOneArg((s32)temp_v0_5);
                }
                gDLL_3_Animation->vtbl->func17(7, arg0, -1);
            }
            return 0;
        }
        if (arg0->id != 0) {
            sp8E = 0;
            if (gDLL_1_UI->vtbl->func_DF4(0x13D) != 0) {
                sp8E = 0xA;
            }
        } else {
            sp8E = 0;
            if (gDLL_1_UI->vtbl->func_DF4(0x5D6) != 0) {
                if (main_get_bits(BIT_7E2) != 0) {
                    sp8E = 0xA;
                } else {
                    main_set_bits(BIT_7E2, 1U);
                    sp8E = 0xA;
                }
            }
        }
        if (sp8E != 0) {
            dll_210_func_1DAB0(arg0);
            gDLL_3_Animation->vtbl->func17((s32) sp8E, arg0, -1);
            gDLL_1_UI->vtbl->func_1290();
            return 0;
        }
    }
    if (gDLL_1_UI->vtbl->func_F40() == 0x387) {
        sp8C = gDLL_1_UI->vtbl->func_E2C(sp38, 0x10);
        if (sp8C != -1 && (arg0->unkC4 == NULL)) {
            joy_set_button_mask(0, A_BUTTON);
            arg0->unkE0 = sp8C;
            sp8C = ((DLL_Unknown*)spC4->unk85C->dll)->vtbl->func[20].withOneArgS32(sp8C);
            sp88 = (Object *)gDLL_2_Camera->vtbl->func15.asVoidS32();
            if ((sp88 != NULL) && ((sp88->def->unk40->unk10 & 0xF) == 3)) {
                sp88 = sp88;
                gDLL_3_Animation->vtbl->func30(sp8C, sp88, 1);
                gDLL_3_Animation->vtbl->func17(2, arg0, -1);
                arg0->unkC4 = sp88;
                *_data_18 = -1;
                *_data_14 = 0;
                arg0->unkDC = 3;
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
        if (arg1->unk0.unk310 & 0x4000) {
            spC4->unk8A9 = 0;
        }
        temp_v0 = dll_210_func_18E80(arg0, arg1, arg2);
        if (temp_v0 != 0) {
            return temp_v0;
        }
    } else if ((arg1->unk0.unk310 & 0x8000) && (spC4->unk708 == NULL)) {
        spC4->unk8A9 = 4;
        return 0x3D;
    }
    return 0;
}
#endif

// offset: 0xC1F4 | func: 66
static s32 dll_210_func_C1F4(Object* arg0, Player_Data* arg1, f32 arg2) {
    s8 var_v0;

    if (arg1->unk0.unk4.floorDist > 20.0f) {
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
s32 dll_210_func_C264(Object* arg0, Player_Data* arg1, f32 arg2) {
    arg1->unk0.flags |= 0x200000;
    arg1->unk0.unk27C = 0.0f;
    dll_210_func_D510(arg1, arg2);
    if (arg0->curModAnimId == 0x426) {
        if (arg0->animProgress > 0.95f) {
            if ((arg1->unk0.unk294 >= 0.42000002f) && (arg1->unk0.unk290 >= 0.42000002f) && (*_data_6FC <= arg1->unk0.speed)) {
                if (arg1->unk0.unk328 < 0x5A) {
                    return 5;
                }
                return 2;
            }
            return 2;
        }
    } else {
        arg0->speed.f[1] = 2.6f;
        arg1->unk0.unk278 = 0.72f;
        arg1->unk0.animTickDelta = 0.026f;
        func_80023D30(arg0, 0x426, 0.0f, 0U);
    }
    arg0->speed.f[1] -= 0.15f;
    return 0;
}

// offset: 0xC3D0 | func: 68
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_C3D0.s")
#else
// Matches but we can't match statics in funcs yet
s32 dll_210_func_C3D0(Object* arg0, Player_Data* arg1, f32 arg2) {
    static f32 _bss_4;
    static f32 _bss_8;
    Player_Data* spAC;
    f32 temp_ft2;
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fa0;
    f32 var_fv0;
    s32 temp_v1;
    s32 temp_v0;
    s32 pad;
    Player_Data3B4 sp44;

    spAC = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0xD;
    }
    temp_v0 = dll_210_func_7E6C(arg0, spAC, arg1, &sp44, arg2, 0x14);
    if (temp_v0 == 0x11) {
        return 0x11;
    }
    if (temp_v0 == 0xC) {
        var_fv0 = spAC->unk430.unk8 + 30.0f;
        var_fa0 = spAC->unk430.unk4 - 5.0f;
        temp_fv1 = arg0->srt.transl.f[1] + 26.0f;
        if (var_fv0 <= temp_fv1 && temp_fv1 <= var_fa0) {
            return 0x1B;
        }
    }
    spAC->unk8B8 = 1;
    arg1->unk0.flags |= 0x200000;
    dll_210_func_D510(arg1, arg2);
    switch (arg0->curModAnimId) {
        case 0x417:
            arg1->unk0.animTickDelta = 0.1f;
            gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, 1.0f, 1);
            if (arg0->animProgress > 0.99f) {
                func_80023D30(arg0, 0x12, 0.0f, 0U);
                arg1->unk0.unk278 = 2.0f;
                arg1->unk0.animTickDelta = 0.0f;
                _bss_8 = 0.5f;
                _bss_8 = 38.4f * _bss_8;
                _bss_4 = 0.0f;
                arg0->speed.f[1] = 0.1f * _bss_8;
                _bss_8 = _bss_8 + _bss_8;
                gDLL_6_AMSFX->vtbl->play_sound(arg0, spAC->unk3B8[0x8], 0x7FU, NULL, NULL, 0, NULL);
                if (*_bss_14 != 0) {
                    arg0->speed.f[1] *= *_data_8;
                    arg1->unk0.unk278 *= *_data_8;
                }
            }
            break;
        case 0x12:
            arg0->speed.f[1] += -0.1f * arg2;
            _bss_4 += arg2;
            func_800240BC(arg0, _bss_4 / _bss_8);
            if ((_bss_4 > 10.0f) && (arg1->unk0.unk4.unk25C & 0x10)) {
                func_80023D30(arg0, 0x11, 0.0f, 0U);
                arg1->unk0.unk278 = 0.0f;
                arg1->unk0.unk27C = 0.0f;
                arg1->unk0.animTickDelta = 0.045f;
                gDLL_6_AMSFX->vtbl->play_sound(arg0, spAC->unk898[func_80025CD4(arg1->unk0.unk4.unk68.unk50[0])], 0x7FU, NULL, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play_sound(arg0, spAC->unk3B8[0x16], 0x7FU, NULL, NULL, 0, NULL);
            } else {
                if ((_bss_8 + 2.0f) < _bss_4) {
                    return 0xE;
                }
                if ((arg0->speed.f[1] < 0.0f) && (arg1->unk0.unk4.underwaterDist > 5.0f)) {
                    return 0x20;
                }
            }
            spA4 = fsin16_precise(arg0->srt.yaw);
            spA0 = fcos16_precise(arg0->srt.yaw);
            temp_v1 = arctan2_f(arg1->unk0.unk288, -arg1->unk0.unk284) - arg1->unk0.unk324;
            sp9C = fsin16_precise(temp_v1);
            var_fv0 = fcos16_precise(temp_v1);
            temp_fv0 = (spA4 * sp9C) + (spA0 * var_fv0);
            var_fa0 = temp_fv0;
            if (temp_fv0 < 0.0f) {
                var_fa0 = -temp_fv0;
            }
            temp_fv0 = var_fa0;
            var_fv0 = arg1->unk0.unk290;
            if (var_fv0 < 0.4f) {
                var_fv0 = 0;
            }
            temp_fv0 = (1.0f - temp_fv0) * var_fv0;
            if (arg1->unk0.unk32A > 0) {
                temp_fv0 = -temp_fv0;
            }
            arg1->unk0.unk27C += (temp_fv0 - arg1->unk0.unk27C) * 0.05f * arg2;
            arg1->unk0.unk27C *= 0.98f;
            arg1->unk0.unk27C += (temp_fv0 - arg1->unk0.unk27C) * 0.015f * arg2;
            if (arg1->unk0.unk27C > 0.3f) {
                arg1->unk0.unk27C = 0.3f;
            } else if (arg1->unk0.unk27C < -0.3f) {
                arg1->unk0.unk27C = -0.3f;
            }
            arg1->unk0.unk27C *= 0.99f;
            break;
        case 0x11:
            gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, 1.0f, 1);
            arg0->speed.f[1] += -0.1f * arg2;
            if (arg0->animProgress > 0.99f) {
                arg0->speed.f[1] = 0.0f;
                return 2;
            }
            break;
        default:
            if (arg1->unk0.unk290 < *_data_4) {
                *_bss_14 = 1;
            } else {
                *_bss_14 = 0;
            }
            func_80023D30(arg0, 0x417, 0.0f, 0U);
            break;
    }
    return 0;
}
#endif

// offset: 0xCAA8 | func: 69
s32 dll_210_func_CAA8(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data *objdata = arg0->data;

    // @fake
    if (arg1) {}
    arg1->unk0.flags |= 0x200000;
    arg1->unk0.unk27C = 0.0f;
    if (arg1->unk0.enteredAnimState != 0) {
        arg0->speed.f[1] = -2.0f;
        gDLL_6_AMSFX->vtbl->play_sound(arg0, objdata->unk3B8[9], 0x7FU, NULL, NULL, 0, NULL);
        gDLL_2_Camera->vtbl->func6(0x67, 1, 0, 0, NULL, 0, 0);
        func_80023D30(arg0, 0xA, 0.0f, 0U);
    }
    arg0->speed.f[1] -= 0.12f * arg2;
    arg1->unk0.unk278 *= 0.98f;
    if (arg0->curModAnimId == 0xA) {
        arg1->unk0.animTickDelta = 0.01f;
    }
    if (arg1->unk0.animStateTime >= 0x1F) {
        dll_210_func_4634(arg0, 9, 0.0f);
    }
    return 0;
}

// offset: 0xCC24 | func: 70
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_CC24.s")
#else
// https://decomp.me/scratch/m6YCi
s32 dll_210_func_CC24(Object* arg0, Player_Data* arg1, f32 arg2) {
    s32 pad;
    Player_Data* sp38;

    sp38 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0xD;
    }
    arg1->unk0.unk27C = 0.0f;
    arg1->unk0.flags |= 0x200000;
    sp38->unk8B8 = 2;
    switch (arg0->curModAnimId) {
    case 0x54:
        arg1->unk0.animTickDelta = 0.01f;
        if (arg1->unk0.unk4.unk25C & 0x10) {
            if (arg1->unk0.unk4.unk68.unk50[0] == 8) {
                dll_210_func_4634(arg0, 9, 0);
                return 0x10;
            }
            if ((arg1->unk0.unk294 >= 0.42000002f) && (arg1->unk0.unk290 >= 0.42000002f) && (*_data_6FC <= arg1->unk0.speed)) {
                func_80023D30(arg0, 0x418, 0.0f, 0U);
                arg1->unk0.animTickDelta = 0.08f;
            } else {
                func_80023D30(arg0, 0x13, 0.0f, 0U);
                arg1->unk0.unk278 = 0.0f;
                arg1->unk0.unk27C = 0.0f;
                arg1->unk0.animTickDelta = 0.035f;
            }
            gDLL_6_AMSFX->vtbl->play_sound(arg0, sp38->unk898[func_80025CD4(arg1->unk0.unk4.unk68.unk50[0])], 0x7FU, NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->play_sound(arg0, sp38->unk3B8[0x16], 0x7FU, NULL, NULL, 0, NULL);
        }
        if (arg1->unk0.animStateTime >= 0x51) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, sp38->unk3B8[9], 0x7FU, NULL, NULL, 0, NULL);
            func_80023D30(arg0, 9, 0.0f, 0U);
        }
        arg0->speed.f[1] -= 0.1f * arg2;
        arg1->unk0.unk278 *= 0.98f;
        break;
    case 0x13:
        gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, 1.0f, 1);
        arg0->speed.f[1] = 0.0f;
        if (arg0->animProgress > 0.99f) {
            return 2;
        }
        sp38->unk8B8 = 3;
        break;
    case 0x418:
        gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, 1.0f, 1);
        arg0->speed.f[1] = 0.0f;
        if (arg0->animProgress > 0.99f) {
            if ((arg1->unk0.unk294 >= 0.42000002f) && (arg1->unk0.unk290 >= 0.42000002f) && (*_data_6FC <= arg1->unk0.speed)) {
                return 5;
            }
            return 2;
        }
        sp38->unk8B8 = 3;
        break;
    case 9:
        arg1->unk0.animTickDelta = 0.02f;
        if (arg1->unk0.unk33A != 0) {
            func_80023D30(arg0, 0xA, 0.0f, 0U);
        }
        if (arg1->unk0.unk4.floorDist == 0.0f) {
            if (arg1->unk0.unk4.unk68.unk50[0] == 8) {
                dll_210_func_4634(arg0, 9, 0);
                return 0x10;
            }
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x226U, 0x7FU, NULL, NULL, 0, NULL);
            func_80023D30(arg0, 0xB, 0.0f, 0U);
            dll_210_add_health(arg0, -4);
        }
        arg0->speed.f[1] -= 0.1f * arg2;
        arg1->unk0.unk278 *= 0.98f;
        break;
    case 10:
        arg1->unk0.animTickDelta = 0.01f;
        if (arg1->unk0.unk4.floorDist == 0.0f) {
            if (arg1->unk0.unk4.unk68.unk50[0] == 8) {
                dll_210_func_4634(arg0, 9, 0);
                return 0x10;
            }
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x226U, 0x7FU, NULL, NULL, 0, NULL);
            func_80023D30(arg0, 0xB, 0.0f, 0U);
            dll_210_add_health(arg0, -8);
        }
        if (arg1->unk0.animStateTime >= 0x3D) {
            dll_210_func_4634(arg0, 9, 0);
        }
        arg0->speed.f[1] -= 0.1f * arg2;
        arg1->unk0.unk278 *= 0.98f;
        break;
    case 0xB:
        arg1->unk0.animTickDelta = 0.015f;
        arg1->unk0.unk278 = 0.0f;
        if (arg1->unk0.unk33A != 0) {
            if (sp38->stats->health > 0) {
                func_80023D30(arg0, 0xC, 0.0f, 0U);
                break;
            }
            dll_210_func_9F1C(arg0, 1);
            return -0xE;
        }
        break;
    case 0xC:
        arg1->unk0.animTickDelta = 0.004f;
        arg1->unk0.unk278 = 0.0f;
        if (arg1->unk0.unk33A != 0) {
            return -1;
        }
        break;
    default:
        func_80023D30(arg0, 0x54, 0.0f, 0U);
        break;
    }

    if (gDLL_2_Camera->vtbl->func3() == 0x5D) {
        gDLL_2_Camera->vtbl->func6(0x54, 0, 2, 0, NULL, 0, 0xFF);
    }

    if ((arg0->speed.f[1] < 0.0f) && (arg1->unk0.unk4.underwaterDist > 5.0f)) {
        return 0x20;
    }
    return 0;
}
#endif

// offset: 0xD3E4 | func: 71
s32 dll_210_func_D3E4(Object* arg0, Player_Data* arg1, UNK_TYPE_32 arg2) {
    Player_Data *objdata = arg0->data;

    if (!(arg1->unk0.unk4.unk25C & 0x10) && (arg1->unk0.unk4.underwaterDist > 5.0f)) {
        return 0x20;
    }

    arg1->unk0.unk341 = 3;

    if (objdata->stats->health > 0) {
        func_80023D30(arg0, 0xC, 0.0f, 0U);
        return -0xD;
    }
    return 0;
}

// offset: 0xD480 | func: 72
s32 dll_210_func_D480(Object* arg0, Player_Data* arg1, UNK_TYPE_32 arg2) {
    if (!(arg1->unk0.unk4.unk25C & 0x10) && (arg1->unk0.unk4.underwaterDist > 5.0f)) {
        return 0x20;
    }

    dll_210_func_A024(arg0, arg1);
    arg1->unk0.flags |= 0x200000;
    return 0;
}

// offset: 0xD510 | func: 73
static void dll_210_func_D510(Player_Data* arg0, f32 arg1) {
    f32 var_fv1;

    if (arg0->unk0.unk290 < 0.05f) {
        arg0->unk0.unk328 = 0;
        arg0->unk0.unk32A = 0;
        arg0->unk0.unk290 = 0.0f;
    }
    var_fv1 = (arg0->unk0.unk290 - 0.4f) / 0.6f;
    if (var_fv1 < 0.0f) {
        var_fv1 = 0.0f;
    }
    if (var_fv1 > 1.0f) {
        var_fv1 = 1.0f;
    }
    arg0->unk0.speed += (((var_fv1 * 1.6f) - arg0->unk0.speed) / arg0->unk0.unk2B0) * arg1;
    if (arg0->unk0.speed > 1.65f) {
        arg0->unk0.speed = 1.65f;
    }
}

// offset: 0xD5F0 | func: 74
s32 dll_210_func_D5F0(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* sp2C;

    sp2C = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x10;
    }
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    arg0->speed.f[1] = 0;
    if (arg0->curModAnimId == 0x419) {
        if (arg1->unk0.unk33A != 0) {
            _bss_200 = 3;
            gDLL_2_Camera->vtbl->func10(arg0->srt.transl.f[0], sp2C->unk490.unk10, arg0->srt.transl.f[2]);
            return -0x13;
        }
    } else {
        func_80023D30(arg0, 0x419, 0, 1U);
        arg0->srt.yaw = arctan2_f(sp2C->unk490.unk1C.x, sp2C->unk490.unk1C.z); 
        arg0->srt.transl.f[0] = sp2C->unk490.unk2C.x;
        arg0->srt.transl.f[1] = sp2C->unk490.unk4;
        arg0->srt.transl.f[2] = sp2C->unk490.unk2C.z;
        sp2C->unk490.unkC = sp2C->unk490.unk2C.x;
        sp2C->unk490.unk10 = (sp2C->unk490.unk4 - 26.0f);
        sp2C->unk490.unk14 = sp2C->unk490.unk2C.z;
        arg1->unk0.animTickDelta = 0.04f;
    }
    gDLL_2_Camera->vtbl->func10(arg0->srt.transl.f[0], sp2C->unk490.unk10, arg0->srt.transl.f[2]);
    return 0;
}

// offset: 0xD788 | func: 75
typedef struct {
s16 unk0;
s8 unk2;
s8 unk3;
s32 pad;
f32 unk8;
f32 unkC;
f32 unk10;
} UnkStruct_D788;

s32 dll_210_func_D788(Object* arg0, Player_Data* arg1, f32 arg2) {
    s32 temp_v0;
    s32 var_v0;
    UnkStruct_D788 sp44;
    Player_Data* temp_s0;

    temp_s0 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, temp_s0->unk3B8[0x17], 0x7FU, NULL, NULL, 0, NULL);
        arg1->unk0.unk270 = 0x11;
    }
    temp_s0->unk7FC = 0;
    goto dummy_label_21840; dummy_label_21840: ;
    temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
    if (temp_v0 != 0) {
        return temp_v0;
    }
    arg0->speed.f[1] = 0.0f;
    sp44.unk10 = temp_s0->unk490.unk2C.x;
    sp44.unk8 = temp_s0->unk490.unk2C.z;
    switch (arg0->curModAnimId) {
    case 0xD:
        sp44.unkC = temp_s0->unk490.unk4 + ((temp_s0->unk490.unk10 - temp_s0->unk490.unk4) * arg0->animProgress);
        gDLL_18_objfsa->vtbl->func9(arg0, &arg1->unk0, arg2, 0.2f);
        if (arg1->unk0.unk33A != 0) {
            func_80023D30(arg0, _data_57C[0], 0.0f, 0U);
            arg1->unk0.animTickDelta = 0.021f;
            sp44.unk0 = 0;
            sp44.unk2 = 0;
            sp44.unk3 = 1;
        }
        break;
    case 0x22:
        sp44.unkC = temp_s0->unk490.unk4 + ((temp_s0->unk490.unk10 - temp_s0->unk490.unk4) * arg0->animProgress);
        gDLL_18_objfsa->vtbl->func9(arg0, &arg1->unk0, arg2, 0.2f);
        if (arg1->unk0.unk33A != 0) {
            func_80023D30(arg0, _data_57C[1], 0.0f, 0U);
            arg1->unk0.animTickDelta = 0.021f;
            sp44.unk0 = 0;
            sp44.unk2 = 0;
            sp44.unk3 = 1;
        }
        break;
    case 0x4F:
    case 0x50:
        sp44.unkC = temp_s0->unk490.unk10;
        gDLL_18_objfsa->vtbl->func9(arg0, &arg1->unk0, arg2, 0.2f);
        gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, arg2, 2);
        if (arg1->unk0.unk33A != 0) {
            _bss_200 = 3;
            gDLL_2_Camera->vtbl->func10(sp44.unk10, sp44.unkC, sp44.unk8);
            return -0x13;
        }
        break;
    default:
        temp_v0 = arctan2_f(temp_s0->unk490.unk1C.x, temp_s0->unk490.unk1C.z) - arg0->srt.yaw;
        CIRCLE_WRAP(temp_v0)
        if (temp_s0->unk490.unk47 == 1) {
            var_v0 = 0xA;
        } else {
            var_v0 = 0xB;
        }
        arg0->srt.yaw += temp_v0;
        arg0->srt.transl.f[1] = temp_s0->unk490.unk4;
        func_80023D30(arg0, _data_564[var_v0], 0.0f, 4U);
        arg1->unk0.animTickDelta = 0.033f;
        arg1->unk0.unk2EC.x = ((temp_s0->unk490.unk2C.x - arg0->srt.transl.f[0]) + (temp_s0->unk490.unk1C.x * 0.5f));
        arg1->unk0.unk2EC.y = 0.0f;
        arg1->unk0.unk2EC.z = ((temp_s0->unk490.unk2C.z - arg0->srt.transl.f[2]) + (temp_s0->unk490.unk1C.z * 0.5f));
        arg1->unk0.unk2A4 = 0.0f;
        temp_s0->unk490.unkC = temp_s0->unk490.unk2C.x;
        temp_s0->unk490.unk10 = (temp_s0->unk490.unk4 - 26.0f);
        temp_s0->unk490.unk14 = temp_s0->unk490.unk2C.z;
        sp44.unkC = temp_s0->unk490.unk4;
        gDLL_2_Camera->vtbl->func6(0x55, 1, 0, 4, &sp44, 0, 0xFF);
        break;
    }

    temp_s0->unk8BD |= 4;
    gDLL_2_Camera->vtbl->func10(sp44.unk10, sp44.unkC, sp44.unk8);
    return 0;
}

// offset: 0xDC10 | func: 76
s32 dll_210_func_DC10(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* temp_s1;
    f32 var_fa0;
    f32 var_fv0;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 var_fv1;
    s32 pad;

    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x12;
    }
    temp_s1 = arg0->data;
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    arg0->speed.f[1] = 0.0f;
    switch (_bss_200) {
        case 0xE:
        case 0x12:
        case 0x16:
        case 0x1A:
            if (arg1->unk0.unk308 & 1) {
                gDLL_6_AMSFX->vtbl->play_sound(arg0, temp_s1->unk3B8[rand_next(0xA, 0xB)], 0x7FU, NULL, NULL, 0, NULL);
            }
            if (arg1->unk0.unk33A != 0) {
                arg0->positionMirror.f[0] = temp_s1->unk7EC.x;
                arg0->positionMirror.f[1] = temp_s1->unk490.unk4;
                arg0->positionMirror.f[2] = temp_s1->unk7EC.z;
                inverse_transform_point_by_object(arg0->positionMirror.f[0], arg0->positionMirror.f[1], arg0->positionMirror.f[2], arg0->srt.transl.f, &arg0->srt.transl.f[1], &arg0->srt.transl.f[2], arg0->parent);
                arg0->curModAnimIdLayered = -1;
                dll_210_func_7260(arg0, temp_s1);
                func_80023D30(arg0, (s32) *temp_s1->modAnims, 0.0f, 1U);
                return 2;
            }
            break;
        default:
            if (temp_s1->unk490.unk46 == 6) {
                if (_bss_1B0[4] <= temp_s1->unk490.unk0) {
                    _bss_200 = 0xE;
                    var_fa0 = _bss_1B0[5];
                    var_fv1 = _bss_1B0[4];
                    arg1->unk0.animTickDelta = 0.009f;
                } else if (_bss_1B0[8] <= temp_s1->unk490.unk0) {
                    _bss_200 = 0x16;
                    var_fa0 = _bss_1B0[9];
                    var_fv1 = _bss_1B0[8];
                    arg1->unk0.animTickDelta = 0.011f;
                } else {
                    _bss_200 = 0x12;
                    var_fv1 = _bss_1B0[6];
                    var_fa0 = _bss_1B0[7];
                    arg1->unk0.animTickDelta = 0.014f;
                }
            } else {
                _bss_200 = 0x1A;
                var_fv1 = _bss_1B0[10];
                var_fa0 = _bss_1B0[11];
                arg1->unk0.animTickDelta = 0.01f;
            }
            var_fv0 = ((temp_s1->unk490.unk0 - var_fv1) / (var_fa0 - var_fv1));
            var_fv0 *= 1023.0f;
            if (var_fv0 < 0.0f) {
                var_fv0 = 0.0f;
            } else if (var_fv0 > 1023.0f) {
                var_fv0 = 1023.0f;
            }
            temp_s1->unk490.unk44 = var_fv0;
            dll_210_func_8AE0(arg0, _data_564[_bss_200 + 0], _data_564[_bss_200 + 2], &temp_s1->unk490.unk38, &temp_s1->unk490.unk1C, 0.0f, arg1->unk0.animTickDelta, 2U, 9U);
            dll_210_func_8AE0(arg0, _data_564[_bss_200 + 0], _data_564[_bss_200 + 1], &temp_s1->unk490.unk38, &temp_s1->unk490.unk1C, 0.0f, arg1->unk0.animTickDelta, 2U, 0x34U);
            dll_210_func_8AE0(arg0, _data_564[_bss_200 + 2], _data_564[_bss_200 + 3], &temp_s1->unk490.unk38, &temp_s1->unk490.unk1C, 0.0f, arg1->unk0.animTickDelta, 2U, 0x1AU);
            arg0->srt.yaw = arctan2_f(temp_s1->unk490.unk1C.x, temp_s1->unk490.unk1C.z);
            arg0->srt.transl.f[0] = temp_s1->unk490.unk2C.x;
            arg0->srt.transl.f[2] = temp_s1->unk490.unk2C.z;
            dll_210_func_7260(arg0, temp_s1);
            break;
    }
    func_80024DD0(arg0, 0, 2, 0);
    func_80024DD0(arg0, 1, 2, 0);
    func_80024DD0(arg0, 1, 0, temp_s1->unk490.unk44);
    func_80025140(arg0, arg1->unk0.animTickDelta, arg2, 0);
    sp60 = temp_s1->unk7EC.x;
    sp5C = arg0->srt.transl.f[1];
    sp5C += (temp_s1->unk490.unk4 - arg0->srt.transl.f[1]) * arg0->animProgress;
    sp58 = temp_s1->unk7EC.z;
    gDLL_2_Camera->vtbl->func10(sp60, sp5C, sp58);
    func_8004D844(arg0, sp60, sp5C, sp58);
    return 0;
}

// offset: 0xE14C | func: 77
s32 dll_210_func_E14C(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 temp_fa0;
    f32 temp_ft1;
    f32 temp_fv0;
    Vec3f sp88;
    Vec3f sp7C;
    Vec3f sp70;
    f32 sp6C;
    f32 sp68;
    f32 temp_fv1;
    s16 pad;
    s16 sp60;
    s16 sp5E;
    ModelInstance* sp58;
    Player_Data* objdata;
    Vec3f sp48;
    u8 sp47;

    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x13;
        arg0->speed.f[1] = 0.0f;
    }
    objdata = arg0->data;
    objdata->unk7FC = 0.0f;
    sp58 = arg0->modelInsts[arg0->modelInstIdx];
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    // @fake
    if ((_bss_200 && _bss_200) && _bss_200) {}
    _bss_202 = _bss_200;
    sp5E = 0;
    sp47 = 0;
    switch (_bss_200) {
    case 0:
        if (arg1->unk0.unk33A != 0) {
            arg0->positionMirror.f[0] = objdata->unk7EC.x;
            arg0->positionMirror.f[1] = objdata->unk7EC.y;
            arg0->positionMirror.f[2] = objdata->unk7EC.z;
            inverse_transform_point_by_object(arg0->positionMirror.f[0], arg0->positionMirror.f[1], arg0->positionMirror.f[2], arg0->srt.transl.f, &arg0->srt.transl.f[1], &arg0->srt.transl.f[2], arg0->parent);
            func_80023D30(arg0, (s32) *_data_568, 0.0f, 1U);
            arg1->unk0.animTickDelta = 0.01f;
            _bss_202 = _bss_200 = 2;
            func_8001A3FC(sp58, 0U, 0, 0.0f, arg0->srt.scale, &sp7C, &sp60);
            arg0->srt.transl.f[1] -= sp7C.f[1];
            temp_fa0 = _bss_1B0[2] + (objdata->unk490.unk4 - objdata->unk7EC.y);
            temp_fa0 = -temp_fa0 * -0.3f;
            if (temp_fa0 >= 0.0f) {
                arg0->speed.f[1] = sqrtf(temp_fa0);
            } else {
                arg0->speed.f[1] = 0.0f;
            }

            sp48.f[0] = *_bss_1F8 * objdata->unk490.unk1C.x;
            sp48.f[1] = *_bss_1F8 * objdata->unk490.unk1C.y;
            sp48.f[2] = *_bss_1F8 * objdata->unk490.unk1C.z;
            sp70.f[0] = sp48.f[0] + objdata->unk490.unk2C.f[0];
            sp70.f[1] = sp48.f[1] + objdata->unk490.unk2C.f[1];
            sp70.f[2] = sp48.f[2] + objdata->unk490.unk2C.f[2];
            arg1->unk0.unk2EC.y = 0.0f;
            arg1->unk0.unk2EC.x = objdata->unk490.unk2C.x - arg0->srt.transl.f[0];
            arg1->unk0.unk2EC.z = objdata->unk490.unk2C.z - arg0->srt.transl.f[2];
            _bss_204 = arg0->srt.transl.f[0];
            _bss_208 = arg0->srt.transl.f[2];
            gDLL_6_AMSFX->vtbl->play_sound(arg0, objdata->unk3B8[8], 0x7FU, NULL, NULL, 0, NULL);
        } else {
            gDLL_18_objfsa->vtbl->func10(arg0, &arg1->unk0, arg2, 0.1f);
        }
        gDLL_2_Camera->vtbl->func10(objdata->unk490.unkC, objdata->unk490.unk10, objdata->unk490.unk14);
        break;
    case 2:
        temp_fa0 = _bss_1B0[2] + objdata->unk490.unk4;
        arg0->speed.f[1] += -0.15f * arg2;
        sp68 = (objdata->unk7EC.y - objdata->unk490.unk8) / (temp_fa0 - objdata->unk490.unk8);
        if (sp68 < 0.0f) {
            sp68 = 0.0f;
        } else if (sp68 > 1.0f) {
            sp68 = 1.0f;
        }
        arg0->srt.transl.f[0] = (arg1->unk0.unk2EC.x * sp68) + _bss_204;
        arg0->srt.transl.f[2] = (arg1->unk0.unk2EC.z * sp68) + _bss_208;
        if (temp_fa0 <= objdata->unk7EC.y) {
            _bss_200 = 3;
            sp47 = 1;
            sp6C = 0.035f;
            arg0->srt.transl.f[1] = objdata->unk490.unk4;
            arg0->speed.f[1] = 0.0f;
        }
        objdata->unk490.unk10 += (arg0->srt.transl.f[1] - objdata->unk490.unk10) * 0.02f * arg2;
        gDLL_2_Camera->vtbl->func10(objdata->unk490.unkC, objdata->unk490.unk10, objdata->unk490.unk14);
        func_8004D844(arg0, objdata->unk490.unkC, objdata->unk490.unk10, objdata->unk490.unk14);
        break;
    case 3:
        if (arg1->unk0.unk33A != 0) {
            temp_fv0 = arg1->unk0.unk284;
            if (temp_fv0 > 5.0f) {
                _bss_200 = 5;
                sp6C = 0.014f;
                gDLL_6_AMSFX->vtbl->play_sound(arg0, objdata->unk3B8[rand_next(0xA, 0xB)], 0x7FU, NULL, NULL, 0, NULL);
            } else {
                if ((temp_fv0 < -5.0f) && (objdata->unk490.unk46 != 0x11)) {
                    return 0x15;
                }
                _bss_200 = 6;
                sp6C = 0.008f;
            }
        }
        gDLL_2_Camera->vtbl->func10(objdata->unk490.unkC, objdata->unk490.unk10, objdata->unk490.unk14);
        func_8004D844(arg0, objdata->unk490.unkC, objdata->unk490.unk10, objdata->unk490.unk14);
        break;
    case 6:
        if (arg1->unk0.unk284 > 5.0f) {
            _bss_200 = 5;
            sp6C = 0.014f;
            gDLL_6_AMSFX->vtbl->play_sound(arg0, objdata->unk3B8[rand_next(0xA, 0xB)], 0x7FU, NULL, NULL, 0, NULL);
        } else if ((arg1->unk0.unk284 < -5.0f) && (objdata->unk490.unk46 != 0x11)) {
            return 0x15;
        }
        gDLL_2_Camera->vtbl->func10(objdata->unk490.unkC, objdata->unk490.unk10, objdata->unk490.unk14);
        func_8004D844(arg0, objdata->unk490.unkC, objdata->unk490.unk10, objdata->unk490.unk14);
        break;
    case 5:
        if (arg1->unk0.unk33A != 0) {
            arg0->positionMirror.f[0] = objdata->unk7EC.x;
            arg0->positionMirror.f[2] = objdata->unk7EC.z;
            inverse_transform_point_by_object(arg0->positionMirror.f[0], 0.0f, arg0->positionMirror.f[2], arg0->srt.transl.f, &sp68, &arg0->srt.transl.f[2], arg0->parent);
            dll_210_func_7260(arg0, objdata);
            func_80023D30(arg0, (s32) *objdata->modAnims, 0.0f, 1U);
            return 2;
        }
        sp88.z = objdata->unk490.unkC + ((arg0->srt.transl.x - objdata->unk490.unkC) * arg0->animProgress);
        sp88.y = objdata->unk490.unk10 + ((objdata->unk490.unk4 - objdata->unk490.unk10) * arg0->animProgress);
        sp88.x = objdata->unk490.unk14 + ((arg0->srt.transl.z - objdata->unk490.unk14) * arg0->animProgress);
        gDLL_2_Camera->vtbl->func10(sp88.z, sp88.y, sp88.x);
        func_8004D844(arg0, sp88.z, sp88.y, sp88.x);
        break;
    default:
        arg0->speed.f[1] = 0.0f;
        temp_fv0 = (1.0f - ((objdata->unk490.unk0 - 32.0f) / 32));
        sp5E = temp_fv0 * 1023.0f;
        _bss_200 = 0;
        sp6C = 0.029f;
        arg1->unk0.unk2F8 = arctan2_f(objdata->unk490.unk1C.x, objdata->unk490.unk1C.z) - arg0->srt.yaw;
        if (arg1->unk0.unk2F8 > 32768.0f) {
            arg1->unk0.unk2F8 += -65535.0f;
        }
        if (arg1->unk0.unk2F8 < -32768.0f) {
            arg1->unk0.unk2F8 += 65535.0f;
        }
        arg1->unk0.unk2A0 = 0.0f;
        objdata->unk490.unkC = arg0->srt.transl.f[0];
        objdata->unk490.unk10 = arg0->srt.transl.f[1];
        objdata->unk490.unk14 = arg0->srt.transl.f[2];
        break;
    }
    if (_bss_202 != _bss_200) {
        func_80023D30(arg0, _data_564[_bss_200], 0.0f, sp47);
        if (sp5E != 0) {
            func_80025540(arg0, _data_564[_bss_200 + 1], sp5E);
        }
        arg1->unk0.animTickDelta = sp6C;
    }
    dll_210_func_7260(arg0, objdata);
    return 0;
}

// offset: 0xEB1C | func: 78
s32 dll_210_func_EB1C(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* objdata;
    ModelInstance* modelInstance;
    f32 sp7C;
    f32 temp_fa0;
    Vec3f sp6C;
    f32 temp_fa0_2;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 temp_fa1;
    s16 sp52[3];
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fv1;

    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x14;
        arg1->unk0.animExitAction = dll_210_func_EF8C;
        _bss_200 = -1;
    }
    objdata = arg0->data;
    modelInstance = arg0->modelInsts[arg0->modelInstIdx];
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
        temp_v0 = dll_210_func_C1F4(arg0, arg1, arg2);
        if (temp_v0 != 0) {
            return temp_v0;
        }
    }
    if (!(arg1->unk0.unk4.unk25C & 0x10) && (arg1->unk0.unk4.underwaterDist > 5.0f)) {
        return 0x20;
    }
    sp52[1] = 0;
    _bss_202 = _bss_200;
    temp_fv1 = objdata->unk7EC.f[0];
    sp64 = temp_fv1;
    temp_fa0 = objdata->unk7EC.f[2];
    sp5C = temp_fa0;
    switch (_bss_200) {
    default:
        _bss_200 = 7;
        sp7C = 0.03f;
        sp60 = objdata->unk490.unk10;
        break;
    case 7:
        temp_fv0 = 1.0f - arg0->animProgress;
        sp64 += (objdata->unk490.unk2C.f[0] - sp64) * temp_fv0;
        sp60 = objdata->unk490.unk10;
        sp5C += (objdata->unk490.unk2C.f[2] - sp5C) * temp_fv0;
        if (arg1->unk0.unk33A != 0) {
            arg0->positionMirror.x = temp_fv1;
            // @fake
            if (1) {}
            arg0->positionMirror.y = objdata->unk7EC.y;
            arg0->positionMirror.z = objdata->unk7EC.z;
            inverse_transform_point_by_object(arg0->positionMirror.x, arg0->positionMirror.y, arg0->positionMirror.z, arg0->srt.transl.f, &arg0->srt.transl.f[1], &arg0->srt.transl.f[2], arg0->parent);
            func_80023D30(arg0, *_data_574, 0.0f, 1U);
            arg1->unk0.animTickDelta = 0.02f;
            _bss_202 = _bss_200 = 8;
            func_8001A3FC(modelInstance, 0U, 0, 0.0f, arg0->srt.scale, &sp6C, &sp52[2]);
            arg0->srt.transl.f[1] -= sp6C.f[1];
            arg0->speed.f[1] = -2.0f;
            objdata->unk490.unk18 = 1.0f;
        }
        break;
    case 8:
        arg1->unk0.unk4.mode = 2;
        objdata->unk490.unk18 -= 0.05f * arg2;
        if (objdata->unk490.unk18 < 0.0f) {
            objdata->unk490.unk18 = 0.0f;
        }
        temp_fv0 = arg0->srt.transl.f[1];
        sp60 = temp_fv0;
        sp60 += ((objdata->unk490.unk10 - temp_fv0) * objdata->unk490.unk18);
        arg0->speed.f[1] += -0.05f * arg2;
        temp_fa1 = (_bss_1B0[3] + arg1->unk0.unk4.floorY);
        if (temp_fv0 <= temp_fa1) {
            objdata->unk490.unk10 = sp60;
            _bss_200 = 9;
            sp52[1] = 1;
            sp7C = 0.027f;
            arg0->srt.transl.f[1] = arg1->unk0.unk4.floorY;
            arg0->speed.f[1] = 0.0f;
        }
        break;
    case 9:
        var_fv1 = arg0->animProgress;
        if (arg0->animProgress > 0.8f) {
            var_fv1 = 0.8f;
        }
        var_fv1 /= 0.8f;

        // @fake
        temp_fa1 = arg0->srt.transl.f[1];
        sp60 = arg0->srt.transl.f[1];
        sp60 += ((objdata->unk490.unk10 - sp60) * (1.0f - var_fv1));
        if (arg0->animProgress > 0.95f) {
            return 2;
        }
        break;
    }

    gDLL_2_Camera->vtbl->func10(sp64, sp60, sp5C);
    if (_bss_202 != _bss_200) {
        func_80023D30(arg0, (s32) _data_564[_bss_200], 0.0f, sp52[1]);
        arg1->unk0.animTickDelta = sp7C;
    }
    dll_210_func_7260(arg0, objdata);
    return 0;
}

// offset: 0xEF8C | func: 79
static void dll_210_func_EF8C(Object *arg0, ObjFSA_Data *arg1) {

}

// offset: 0xEF9C | func: 80
s32 dll_210_func_EF9C(Object* player, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0xEFB4 | func: 81
static s32 dll_210_func_EFB4(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* temp_v1;

    temp_v1 = arg0->data;
    temp_v1->flags &= ~2;
    arg1->unk0.unk4.mode = 0;
    arg1->unk0.unk278 = 0.0f;
    arg1->unk0.unk27C = 0.0f;
    arg1->unk0.flags |= 0x200000;
    arg0->speed.f[0] = 0.0f;
    arg0->speed.f[2] = 0.0f;

    return 0;
}

// offset: 0xF00C | func: 82
s32 dll_210_func_F00C(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 pad;
    f32 var_fv1;
    s32 spA4;
    Player_Data* objdata;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_v1;
    s16 pad_sp92;
    s16 sp90;
    Vec3f sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    Vec3f sp6C;
    Vec4f sp5C;
    s16* var_v0;

    objdata = arg0->data;
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    arg0->speed.y = 0.0f;
    spA4 = objdata->unk3CC.unk0 != 1;
    if (spA4) {
        arg1->unk0.animTickDelta = 0.01f;
    } else {
        arg1->unk0.animTickDelta = 0.014f;
    }
    if (arg1->unk0.unk308 & 0x80) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, (u16) objdata->unk3B8[rand_next(0xA, 0xE)], 0x7FU, NULL, NULL, 0, NULL);
    }
    if (arg1->unk0.unk308 & 1) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xB1CU, 0x7FU, NULL, NULL, 0, NULL);
    }
    if (arg1->unk0.enteredAnimState != 0) {
        objdata->unk8A9 = 1;
        arg1->unk0.unk278 = 0.0f;
        arg1->unk0.unk27C = 0.0f;
        arg1->unk0.unk270 = 0x17;
        arg1->unk0.animExitAction = dll_210_func_12514;
        sp5C.y = 0.0f;
        if (spA4 != 0) {
            sp5C.x = -objdata->unk3CC.unk28.x;
            sp5C.z = -objdata->unk3CC.unk28.z;
            sp5C.w = -objdata->unk3CC.unk28.w;
        } else {
            sp5C.x = objdata->unk3CC.unk28.x;
            sp5C.z = objdata->unk3CC.unk28.z;
            sp5C.w = objdata->unk3CC.unk28.w;
        }
        var_v1 = arctan2_f(sp5C.x, sp5C.z) - arg0->srt.yaw;
        CIRCLE_WRAP(var_v1)
        arg0->srt.yaw += var_v1;
        objdata->unk3CC.unk20 = arg0->srt.transl.x;
        objdata->unk3CC.unk24 = arg0->srt.transl.z;
        arg0->srt.transl.x = objdata->unk3CC.unk48.x;
        arg0->srt.transl.z = objdata->unk3CC.unk48.z;
        if (objdata->unk3CC.unk18 >= 0.0f) {
            var_t0 = 0;
        } else {
            var_t0 = 4;
        }
        if (spA4 != 0) {
            var_v0 = _data_6E0;
        } else {
            var_v0 = _data_6D0;
        }
        objdata->unk42C = dll_210_func_8AE0(arg0, var_v0[var_t0], var_v0[var_t0 + 2], &objdata->unk3CC.unk54, &sp5C, 0.0f, arg1->unk0.animTickDelta, 2U, 9U);
        var_t0_2 = 0x34;
        if (spA4 != 0) {
            var_t0_2 = 0x74;
        }
        dll_210_func_8AE0(arg0, var_v0[var_t0], var_v0[var_t0 + 1], &objdata->unk3CC.unk54, &objdata->unk3CC.unk38, 0.0f, arg1->unk0.animTickDelta, 0U, var_t0_2);
        dll_210_func_8AE0(arg0, var_v0[var_t0 + 2], var_v0[var_t0 + 3], &objdata->unk3CC.unk54, &objdata->unk3CC.unk38, 0.0f, arg1->unk0.animTickDelta, 0U, 0x1AU);
        objdata->unk3CC.unk10 = (objdata->unk3CC.unk8 + (objdata->unk3CC.unkC * objdata->unk3CC.unk0));
        objdata->unk3CC.unk14 = arg0->srt.transl.y;
        func_8001A3FC(arg0->modelInsts[arg0->modelInstIdx], 0U, 0, 1.0f, arg0->srt.scale, &sp84, &sp90);
        _bss_204 = sp84.f[1] + arg0->srt.transl.y;
        _bss_208 = _bss_1B0[1] + objdata->unk3CC.unk10;
        sp6C.x = objdata->unk3CC.unk4;
        sp6C.y = objdata->unk3CC.unk8;
        gDLL_2_Camera->vtbl->func6(0x5D, 1, 1, 8, &sp6C, 0, 0);
    } else if (arg0->animProgress > 0.9f) {
        func_80025140(arg0, arg1->unk0.animTickDelta, arg2, 0);
        return 0x19;
    }
    if (arg0->animProgress >= 0.7f) {
        var_fv1 = ((arg0->animProgress * 1.052f) - 0.7f) * 3.333f;
        if (var_fv1 > 1.0f) {
            var_fv1 = 1.0f;
        } else if (var_fv1 < 0.0f) {
            var_fv1 = 0.0f;
        }
        arg0->srt.transl.y = objdata->unk3CC.unk14 + ((_bss_208 - _bss_204) * var_fv1);
    }
    func_80024DD0(arg0, 0, 2, 0);
    func_80024DD0(arg0, 1, 2, 0);
    func_80024DD0(arg0, 1, 0, objdata->unk42C);
    func_80025140(arg0, arg1->unk0.animTickDelta, arg2, 0);
    sp80 = objdata->unk3CC.unk20 + ((arg0->srt.transl.x - objdata->unk3CC.unk20) * arg0->animProgress);
    sp7C = arg0->srt.transl.y + ((objdata->unk3CC.unk10 - arg0->srt.transl.y) * arg0->animProgress);
    sp78 = objdata->unk3CC.unk24 + ((arg0->srt.transl.z - objdata->unk3CC.unk24) * arg0->animProgress);
    gDLL_2_Camera->vtbl->func10(arg0->srt.transl.x, sp7C, arg0->srt.transl.z);
    func_8004D844(arg0, sp80, sp7C, sp78);
    dll_210_func_7260(arg0, arg0->data);
    return 0;
}

// offset: 0xF690 | func: 83
#ifndef NON_MATCHING
s32 dll_210_func_F690(Object* arg0, Player_Data* arg1, f32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_F690.s")
#else
// https://decomp.me/scratch/zsXY2
s32 dll_210_func_F690(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    Vec3f sp78;
    Vec3f sp6C;
    Player_Data* temp_s0;
    f32 temp_fa0;
    ModelInstance* sp60;
    f32 temp_fv0;
    f32 temp_fv1;
    s16 pad_sp56;
    s16 sp54;
    s16 temp_v0_2;
    s16 var_v1;
    s32 temp_v0_3;

    temp_s0 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        temp_s0->unk8A9 = 1;
        temp_v0_2 = arg0->curModAnimId;
        if ((temp_v0_2 == *_data_5B0) || (temp_v0_2 == *_data_5B8)) {
            _bss_200 = 8;
        } else {
            _bss_200 = 9;
        }
        arg1->unk0.unk270 = 0xD;
        arg1->unk0.animExitAction = dll_210_func_12514;
    }
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    if ((arg1->unk0.unk4.underwaterDist > 25.0f) && (arg1->unk0.unk4.floorDist < 100.0f)) {
        return 0x21;
    }
    arg0->speed.y = 0.0f;
    sp9C = arg1->unk0.unk284 / 60.0f;
    if (sp9C < 0.0f) {
        sp9C = -sp9C;
    }
    if (sp9C < 0.1f) {
        sp9C = 0.1f;
    }
    if (sp9C > 1.0f) {
        sp9C = 1.0f;
    }
    sp60 = arg0->modelInsts[arg0->modelInstIdx];
    sp98 = 0.0f;
    sp94 = arg1->unk0.animTickDelta;
    _bss_202 = _bss_200;
    if (arg1->unk0.unk308 & 1) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xB1CU, 0x7FU, NULL, NULL, 0, NULL);
    }
    switch (_bss_200) {
    case 8:
    case 9:
    case 12:
    case 13:
        arg0->srt.transl.y = temp_s0->unk3CC.unk10;
        arg0->curModAnimIdLayered = -1;
        temp_s0->unk3CC.unk2 = 0;
        temp_s0->unk3CC.unk14 = temp_s0->unk3CC.unk10;
        sp94 = 0.0f;
        if (_bss_200 & 1) {
            _bss_200 = 1;
        } else {
            _bss_200 = 0;
        }
        break;
    case 6:
    case 7:
        if (arg1->unk0.unk308 & 0x80) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, temp_s0->unk3B8[rand_next(0xA, 0xE)], 0x7FU, NULL, NULL, 0, NULL);
        }
        if (arg1->unk0.unk33A != 0) {
            arg0->srt.transl.y = temp_s0->unk3CC.unk4;
        } else {
            func_8001A3FC(sp60, 0, 0, 0, arg0->srt.scale, &sp78, &sp54);
            func_8001A3FC(sp60, 0, 0, 1, arg0->srt.scale, &sp6C, &sp54);
            temp_fv1 = sp78.y + _bss_204;
            temp_fa0 = _bss_208 - (sp6C.y - sp78.y);
            arg0->srt.transl.y = ((temp_fa0 - temp_fv1) * arg0->animProgress) + _bss_204;
        }
        /* fallthrough */
    case 10:
    case 11:
        if (arg1->unk0.unk33A != 0) {
            arg0->positionMirror.x = temp_s0->unk7EC.x;
            arg0->positionMirror.z = temp_s0->unk7EC.z;
            inverse_transform_point_by_object(arg0->positionMirror.x, 0.0f, arg0->positionMirror.z, arg0->srt.transl.f, &sp84, &arg0->srt.transl.z, arg0->parent);
            dll_210_func_7260(arg0, temp_s0);
            func_80023D30(arg0, *temp_s0->modAnims, 0.0f, 1U);
            return -1;
        }
        break;
    case 4:
    case 5:
        if (arg1->unk0.unk284 > 5.0f) {
            goto dummy_label_201559; dummy_label_201559: ;
            func_800240BC(arg0, 0.0f);
        } else if (arg1->unk0.unk284 < -5.0f) {
            goto dummy_label_201558; dummy_label_201558: ;
            func_800240BC(arg0, 0.0f);
        } else if ((arg1->unk0.unk310 & 0x8000) && (temp_s0->unk3CC.unk0 >= 4)) {
            return -0x19;
        } else {
            break;
        }
        /* fallthrough */
    default:
        if (arg1->unk0.unk308 & 0x80) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xB1DU, 0x7FU, NULL, NULL, 0, NULL);
        }
        if ((arg1->unk0.unk310 & 0x8000) && (temp_s0->unk3CC.unk0 >= 4)) {
            return -0x19;
        }
        if (arg0->animProgress == 1.0f) {
            if (arg1->unk0.unk284 < -5.0f) {
                temp_s0->unk3CC.unk2 = 0;
                sp94 = -((sp9C * 0.01f) + 0.025f);
                if (_bss_200 < 2) {
                    _bss_200 += 2;
                    sp98 = 0.99f;
                }
            } else {
                temp_s0->unk3CC.unk0 += 1;
                temp_s0->unk3CC.unk2 = 1;
                sp94 = 0.0f;
                if (_bss_200 < 2) {
                    _bss_200 ^= 1;
                }
                temp_s0->unk3CC.unk14 = arg0->srt.transl.y + temp_s0->unk3CC.unk1C;
                temp_s0->unk3CC.unk10 = temp_s0->unk3CC.unk8 + (temp_s0->unk3CC.unk0 * temp_s0->unk3CC.unkC);
                arg0->srt.transl.y = temp_s0->unk3CC.unk14;
            }
        }
        if (arg0->animProgress == 0.0f) {
            if (arg1->unk0.unk284 > 5.0f) {
                temp_s0->unk3CC.unk2 = 1;
                sp98 = 0.0f;
                if (temp_s0->unk3CC.unk0 >= (temp_s0->unk3CC.unk1 - 3)) {
                    sp94 = 0.011f;
                    if (_bss_200 & 1) {
                        _bss_200 = 7;
                    } else {
                        _bss_200 = 6;
                    }
                    _bss_204 = arg0->srt.transl.y;
                    _bss_208 = *_bss_1B0 + temp_s0->unk3CC.unk4;
                    gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0x1E, 0xFF);
                    break;
                }
                sp94 = (sp9C * 0.012f) + 0.025f;
                if (_bss_200 >= 2) {
                    if (_bss_200 & 1) {
                        _bss_200 = 1;
                    } else {
                        _bss_200 = 0;
                    }
                }
            } else if (arg1->unk0.unk284 < -5.0f) {
                temp_s0->unk3CC.unk2 = 0;
                temp_s0->unk3CC.unk0 -= 1;
                if (temp_s0->unk3CC.unk0 <= 0) {
                    sp98 = 0.0f;
                    sp94 = 0.016f;
                    if (_bss_200 & 1) {
                        _bss_200 = 0xB;
                    } else {
                        _bss_200 = 0xA;
                    }
                    gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0x1E, 0xFF);
                    arg0->srt.transl.y = temp_s0->unk3CC.unk8;
                    break;
                }
                sp98 = 0.99f;
                if (_bss_200 & 1) {
                    _bss_200 = 2;
                } else {
                    _bss_200 = 3;
                }
                sp94 = -((sp9C * 0.01f) + 0.025f);
                temp_s0->unk3CC.unk10 = temp_s0->unk3CC.unk8 + ((f32) temp_s0->unk3CC.unk0 * temp_s0->unk3CC.unkC);
                temp_fv0 = arg0->srt.transl.y - temp_s0->unk3CC.unk1C;
                temp_s0->unk3CC.unk14 = temp_fv0;
                arg0->srt.transl.y = temp_fv0;
            } else {
                if (func_80024E2C(arg0) == 0) {
                    temp_v0_3 = _bss_200 & 1;
                    sp98 = 0.0f;
                    sp94 = 0.01f;
                    if ((temp_v0_3 != 0) && (_bss_200 != 5)) {
                        _bss_200 = 5;
                    } else if ((temp_v0_3 == 0) && (_bss_200 != 4)) {
                        _bss_200 = 4;
                    }
                    break;
                }
            }
        }
        if (sp94 < 0.0f) {
            sp94 = -((sp9C * 0.01f) + 0.025f);
        } else if (sp94 > 0.0f) {
            sp94 = (sp9C * 0.012f) + 0.025f;
        }
        if (temp_s0->unk3CC.unk2 != 0) {
            arg0->srt.transl.y = temp_s0->unk3CC.unk14 + ((temp_s0->unk3CC.unk10 - temp_s0->unk3CC.unk14) * arg0->animProgress);
        } else {
            arg0->srt.transl.y = temp_s0->unk3CC.unk14 + ((temp_s0->unk3CC.unk10 - temp_s0->unk3CC.unk14) * (1.0f - arg0->animProgress));
        }
        break;
    }

    arg1->unk0.animTickDelta = sp94;
    if (_bss_202 != _bss_200) {
        func_80023D30(arg0, _data_5A0[_bss_200], sp98, 1U);
        if ((_bss_200 < 2) && (temp_s0->unk3CC.unk3 == 0)) {
            func_8001A3FC(sp60, 0, 0, 0, arg0->srt.scale, &sp78, &sp54);
            func_8001A3FC(sp60, 0, 0, 1, arg0->srt.scale, &sp6C, &sp54);
            temp_s0->unk3CC.unk1C = sp6C.y - sp78.y;
            temp_s0->unk3CC.unk3 = 1;
        }
    }
    sp90 = arg0->srt.transl.x;
    sp8C = arg0->srt.transl.y;
    sp88 = arg0->srt.transl.z;
    switch (_bss_200) {
    case 0:
    case 1:
    case 2:
    case 3:
        sp8C = (temp_s0->unk3CC.unk8 + ((f32) (temp_s0->unk3CC.unk0 + 1) * temp_s0->unk3CC.unkC));
        sp8C = arg0->srt.transl.y + ((sp8C - arg0->srt.transl.y) * arg0->animProgress);
        break;
    case 10:
    case 11:
        sp90 = arg0->srt.transl.x + ((temp_s0->unk7EC.x - arg0->srt.transl.x) * arg0->animProgress);
        sp8C = arg0->srt.transl.y + ((temp_s0->unk3CC.unk10 - arg0->srt.transl.y) * (1.0f - arg0->animProgress));
        sp88 = arg0->srt.transl.z + ((temp_s0->unk7EC.z - arg0->srt.transl.z) * arg0->animProgress);
        break;
    case 6:
    case 7:
        sp90 = arg0->srt.transl.x + ((temp_s0->unk7EC.x - arg0->srt.transl.x) * arg0->animProgress);
        sp8C = arg0->srt.transl.y + ((temp_s0->unk3CC.unk4 - arg0->srt.transl.y) * arg0->animProgress);
        sp88 = arg0->srt.transl.z + ((temp_s0->unk7EC.z - arg0->srt.transl.z) * arg0->animProgress);
        break;
    default:
        sp8C = arg0->srt.transl.y;
        break;
    }
    gDLL_2_Camera->vtbl->func10(sp90, sp8C, sp88);
    func_8004D844(arg0, sp90, sp8C, sp88);
    dll_210_func_7260(arg0, temp_s0);
    return 0;
}
#endif

// offset: 0x1034C | func: 84
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1034C.s")
#else
// Matches but has static var
s32 dll_210_func_1034C(Object* arg0, Player_Data* arg1, f32 arg2) {
    static f32 _bss_38;
    f32 sp6C;
    Player_Data* temp_s2;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 var_fv1;

    temp_s2 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        _bss_38 = 0.0f;
        func_80023D30(arg0, 0x35, 0.0f, 1U);
        arg1->unk0.animTickDelta = 0.025f;
        temp_s2->unk3CC.unk1C = arg0->srt.transl.y;
        arg0->srt.transl.y = temp_s2->unk7EC.y;
        dll_210_func_7260(arg0, temp_s2);
        arg1->unk0.unk270 = 0xD;
    }
    if (!(arg1->unk0.unk4.unk25C & 0x10) && (arg1->unk0.unk4.underwaterDist > 5.0f)) {
        return 0x20;
    }
    arg1->unk0.unk4.mode = 2;
    arg1->unk0.flags |= 0x200000;
    switch (arg0->curModAnimId) {
    case 0x35:
        if (arg1->unk0.unk33A != 0) {
            func_80023D30(arg0, 0x36, 0.0f, 0U);
            arg1->unk0.animTickDelta = 0.025f;
        }
        /* fallthrough */
    case 0x36:
        sp6C = -_bss_38 * 10.0f;
        if (arg1->unk0.unk308 & 1) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x769U, 0x7FU, NULL, NULL, 0, NULL);
        }
        var_fv1 = arg0->srt.transl.y - (temp_s2->unk3CC.unk8 + 32.0f);
        if (var_fv1 < 0.0f) {
            var_fv1 = 0.0f;
        }
        if (var_fv1 < sp6C) {
            temp_fv0 = SQ(_bss_38) / (2.0f * sp6C);
            temp_fv1 = -sqrtf(2.0f * (temp_fv0) * var_fv1);
            arg0->speed.y = temp_fv1;
            if (temp_fv1 >= -0.01f) {
                func_80023D30(arg0, 0x37, 0.0f, 1U);
                gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0, 0xFF);
                arg1->unk0.animTickDelta = 0.02f;
                temp_s2->unk3CC.unk1C = arg0->srt.transl.y;
                arg0->srt.transl.y = temp_s2->unk3CC.unk8;
                dll_210_func_7260(arg0, temp_s2);
                arg0->speed.y = 0.0f;
            }
        } else {
            if (arg0->speed.y > -3.0f) {
                arg0->speed.y = arg0->speed.y - (0.05f * arg2);
            }
            if (arg0->speed.y < -3.0f) {
                arg0->speed.y = -3.0f;
            }
            if (arg0->speed.y < _bss_38) {
                _bss_38 = arg0->speed.y;
            }
        }
        break;
    case 0x37:
        if (arg1->unk0.unk308 & 1) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, (u16) temp_s2->unk898[func_80025CD4((s32) arg1->unk0.unk4.unk68.unk50[0])], 0x7FU, NULL, NULL, 0, NULL);
        }
        if (arg1->unk0.unk33A != 0) {
            arg0->positionMirror.x = temp_s2->unk7EC.x;
            arg0->positionMirror.z = temp_s2->unk7EC.z;
            inverse_transform_point_by_object(arg0->positionMirror.x, 0.0f, arg0->positionMirror.z, arg0->srt.transl.f, &sp5C, &arg0->srt.transl.z, arg0->parent);
            dll_210_func_7260(arg0, temp_s2);
            func_80023D30(arg0, (s32) *temp_s2->modAnims, 0.0f, 1U);
            return 2;
        }
        break;
    }

    sp58 = arg0->srt.transl.x;
    sp54 = arg0->srt.transl.y;
    sp50 = arg0->srt.transl.z;
    switch (arg0->curModAnimId) {
        default:
            sp54 = arg0->srt.transl.y;
            break;
        case 0x35:
            sp54 = temp_s2->unk3CC.unk1C + ((arg0->srt.transl.y - temp_s2->unk3CC.unk1C) * arg0->animProgress);
            break;
        case 0x37:
            sp58 = arg0->srt.transl.x + ((temp_s2->unk7EC.x - arg0->srt.transl.x) * arg0->animProgress);
            sp54 = arg0->srt.transl.y + ((temp_s2->unk3CC.unk1C - arg0->srt.transl.y) * (1.0f - arg0->animProgress));
            sp50 = arg0->srt.transl.z + ((temp_s2->unk7EC.z - arg0->srt.transl.z) * arg0->animProgress);
            break;
    }
    gDLL_2_Camera->vtbl->func10(sp58, sp54, sp50);
    func_8004D844(arg0, sp58, sp54, sp50);
    return 0;
}
#endif

// offset: 0x10898 | func: 85
s32 dll_210_func_10898(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* temp_s1;
    f32 sp3C[2];

    temp_s1 = arg0->data;
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    arg0->speed.y = 0.0f;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.animExitAction = dll_210_func_12514;
        arg1->unk0.unk270 = 0xD;
    }
    if (arg0->curModAnimId == 0x41A) {
        if (arg1->unk0.unk33A != 0) {
            return -0x1C;
        }
    } else {
        sp3C[0] = temp_s1->unk430.unk4;
        sp3C[1] = temp_s1->unk430.unk8;
        gDLL_2_Camera->vtbl->func6(0x5D, 1, 1, 8, &sp3C, 0, 0xFF);
        func_80023D30(arg0, 0x41A, 0.0f, 1U);
        arg0->srt.yaw = arctan2_f(temp_s1->unk430.unk24.x, temp_s1->unk430.unk24.z);
        arg0->srt.transl.f[0] = temp_s1->unk430.unk44.x;
        arg0->srt.transl.f[1] = temp_s1->unk7EC.f[1];
        arg0->srt.transl.f[2] = temp_s1->unk430.unk44.z;
        arg1->unk0.animTickDelta = 0.035f;
    }
    return 0;
}

// offset: 0x10A0C | func: 86
s32 dll_210_func_10A0C(Object* arg0, Player_Data* arg1, f32 arg2) {
    s32 sp8C;
    Player_Data* temp_s0;
    f32 temp_fa0;
    f32 temp_fv0;
    s16 pad_sp7E;
    s16 sp7C;
    Vec3f sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 temp_fv1;
    u32 sp5C;
    f32 sp54[2];
    s32 var_v1;
    s16* var_v0;

    temp_s0 = arg0->data;
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    arg0->speed.y = 0.0f;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x1B;
        arg1->unk0.animExitAction = dll_210_func_12514;
        temp_s0->unk8A9 = 1;
    }
    sp8C = temp_s0->unk430.unk1;
    if (sp8C != 0) {
        arg1->unk0.animTickDelta = 0.01f;
    } else {
        arg1->unk0.animTickDelta = 0.0145f;
    }
    if ((arg1->unk0.unk4.underwaterDist > 25.0f) && (arg1->unk0.unk4.floorDist < 100.0f)) {
        return 0x21;
    }
    if (arg1->unk0.unk308 & 1) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, temp_s0->unk3B8[rand_next(0xA, 0xE)], 0x7FU, NULL, NULL, 0, NULL);
    }
    if (arg1->unk0.unk308 & 0x80) {
        sp5C = gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_4D8[temp_s0->unk430.unk2], rand_next(0x5A, 0x7F), NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(sp5C, ((f32) rand_next(-0x64, 0x64) * 0.001f) + 1.0f);
    }
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk278 = 0.0f;
        arg1->unk0.unk27C = 0.0f;
        arg0->srt.yaw = arctan2_f(temp_s0->unk430.unk24.x, temp_s0->unk430.unk24.z);
        arg0->srt.transl.x = temp_s0->unk430.unk44.x;
        arg0->srt.transl.z = temp_s0->unk430.unk44.z;
        if (sp8C != 0) {
            var_v0 = _data_6F4;
        } else {
            var_v0 = _data_6F0;
        }
        var_v1 = 0x25;
        if (sp8C != 0) {
            var_v1 = 0x65;
        }
        temp_s0->unk430.unk5C = dll_210_func_8AE0(arg0, var_v0[0], var_v0[1], &temp_s0->unk430.unk50, &temp_s0->unk430.unk24, 0.0f, 0.0f, 2U, var_v1);
        func_8001A3FC(arg0->modelInsts[arg0->modelInstIdx], 0U, 0, 1.0f, arg0->srt.scale, &sp70, &sp7C);
        temp_s0->unk430.unk18.y = 0.0f;
        temp_s0->unk430.unk18.x = sp70.y;
        temp_s0->unk430.unk18.z = 0.0f;
        sp54[0] = temp_s0->unk430.unk4;
        sp54[1] = temp_s0->unk430.unk8;
        gDLL_2_Camera->vtbl->func6(0x5D, 1, 1, 8, &sp54, 0, 0);
    } else if (arg1->unk0.unk33A != 0) {
        return 0x1D;
    }
    func_80024DD0(arg0, 0, 1, temp_s0->unk430.unk5C);
    sp6C = arg0->srt.transl.x + (temp_s0->unk430.unk18.y * arg0->animProgress);
    sp68 = arg0->srt.transl.y + (temp_s0->unk430.unk18.x * arg0->animProgress);
    sp64 = arg0->srt.transl.z + (temp_s0->unk430.unk18.z * arg0->animProgress);
    gDLL_2_Camera->vtbl->func10(arg0->srt.transl.x, sp68, arg0->srt.transl.z);
    func_8004D844(arg0, sp6C, sp68, sp64);
    dll_210_func_7260(arg0, arg0->data);
    return 0;
}

// offset: 0x10E94 | func: 87
s32 dll_210_func_10E94(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 sp124;
    Vec3f sp118;
    f32 sp114;
    f32 sp110;
    f32 sp10C;
    f32 sp108;
    Player_Data* temp_s0;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 var_fa0;
    f32 spF4;
    f32 spF0;
    s32 pad;
    s16 pad_spEA;
    s16 spE8;
    s16 spE6;
    s16 pad_spE4;
    ModelInstance* spE0;
    f32 var_fv1_2;
    f32 var_fv0;
    s32 spD4;
    s32 spD0;
    s32 spCC;
    s32 spC8;
    s32 spC4;
    s32 spC0;
    u32 spBC;
    Vec3f spB0;
    Vec3f spA4;
    Func_80059C40_Struct sp50;

    if (arg1->unk0.enteredAnimState) {
        _bss_200 = 0x10;
        arg1->unk0.unk270 = 0xD;
        arg1->unk0.animExitAction = dll_210_func_12514;
    }
    if ((arg1->unk0.unk4.underwaterDist > 25.0f) && (arg1->unk0.unk4.floorDist < 100.0f)) {
        return 0x21;
    }
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    // @fake
    if ((_bss_200 && _bss_200) && _bss_200) {}
    arg0->speed.y = 0.0f;
    spE0 = arg0->modelInsts[arg0->modelInstIdx];
    temp_s0 = arg0->data;
    sp124 = arg1->unk0.animTickDelta;
    _bss_202 = _bss_200;
    switch (_bss_200) {
    case 16:
        if (arg0->curModAnimId == 0x66) {
            temp_s0->unk430.unk5E = 0;
            _bss_200 = 0x16;
        } else {
            temp_s0->unk430.unk5E = 1;
            _bss_200 = 0x15;
        }
        arg0->srt.transl.f[1] = temp_s0->unk7EC.y;
        sp124 = 0.006f;
        /* fallthrough */
    case 21:
    case 22:
        temp_s0->unk430.unk18.f[1] = 0.0f;
        temp_s0->unk430.unk18.f[0] = 0.0f;
        temp_s0->unk430.unk18.f[2] = 0.0f;
        if (!(arg1->unk0.unk290 <= 0.1f)) {
            sp114 = arg0->animProgress;
            arg0->animProgress = 1.0f;
        } else {
            break;
        }
    default:
        if (arg0->animProgress == 1.0f) {
            spB0.f[0] = temp_s0->unk7EC.f[0] - (temp_s0->unk430.unk24.f[0] * 30.0f);
            spB0.y = temp_s0->unk7EC.y;
            spB0.f[2] = temp_s0->unk7EC.f[2] - (temp_s0->unk430.unk24.f[2] * 30.0f);
            if (func_80059C40(&temp_s0->unk7EC, &spB0, 0.0f, 3, &sp50, arg0, arg1->unk0.unk4.unk259, 3, 0xFFU, 0) != 0) {
                arg0->srt.transl.f[0] = spB0.f[0];
                arg0->srt.transl.f[2] = spB0.f[2];

                temp_s0->unk430.unk4 = sp50.unk38.f[1] + ((sp50.unk38.f[2] - sp50.unk38.f[1]) * sp50.unk48);
                temp_s0->unk430.unk8 = ((sp50.unk10 - sp50.unkC) * sp50.unk48) + sp50.unkC;

                temp_s0->unk430.unk24.x = sp50.unk1C.x;
                temp_s0->unk430.unk24.y = sp50.unk1C.y;
                temp_s0->unk430.unk24.z = sp50.unk1C.z;
                temp_s0->unk430.unk24.w = sp50.unk1C.w;

                temp_s0->unk430.unk34.x = -sp50.unk1C.z;
                temp_s0->unk430.unk34.y = 0.0f;
                temp_s0->unk430.unk34.z = sp50.unk1C.x;
                temp_s0->unk430.unk34.w = -((temp_s0->unk430.unk34.z * spB0.z) + ((spB0.x * temp_s0->unk430.unk34.x) + (spB0.y * temp_s0->unk430.unk34.y)));

                arg0->srt.yaw = arctan2_f(temp_s0->unk430.unk24.x, temp_s0->unk430.unk24.z);
                if (sp50.unk52 & 4) {
                    spC0 = 0;
                } else if (sp50.unk52 & 8) {
                    spC0 = 1;
                } else {
                    if (sp50.unk52 & 2) {
                        spC0 = 2;
                    } else {
                        spC0 = 3;
                    }
                }
            } else {
                spC0 = 2;
            }
            if ((_bss_200 != 0x15) && (_bss_200 != 0x16)) {
                arg0->srt.transl.f[1] = temp_s0->unk7EC.y;
            }
            if (arg1->unk0.unk290 > 0.1f) {
                _bss_200 = ((arctan2_f(arg1->unk0.unk288, -arg1->unk0.unk284) + 0x1000) >> 0xD) & 7;
                _bss_202 = -1;
                if ((_bss_200 == 4) || (_bss_200 == 0)) {
                    temp_s0->unk430.unk5E ^= 1;
                }
                spD0 = 0;
                spCC = 0;
                spC8 = 0;
                spC4 = 0;
                switch (_bss_200) {
                case 4:
                    spD0 = 1;
                    break;
                case 0:
                    spCC = 1;
                    break;
                case 6:
                    spC8 = 1;
                    break;
                case 2:
                    spC4 = 1;
                    break;
                case 3:
                    spD0 = 1;
                    spC4 = 1;
                    break;
                case 5:
                    spD0 = 1;
                    spC8 = 1;
                    break;
                case 1:
                    spCC = 1;
                    spC4 = 1;
                    break;
                case 7:
                    spCC = 1;
                    spC8 = 1;
                    break;
                }
                if (temp_s0->unk430.unk5E != 0) {
                    _bss_200 += 8;
                }
                if (spD0 != 0) {
                    var_fa0 = _bss_1B0[12];
                    var_fv0 = temp_s0->unk430.unk4 - temp_s0->unk7EC.y;
                    if (var_fa0 < 0.0f) {
                        var_fa0 = -var_fa0;
                    }
                    var_fv1_2 = _bss_1B0[13];
                    if (var_fv1_2 < 0.0f) {
                        var_fv1_2 = -var_fv1_2;
                    }
                    if ((var_fv0 < var_fv1_2) && ((spC0 == 0) || (spC0 == 3))) {
                        var_fv0 = (var_fv0 - var_fa0) / (var_fv1_2 - var_fa0);
                        if (var_fv0 < 0.0f) {
                            var_fv0 = 0.0f;
                        } else if (var_fv0 > 1.0f) {
                            var_fv0 = 1.0f;
                        }
                        temp_s0->unk430.unk5C = var_fv0 * 1023.0f;
                        temp_s0->unk430.unk18.f[0] = var_fv0;
                        return 0x1E;
                    }
                } else if (spCC != 0) {
                    var_fv0 = temp_s0->unk7EC.y - temp_s0->unk430.unk8;
                    var_fa0 = _bss_1B0[14];
                    if (var_fa0 < 0.0f) {
                        var_fa0 = -var_fa0;
                    }
                    var_fv1_2 = _bss_1B0[15];
                    if (var_fv1_2 < 0.0f) {
                        var_fv1_2 = -var_fv1_2;
                    }
                    if ((var_fv0 < var_fv1_2) && ((spC0 == 1) || (spC0 == 3))) {
                        var_fv0 = (var_fv0 - var_fa0) / (var_fv1_2 - var_fa0);
                        if (var_fv0 < 0.0f) {
                            var_fv0 = 0.0f;
                        } else if (var_fv0 > 1.0f) {
                            var_fv0 = 1.0f;
                        }
                        temp_s0->unk430.unk5C = var_fv0 * 1023.0f;
                        temp_s0->unk430.unk18.f[0] = var_fv0;
                        return 0x1F;
                    }
                }
                func_80024E50(arg0, _data_5BC[_bss_200], 0.0f, 1U);
                func_8001A3FC(spE0, 1U, 0, 1.0f, arg0->srt.scale, &sp118, &spE8);
                arg0->curModAnimIdLayered = -1;
                temp_s0->unk430.unk18.y = -sp118.x * temp_s0->unk430.unk34.x;
                temp_s0->unk430.unk18.x = sp118.y;
                temp_s0->unk430.unk18.z = -sp118.x * temp_s0->unk430.unk34.z;
                if ((spD0 == 0) && (spCC == 0)) {
                    temp_s0->unk430.unk18.f[0] = 0.f;
                }
                if ((spC8 == 0) && (spC4 == 0)) {
                    temp_s0->unk430.unk18.f[1] = 0.f;
                    temp_s0->unk430.unk18.f[2] = 0.f;
                }
                spD4 = 0;
                temp_fa0 = temp_s0->unk430.unk34.f[2];
                temp_fa1 = temp_s0->unk430.unk34.f[0];
                if (sp118.f[0] < 0.f) {
                    spF4 = temp_fa1 * 7.0f;
                    spF0 = temp_fa0 * 7.0f;
                } else {
                    spF4 = -temp_fa1 * 7.0f;
                    spF0 = -temp_fa0 * 7.0f;
                }
                if ((spD0 != 0) || (spCC != 0)) {
                    spB0.f[1] = sp118.f[1] + temp_s0->unk7EC.y;
                    if (sp118.f[1] < 0/*.0f*/) {
                        spB0.f[1] -= 11.0f;
                    } else {
                        spB0.f[1] += 11.0f;
                    }
                    for (spE6 = 0; spE6 < 2; spE6++) {
                        if (spE6 != 0) {
                            spB0.f[0] = temp_s0->unk7EC.f[0] + spF4;
                            spB0.f[2] = temp_s0->unk7EC.f[2] + spF0;
                        } else {
                            spB0.f[0] = temp_s0->unk7EC.f[0] - spF4;
                            spB0.f[2] = temp_s0->unk7EC.f[2] - spF0;
                        }
                        spA4.f[0] = spB0.f[0] - (temp_s0->unk430.unk24.f[0] * 30.0f);
                        spA4.f[1] = spB0.f[1];
                        spA4.f[2] = spB0.f[2] - (temp_s0->unk430.unk24.f[2] * 30.0f);
                        if (func_80059C40(&spB0, &spA4, 0/*.0f*/, 3, NULL, arg0, arg1->unk0.unk4.unk259, 3, 0xFFU, 0) != 0) {
                            spD4 |= 1 << spE6;
                        }
                    }
                } else {
                    spD4 = 3;
                }
                if ((spC8 != 0) || (spC4 != 0)) {
                    spB0.f[0] = temp_s0->unk7EC.x + temp_s0->unk430.unk18.y + spF4;
                    spB0.f[2] = temp_s0->unk7EC.z + temp_s0->unk430.unk18.z + spF0;
                    for (spE6 = 0; spE6 < 2; spE6++) {
                        if (spE6 != 0) {
                            spB0.f[1] = temp_s0->unk7EC.y + 11.0f;
                        } else {
                            spB0.f[1] = temp_s0->unk7EC.y - 11.0f;
                        }
                        spA4.f[0] = spB0.f[0] - (temp_s0->unk430.unk24.f[0] * 30.0f);
                        spA4.f[1] = spB0.f[1];
                        spA4.f[2] = spB0.f[2] - (temp_s0->unk430.unk24.f[2] * 30.0f);
                        if (func_80059C40(&spB0, &spA4, 0.0f, 3, NULL, arg0, arg1->unk0.unk4.unk259, 3, 0xFFU, 0) != 0) {
                            spD4 |= 1 << (spE6 + 2);
                        }
                    }
                } else {
                    spD4 |= 0xC;
                }
                sp124 = 0.02f;
                if (spD4 != 0xF) {
                    temp_s0->unk430.unk18.f[1] = 0;
                    temp_s0->unk430.unk18.f[0] = 0;
                    temp_s0->unk430.unk18.f[2] = 0;
                    if ((_bss_200 == 4) || (_bss_200 == 0) || (((_bss_200 == 0xC) | (_bss_200 == 8)) != 0)) {
                        temp_s0->unk430.unk5E ^= 1;
                    }
                    if (temp_s0->unk430.unk5E != 0) {
                        _bss_200 = 0x15;
                    } else {
                        _bss_200 = 0x16;
                    }
                    if (arg0->curModAnimId == _data_5E4[1] || arg0->curModAnimId == _data_5E8[0]) {
                        _bss_202 = _bss_200;
                        arg0->animProgress = sp114;
                    }
                    sp124 = 0.006f;
                }
            } else {
                arg0->srt.transl.f[1] = temp_s0->unk7EC.y;
                sp124 = 0.006f;
                if (temp_s0->unk430.unk5E != 0) {
                    _bss_200 = 0x15;
                } else {
                    _bss_200 = 0x16;
                }
                // @fake
                if (1) {}
            }
        }
        if (_bss_200 != 0x15 && _bss_200 != 0x16) {
            var_fv0 = arg1->unk0.unk290;
            if (sp124 < 0.0f) {
                sp124 = -((var_fv0 * 0.003999997f) + 0.034f);
            } else if (sp124 > 0.0f) {
                sp124 = (var_fv0 * 0.003999997f) + 0.034f;
            }
        }
        if (arg1->unk0.unk308 & 0x80) {
            spBC = gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_4D8[temp_s0->unk430.unk2], rand_next(0x5A, 0x7F), NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->func_954(spBC, (rand_next(-0x64, 0x64) * 0.001f) + 1.0f);
        }
        break;
    }

    arg1->unk0.animTickDelta = sp124;
    if (_bss_202 != _bss_200) {
        func_80023D30(arg0, _data_5BC[_bss_200], 0.0f, 1U);
    }
    sp110 = arg0->srt.transl.x + (temp_s0->unk430.unk18.y * arg0->animProgress);
    sp10C = arg0->srt.transl.y + (temp_s0->unk430.unk18.x * arg0->animProgress);
    sp108 = arg0->srt.transl.z + (temp_s0->unk430.unk18.z * arg0->animProgress);
    gDLL_2_Camera->vtbl->func10(sp110, sp10C, sp108);
    func_8004D844(arg0, sp110, sp10C, sp108);
    dll_210_func_7260(arg0, arg0->data);
    return 0;
}

// offset: 0x11C60 | func: 88
s32 dll_210_func_11C60(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* temp_s1;
    Vec3f sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    s32 temp_v0;
    s16 pad_sp4E;
    s16 sp48;
    u32 sp44;

    temp_s1 = arg0->data;
    if (arg1->unk0.unk308 & 0x80) {
        sp44 = gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_4D8[temp_s1->unk430.unk2], rand_next(0x5A, 0x7F), NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(sp44, ((f32) rand_next(-0x64, 0x64) * 0.001f) + 1.0f);
    }
    if (arg1->unk0.enteredAnimState != 0) {
        gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0x3C, 0xFF);
        gDLL_6_AMSFX->vtbl->play_sound(arg0, temp_s1->unk3B8[rand_next(0xA, 0xE)], 0x7FU, NULL, NULL, 0, NULL);
        func_80023D30(arg0, _data_5DC[1], 0.0f, 1U);
        func_80025540(arg0, _data_5E0[0], 0);
        arg1->unk0.animTickDelta = 0.012f;
        func_8001A3FC(arg0->modelInsts[arg0->modelInstIdx], 0U, 0, 1.0f, arg0->srt.scale, &sp60, &sp48);
        temp_s1->unk430.unk18.y = sp60.f[2] * temp_s1->unk430.unk24.x;
        temp_s1->unk430.unk18.z = sp60.f[2] * temp_s1->unk430.unk24.z;
        arg0->srt.transl.y = temp_s1->unk430.unk4;
        arg1->unk0.unk270 = 0x1D;
        arg1->unk0.animExitAction = dll_210_func_12514;
    }
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    arg0->speed.y = 0.0f;
    func_80024DD0(arg0, 0, 1, temp_s1->unk430.unk5C);
    if (arg0->animProgress > 0.99f) {
        arg0->positionMirror.x = temp_s1->unk7EC.x;
        arg0->positionMirror.z = temp_s1->unk7EC.z;
        inverse_transform_point_by_object(arg0->positionMirror.x, 0.0f, arg0->positionMirror.z, arg0->srt.transl.f, &sp50, &arg0->srt.transl.z, arg0->parent);
        dll_210_func_7260(arg0, temp_s1);
        func_80023D30(arg0, (s32) *temp_s1->modAnims, 0.0f, 1U);
        return -1;
    }
    sp5C = arg0->srt.transl.x + (temp_s1->unk430.unk18.y * arg0->animProgress);
    sp58 = arg0->srt.transl.y - (temp_s1->unk430.unk18.x * (1.0f - arg0->animProgress));
    sp54 = arg0->srt.transl.z + (temp_s1->unk430.unk18.z * arg0->animProgress);
    gDLL_2_Camera->vtbl->func10(sp5C, sp58, sp54);
    func_8004D844(arg0, sp5C, sp58, sp54);
    dll_210_func_7260(arg0, temp_s1);
    return 0;
}

// offset: 0x1209C | func: 89
s32 dll_210_func_1209C(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 temp_fv0;
    Vec3f sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    Player_Data* temp_s1;
    s16 pad_sp4A;
    s16 sp48;
    u32 sp44;

    temp_s1 = arg0->data;
    if (!(arg1->unk0.unk4.unk25C & 0x10) && (arg1->unk0.unk4.underwaterDist > 5.0f)) {
        return 0x20;
    }
    if (arg1->unk0.unk308 & 1) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, temp_s1->unk3B8[rand_next(0xA, 0xE)], 0x7FU, NULL, NULL, 0, NULL);
    }
    if (arg1->unk0.unk308 & 0x80) {
        sp44 = gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_4D8[temp_s1->unk430.unk2], rand_next(0x5A, 0x7F), NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(sp44, ((f32) rand_next(-0x64, 0x64) * 0.001f) + 1.0f);
    }
    if (arg1->unk0.enteredAnimState != 0) {
        gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0x3C, 0xFF);
        func_80023D30(arg0, _data_5E0[1], 0.0f, 1U);
        func_80025540(arg0, (s32) *_data_5E4, 0);
        arg1->unk0.animTickDelta = 0.015f;
        func_8001A3FC(arg0->modelInsts[arg0->modelInstIdx], 0U, 0, 1.0f, arg0->srt.scale, &sp60, &sp48);
        temp_s1->unk430.unk18.y = sp60.f[2] * temp_s1->unk430.unk24.x;
        temp_s1->unk430.unk18.z = sp60.f[2] * temp_s1->unk430.unk24.z;
        arg0->srt.transl.y = temp_s1->unk430.unk8;
        arg1->unk0.unk270 = 0x1E;
        arg1->unk0.animExitAction = dll_210_func_12514;
    }
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0 != 0) { return temp_v0; }
    }
    arg0->speed.y = 0.0f;
    func_80024DD0(arg0, 0, 1, temp_s1->unk430.unk5C);
    if (arg0->animProgress > 0.99f) {
        arg0->positionMirror.x = temp_s1->unk7EC.x;
        arg0->positionMirror.z = temp_s1->unk7EC.z;
        inverse_transform_point_by_object(arg0->positionMirror.x, 0.0f, arg0->positionMirror.z, arg0->srt.transl.f, &sp50, &arg0->srt.transl.z, arg0->parent);
        dll_210_func_7260(arg0, temp_s1);
        func_80023D30(arg0, (s32) *temp_s1->modAnims, 0.0f, 1U);
        return -1;
    }
    sp5C = arg0->srt.transl.x + (temp_s1->unk430.unk18.y * arg0->animProgress);
    sp58 = arg0->srt.transl.y - (temp_s1->unk430.unk18.x * (1.0f - arg0->animProgress));
    sp54 = arg0->srt.transl.z + (temp_s1->unk430.unk18.z * arg0->animProgress);
    gDLL_2_Camera->vtbl->func10(sp5C, sp58, sp54);
    func_8004D844(arg0, sp5C, sp58, sp54);
    dll_210_func_7260(arg0, temp_s1);
    return 0;
}

// offset: 0x12514 | func: 90
static void dll_210_func_12514(Object* arg0, ObjFSA_Data *arg1) {
    Player_Data *objdata = arg0->data;

    if ((objdata->unk818 > 0.0f) && (gDLL_2_Camera->vtbl->func3() != 0x54)) {
        gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0, 0xFF);
    }
}

// offset: 0x125BC | func: 91
s32 dll_210_func_125BC(Object* arg0, Player_Data* arg1, u32 arg2) {
    f32 temp_fs0;
    f32 temp_fs1;
    f32 f2;
    f32 f0;
    s32 i;
    Player_Data* temp_s3;

    if (arg1->unk0.enteredAnimState != 0) {
        ((s16*)arg1)[0x138] = 0x1F;
    }
    arg1->unk0.flags |= 0x200000;
    temp_s3 = (Player_Data *) &arg1->unk0.unk4;
    if (arg1->unk0.enteredAnimState != 0) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x3D8U, 0x7FU, NULL, NULL, 0, NULL);
        for (i = 0; i < 3; i++) {
            temp_fs0 = ((f32) rand_next(-0x32, 0x32) / 10.0f) + arg0->srt.transl.x;
            temp_fs1 = ((f32) rand_next(-0x32, 0x32) / 10.0f) + arg0->srt.transl.z;
            gDLL_24_Waterfx->vtbl->func_174C(temp_fs0, temp_s3->unk0.unk4.floorY, temp_fs1, 4.0f);
            gDLL_24_Waterfx->vtbl->func_1CC8(temp_fs0, temp_s3->unk0.unk4.floorY, temp_fs1, 0, 0.0f, 3);
        }
    }
    if (temp_s3->unk0.unk4.unk1AC > 25.0f && temp_s3->unk0.unk4.floorNormalZ < 100.0f) {
        return 0x21;
    }
    if ((s8)temp_s3->unk0.unk4.numTestPoints & 0x10) {
        return 2;
    }
    f0 = temp_s3->unk0.unk4.floorY - 6.0f;
    f2 = f0 - arg0->srt.transl.y;
    if (f2 > 25.0f) {
        f2 = 25.0f;
    }
    arg0->speed.y += (f2 / 25.0f) * 0.13f * gUpdateRateF;
    arg0->speed.y -= 0.1f * gUpdateRateF;
    arg0->speed.y *= 0.96f;
    if (arg0->speed.y > 1.4f) {
        arg0->speed.y = 1.4f;
    }
    arg0->speed.x *= 0.98f;
    arg0->speed.z *= 0.98f;
    for (i = 0; i < 4; i++) {
        gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_202, NULL, PARTFXFLAG_NONE, -1, NULL);
    }
    return 0;
}

// offset: 0x128F4 | func: 92
static void dll_210_func_128F4(f32* arg0, f32* arg1, f32 arg2, Object* arg3) {
    Object* curObj;
    s32 objCount;
    Object** temp_v0;
    s32 pad[2];
    f32 temp_fs0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fs1;
    f32 var_fs2;
    s32 i;
    s32 var_s5;
    Player_Data* objdata;

    objdata = arg3->data;
    var_fs1 = 0.0f;
    var_fs2 = 0.0f;
    temp_v0 = obj_get_all_of_type(OBJTYPE_22, &objCount);
    var_s5 = 0;
    for (i = 0; i < objCount; i++) {
        curObj = temp_v0[i];
        if (((ObjType22Setup*)curObj->setup)->unk1A & 2) {
            var_s5 = 1;
            temp_fv0 = curObj->srt.transl.f[1] - arg3->srt.transl.f[1];
            if ((temp_fv0 <= 200.0f) && (temp_fv0 >= -200.0f)) {
                temp_fs0 = curObj->srt.transl.f[0] - arg3->srt.transl.f[0];
                temp_fv0 = curObj->srt.transl.f[2] - arg3->srt.transl.f[2];
                temp_fs0 = sqrtf(SQ(temp_fs0) + SQ(temp_fv0));
                temp_fv1 = ((ObjType22Setup*)curObj->setup)->unk19 * 1.5f;
                if (temp_fs0 < temp_fv1) {
                    temp_fs0 = ((temp_fv1 - temp_fs0) / temp_fv1);
                    temp_fs0 *= (curObj->srt.scale * 10.0f);
                    var_fs2 += fsin16_precise(curObj->srt.yaw) * temp_fs0;
                    var_fs1 += fcos16_precise(curObj->srt.yaw) * temp_fs0;
                }
            }
        }
    }

    if (var_s5 != 0) {
        var_fs2 /= var_s5;
        var_fs1 /= var_s5;
        objdata->unk674 -= var_fs2 * 0.05f;
        objdata->unk678 -= var_fs1 * 0.05f;
        objdata->unk674 *= 0.99f;
        objdata->unk678 *= 0.99f;
        temp_fv0 = sqrtf(SQ(objdata->unk674) + SQ(objdata->unk678));
        if (temp_fv0 > 0.85f) {
            temp_fv1 = 0.85f / temp_fv0;
            objdata->unk674 *= temp_fv1;
            objdata->unk678 *= temp_fv1;
        }
        *arg0 = objdata->unk674 * arg2;
        *arg1 = objdata->unk678 * arg2;
    } else {
        *arg0 = 0.0f;
        *arg1 = 0.0f;
    }
}

// offset: 0x12BF0 | func: 93
s32 dll_210_func_12BF0(Object* arg0, Player_Data* arg1, f32 arg2) {
    s32 pad;
    DLL27_Data* temp_s0;
    f32 spC4;
    f32 var_fa0;
    f32 var_fv0;
    MtxF sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    s32 i;
    Player_Data* temp_s1;
    SRT sp50;

    temp_s1 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x20;
    }
    arg1->unk0.flags |= 0x200000;
    arg1->unk0.unk340 |= 2;
    if (arg1->unk0.enteredAnimState != 0) {
        temp_s1->unk880 = rand_next(300, 600);
        temp_s1->unk88A = 0;
        func_80023D30(arg0, 0x412, 0.0f, 0U);
        arg1->unk0.animTickDelta = 0.015f;
        if (arg1->unk0.prevAnimState == 0x1F) {
            temp_s1->unk67C = 1U;
        } else {
            temp_s1->unk67C = 0U;
        }
        temp_s1->unk674 = 0.0f;
        temp_s1->unk678 = 0.0f;
        if (arg1->unk0.prevAnimState != 0x21) {
            temp_s1->unk838 = 0.0f;
        }
    }
    temp_s0 = &arg1->unk0.unk4;
    temp_s1->unk838 += gUpdateRateF;
    if ((temp_s1->unk838 > 120.0f) && (temp_s1->flags & 0x80000)) {
        gDLL_3_Animation->vtbl->func17(0xB, arg0, -1);
    } else {
        if ((temp_s0->underwaterDist <= 0.0f) && (temp_s0->floorDist > 20.0f)) {
            return -0xD;
        }
        if ((temp_s0->underwaterDist < 23.0f) && (temp_s0->unk25C & 0x10)) {
            return -1;
        }
        spC4 = temp_s0->waterY - 6.0f;
        spC4 += fsin16_precise(temp_s1->unk88A);
        temp_s1->unk88A += gUpdateRateF * 128.0f;
        var_fa0 = spC4 - arg0->srt.transl.f[1];
        if (var_fa0 > 25.0f) {
            var_fa0 = 25.0f;
        }
        arg0->speed.f[1] += (var_fa0 / 25.0f) * 0.13f * gUpdateRateF;
        arg0->speed.f[1] -= 0.1f * gUpdateRateF;
        arg0->speed.f[1] *= 0.96f;
        if (arg0->speed.f[1] > 1.4f) {
            arg0->speed.f[1] = 1.4f;
        }
        if (arg1->unk0.unk290 > 0.1f) {
            arg0->srt.yaw += ((arg1->unk0.unk32A * arg2) / 24.0f) * 182.0f;
        }
        if (var_fa0 < 25.0f) {
            if (arg1->unk0.unk308 & 0x200) {
                sp50.transl.f[0] = arg0->srt.transl.f[0];
                sp50.transl.f[1] = 0.0f;
                sp50.transl.f[2] = arg0->srt.transl.f[2];
                sp50.yaw = arg0->srt.yaw;
                sp50.scale = 1.0f;
                sp50.pitch = 0;
                sp50.roll = 0;
                matrix_from_srt(&sp7C, &sp50);
                sp78 = (rand_next(-0x14, 0x14) / 10.0f) + 5.0f;
                sp70 = (rand_next(-0x14, 0x14) / 10.0f) + 2.0f;
                vec3_transform(&sp7C, sp78, 0.0f, sp70, &sp78, &sp74, &sp70);
                gDLL_24_Waterfx->vtbl->func_1CC8(sp78, temp_s0->waterY, sp70, 0, 0.0f, 3);
                sp78 = (rand_next(-0x14, 0x14) / 10.0f) + -5.0f;
                sp70 = (rand_next(-0x14, 0x14) / 10.0f) + 2.0f;
                vec3_transform(&sp7C, sp78, 0.0f, sp70, &sp78, &sp74, &sp70);
                gDLL_24_Waterfx->vtbl->func_1CC8(sp78, temp_s0->waterY, sp70, 0, 0.0f, 3);
            }
            if ((arg0->speed.f[1] > -0.1f) && (arg0->speed.f[1] < 0.1f)) {
                temp_s1->unk880 -= gUpdateRate;
                if (temp_s1->unk880 <= 0) {
                    if (rand_next(0, 1) != 0) {
                        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x3ECU, 0x7FU, NULL, NULL, 0, NULL);
                    } else {
                        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x3EDU, 0x7FU, NULL, NULL, 0, NULL);
                    }
                    temp_s1->unk880 = rand_next(0x12C, 0x258);
                }
            }
        }
        if ((arg0->speed.f[1] < 0.0f) && (temp_s1->unk67C != 0)) {
            for (i = 0; i < 4; i++) {
                gDLL_17_partfx->vtbl->spawn(arg0, 0x202, NULL, 0, -1, NULL);
            }
        } else {
            temp_s1->unk67C = 0U;
            for (i = 0; i < 2; i++) {
                gDLL_17_partfx->vtbl->spawn(arg0, 0x201, NULL, 0, -1, NULL);
            }
        }
        dll_210_func_128F4(arg0->speed.f, &arg0->speed.f[2], arg2, arg0);
        arg0->speed.f[0] *= 0.98f;
        arg0->speed.f[2] *= 0.98f;
        if (arg1->unk0.unk290 < 0.05f) {
            arg1->unk0.unk328 = 0;
            arg1->unk0.unk32A = 0;
            arg1->unk0.unk290 = 0.0f;
        }
        var_fv0 = (arg1->unk0.unk290 - 0.4f) / 0.6f;
        if (var_fv0 < 0.0f) {
            var_fv0 = 0.0f;
        }
        if (var_fv0 > 1.0f) {
            var_fv0 = 1.0f;
        }
        arg1->unk0.speed += (((var_fv0 * 1.6f) - arg1->unk0.speed) / 24.0f) * arg2;
        if (arg1->unk0.speed > 1.65f) {
            arg1->unk0.speed = 1.65f;
        }
        if ((arg1->unk0.unk294 >= 0.42000002f) && (arg1->unk0.unk290 >= 0.42000002f) && (*_data_724 <= arg1->unk0.speed)) {
            return 0x22;
        }
    }

    return 0;
}

// offset: 0x13524 | func: 94
s32 dll_210_func_13524(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 temp_fs0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fs1;
    f32 var_fv0;
    f32 sp120;
    DLL27_Data* dll27data;
    Player_Data* objdata;
    f32 sp114;
    f32 sp110;
    f32 sp10C;
    MtxF spCC;
    s32 i;
    SRT spB0;
    Player_Data3B4 playerdata;

    objdata = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x21;
    }
    if ((arg1->unk0.unk4.underwaterDist < 24.0f) && (arg1->unk0.unk4.unk25C & 0x10)) {
        return -1;
    }
    if (dll_210_func_7E6C(arg0, objdata, arg1, &playerdata, arg2, 0x400) == 5) {
        _bss_200 = -1;
        return 0x13;
    }
    arg1->unk0.flags |= 0x200000;
    if (arg1->unk0.enteredAnimState != 0) {
        arg0->srt.yaw += arg1->unk0.unk32A * 0xB6;
        arg1->unk0.unk328 = 0;
        arg1->unk0.unk32A = 0;
        if (arg1->unk0.prevAnimState != 0x20) {
            objdata->unk838 = 0.0f;
        }
    }
    objdata->unk838 += gUpdateRateF;
    dll27data = &arg1->unk0.unk4;
    if ((objdata->unk838 > 120.0f) && (objdata->flags & 0x80000)) {
        gDLL_3_Animation->vtbl->func17(0xB, arg0, -1);
    } else {
        temp_fs0 = dll27data->waterY - 6.0f;
        temp_fs0 += fsin16_precise(objdata->unk88A);
        objdata->unk88A += gUpdateRateF * 128.0f;
        sp120 = temp_fs0 - arg0->srt.transl.f[1];
        if (sp120 > 25.0f) {
            sp120 = 25.0f;
        }
        arg0->speed.f[1] += (sp120 / 25.0f) * 0.13f * gUpdateRateF;
        arg0->speed.f[1] -= 0.1f * gUpdateRateF;
        arg0->speed.f[1] *= 0.96f;
        if (arg0->speed.f[1] > 1.4f) {
            arg0->speed.f[1] = 1.4f;
        }
        if (arg1->unk0.unk290 < 0.05f) {
            arg1->unk0.unk328 = 0;
            arg1->unk0.unk32A = 0;
            arg1->unk0.unk290 = 0.0f;
        }
        var_fv0 = (arg1->unk0.unk290 - 0.4f) / 0.6f;
        if (var_fv0 < 0.0f) {
            var_fv0 = 0.0f;
        }
        if (var_fv0 > 1.0f) {
            var_fv0 = 1.0f;
        }
        temp_fv1 = var_fv0 * 0.7f;
        var_fs1 = temp_fv1;
        if (arg1->unk0.unk328 < 0x5A) {
            arg0->srt.yaw = (s16) (s32) ((f32) arg0->srt.yaw + ((((f32) arg1->unk0.unk32A * arg2) / 28.0f) * 182.0f));
        } else {
            var_fs1 = -temp_fv1;
        }
        sp120 = sp120;
        dll_210_func_128F4(arg0->speed.f, &arg0->speed.f[2], arg2, arg0);
        temp_fs0 = fsin16_precise(arg0->srt.yaw);
        temp_fv0 = fcos16_precise(arg0->srt.yaw);
        arg1->unk0.unk27C = (arg0->speed.f[0] * temp_fv0) - (arg0->speed.f[2] * temp_fs0);
        arg1->unk0.unk278 = (-arg0->speed.f[2] * temp_fv0) - (arg0->speed.f[0] * temp_fs0);
        arg1->unk0.speed += ((var_fs1 - arg1->unk0.speed) / 25.0f) * arg2;
        arg1->unk0.unk278 += arg1->unk0.speed;
        arg1->unk0.unk278 *= 0.98f;
        arg1->unk0.unk27C *= 0.97f;
        if (arg1->unk0.speed < *_data_728) {
            return 0x21;
        }
        if (arg0->curModAnimId != 0x413) {
            func_80023D30(arg0, 0x413, 0.0f, 0U);
        }
        func_8002493C(arg0, arg1->unk0.speed, &arg1->unk0.animTickDelta);
        if (sp120 < 25.0f) {
            spB0.transl.f[0] = arg0->srt.transl.f[0];
            spB0.transl.f[1] = 0.0f;
            spB0.transl.f[2] = arg0->srt.transl.f[2];
            spB0.yaw = arg0->srt.yaw;
            spB0.scale = 1.0f;
            spB0.pitch = 0;
            spB0.roll = 0;
            matrix_from_srt(&spCC, &spB0);
            if (arg1->unk0.unk308 & 0x200) {
                for (i = 0; i < 2; i++) {
                    sp114 = (rand_next(-0x14, 0x14) / 10.0f) + 10.0f;
                    sp10C = (rand_next(-0x14, 0x14) / 10.0f) + -17.0f;
                    vec3_transform(&spCC, sp114, 0.0f, sp10C, &sp114, &sp110, &sp10C);
                    gDLL_24_Waterfx->vtbl->func_1CC8(sp114, dll27data->waterY, sp10C, 0, 0.0f, 3);
                }
            }
            if (arg1->unk0.unk308 & 0x400) {
                for (i = 0; i < 2; i++) {
                    sp114 = (rand_next(-0x14, 0x14) / 10.0f) + -10.0f;
                    sp10C = (rand_next(-0x14, 0x14) / 10.0f) + -17.0f;
                    vec3_transform(&spCC, sp114, 0.0f, sp10C, &sp114, &sp110, &sp10C);
                    gDLL_24_Waterfx->vtbl->func_1CC8(sp114, dll27data->waterY, sp10C, 0, 0.0f, 3);
                }
            }
        }
    }
    return 0;
}

// offset: 0x13D08 | func: 95
s32 dll_210_func_13D08(Object* arg0, Player_Data* arg1, f32 arg2) {
    s32 pad;
    s32 sp88;
    ObjectStruct64* temp_v0_3;
    Object* temp_s2;
    Vec3f sp74;
    Vec3f sp68;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    Vec3f sp50;
    Player_Data* objdata;
    s8 v0;
    s16 sp48;
    ModelInstance* sp44;

    objdata = arg0->data;
    temp_s2 = objdata->unk858;
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0 != 0) { return temp_v0; }
    }
    // @fake
    if (((!arg1) && (!arg1)) && (!arg1)) {}
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x22;
    }
    func_800267A4(arg0);
    arg0->speed.f[1] = 0.0f;
    if (arg1->unk0.enteredAnimState != 0) {
        objdata->unk8A9 = 1;
        switch (temp_s2->id) {
        case OBJ_IMSnowBike:
        case OBJ_CRSnowBike:
            objdata->unk76C = _data_158;
            objdata->unk770 = 3;
            gDLL_2_Camera->vtbl->func6(0x57, 1, 0, 0, NULL, 0, 0xFF);
            break;
        case OBJ_DR_CloudRunner:
            objdata->unk76C = _data_170;
            gDLL_2_Camera->vtbl->func6(0x65, 1, 0, 0, NULL, 0, 0xFF);
            break;
        case OBJ_BWLog:
            objdata->unk76C = _data_188;
            objdata->unk770 = 3;
            gDLL_2_Camera->vtbl->func8(0, 0x29);
            break;
        case OBJ_DR_EarthWarrior:
            objdata->unk76C = _data_170;
            objdata->unk770 = 4;
            gDLL_2_Camera->vtbl->func8(0, 0x69);
            break;
        default:
            objdata->unk76C = _data_170;
            objdata->unk770 = 4;
            gDLL_2_Camera->vtbl->func8(0, 0x1D);
            break;
        }
        sp88 = ((DLL_793_BWLog *)temp_s2->dll)->vtbl->func8(temp_s2);
        ((DLL_793_BWLog *)temp_s2->dll)->vtbl->func14(temp_s2, 1);
        switch (sp88) {
            case 1:
                v0 = 6;
                break;
            case 2:
            default:
                v0 = 7;
                break;
        }
        arg0->srt.yaw = temp_s2->srt.yaw;
        func_80023D30(arg0, objdata->unk76C[v0], 0.0f, 4U);
        sp44 = arg0->modelInsts[arg0->modelInstIdx];
        func_8001A3FC(sp44, 0U, 0, 0.0f, arg0->srt.scale, &sp74, &sp48);
        func_8001A3FC(sp44, 0U, 0, 1.0f, arg0->srt.scale, &sp68, &sp48);
        ((DLL_793_BWLog *)temp_s2->dll)->vtbl->func9(temp_s2, &sp5C, &sp60, &sp64);
        sp5C -= arg0->srt.transl.f[0];
        sp60 -= arg0->srt.transl.f[1];
        sp64 -= arg0->srt.transl.f[2];
        objdata->unk738.f[0] = arg0->srt.transl.f[0];
        objdata->unk738.f[1] = arg0->srt.transl.f[1];
        objdata->unk738.f[2] = arg0->srt.transl.f[2];
        objdata->unk744.f[0] = sp5C;
        objdata->unk744.f[1] = sp60 - sp68.f[1];
        objdata->unk744.f[2] = sp64;
        arg0->srt.flags |= 8;
        arg0->ptr0x64->flags |= 0x1000;
        arg1->unk0.animTickDelta = 0.022f;
    }
    arg0->srt.transl.f[0] = objdata->unk738.f[0] + (arg0->animProgress * objdata->unk744.x);
    arg0->srt.transl.f[1] = objdata->unk738.f[1] + (arg0->animProgress * objdata->unk744.y);
    arg0->srt.transl.f[2] = objdata->unk738.f[2] + (arg0->animProgress * objdata->unk744.z);
    ((DLL_793_BWLog *)temp_s2->dll)->vtbl->func12(temp_s2, &sp5C, &sp60, &sp64);
    sp50.z = ((sp5C - objdata->unk738.x) * arg0->animProgress) + objdata->unk738.x;
    sp50.y = ((sp60 - objdata->unk738.y) * arg0->animProgress) + objdata->unk738.y;
    sp50.x = ((sp64 - objdata->unk738.z) * arg0->animProgress) + objdata->unk738.z;
    gDLL_2_Camera->vtbl->func10(sp50.z, sp50.y, sp50.x);
    if ((arg1->unk0.enteredAnimState == 0) && (arg1->unk0.unk33A != 0)) {
        func_80023D30(arg0, *objdata->unk76C, 0.0f, 1U);
        ((DLL_793_BWLog *)temp_s2->dll)->vtbl->func14(temp_s2, 2);
        if (temp_s2->id == 0x22) {
            return 0x26;
        }
        return 0x25;
    }
    return 0;
}

// offset: 0x1426C | func: 96
s32 dll_210_func_1426C(Object* arg0, Player_Data* arg1, f32 arg2) {
    if (main_get_bits(BIT_2D0) != 0) {
        return -1;
    }

    return 0;
}

// offset: 0x142C4 | func: 97
s32 dll_210_func_142C4(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* temp_s0;
    Object* sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;

    gDLL_2_Camera->vtbl->func24.withOneArg(2);
    temp_s0 = arg0->data;
    arg1->unk0.unk4.mode = 0;
    arg1->unk0.animExitAction = dll_210_func_14B70;
    func_800267A4(arg0);
    sp48 = temp_s0->unk858;
    if (sp48 == NULL) {
        arg0->curModAnimIdLayered = -1;
        return 0;
    }
    if (arg1->unk0.enteredAnimState != 0) {
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
        func_80024108(arg0, 0.0f, 0.0f, NULL);
    }
    if (temp_s0->unk770 & 4) {
        func_800240BC(arg0, sp48->animProgress);
        arg1->unk0.animTickDelta = NULL;
    } else {
        sp3C = ((DLL_793_BWLog*)sp48->dll)->vtbl->func16(sp48, &sp44);
        if (sp44 <= 1.0f) {
            arg1->unk0.animTickDelta = sp44;
        } else {
            arg1->unk0.animTickDelta = ((sp3C * 0.05f) + 0.01f);
        }
    }
    if (temp_s0->unk770 & 1) {
        ((DLL_793_BWLog*)sp48->dll)->vtbl->func15(sp48, &sp40, &sp34);
        sp38 = (sp40 * 1023.0f);
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
        func_80024DD0(arg0, 1, 0, sp3C * 1023.0f);
        func_80025140(arg0, arg1->unk0.animTickDelta, arg2, 0);
    }
    if (((DLL_793_BWLog*)sp48->dll)->vtbl->func10(sp48, arg0) != 0) {
        return 0x27;
    }
    return 0;
}

// offset: 0x146D8 | func: 98
s32 dll_210_func_146D8(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* objdata;
    Object* sp60;
    s32 var_a0;
    s32 var_a1;
    s16* temp_v0;
    s32 sp50;
    s32 sp4C;
    f32 sp48;
    s32 v1;
    s32 sp40;

    gDLL_2_Camera->vtbl->func24.withOneArg(2);
    objdata = arg0->data;
    arg1->unk0.unk4.mode = 0;
    arg1->unk0.animExitAction = dll_210_func_14B70;
    func_800267A4(arg0);
    sp60 = objdata->unk858;
    if (sp60 == NULL) {
        arg0->curModAnimIdLayered = -1;
        return 0;
    }
    sp60 = objdata->unk858;
    if (((DLL_793_BWLog *)sp60->dll)->vtbl->func10(sp60, arg0) != 0) {
        temp_v0 = func_80034804(arg0, 9);
        if (temp_v0 != NULL) {
            temp_v0[2] = 0;
            temp_v0[0] = 0;
        }
        return 0x27;
    }
    temp_v0 = func_80034804(arg0, 9);
    if (temp_v0 != NULL) {
        var_a0 = sp60->srt.roll;
        if (sp60->srt.roll < -0x1555) {
            var_a0 = -0x1555;
        } else {
            if (var_a0 > 0x1555) {
                v1 = 0x1555;
            } else {
                v1 = var_a0;
            }
            var_a0 = v1;
        }
        temp_v0[2] = -var_a0;
        var_a1 = sp60->srt.pitch;
        if (sp60->srt.pitch < -0x1555) {
            var_a1 = -0x1555;
        } else {
            if (var_a1 > 0x1555) {
                v1 = 0x1555;
            } else {
                v1 = var_a1;
            }
            var_a1 = v1;
        }
        temp_v0[0] = -var_a1;
    }
    ((DLL_793_BWLog *)sp60->dll)->vtbl->func15(sp60, &sp48, &sp50);
    sp4C = arg0->curModAnimId;
    switch (arg0->curModAnimId) {
    case 0x1B:
        arg1->unk0.animTickDelta = 0.007f;
        sp4C = objdata->unk76C[sp50];
        if (sp4C == 0x1D) {
        }
        break;
    case 0x453:
    case 0x454:
        func_800240BC(arg0, sp48);
        if (sp48 == 1.0f) {
            sp4C = objdata->unk76C[0];
        }
        break;
    default:
        if (arg1->unk0.unk308 & 1) {
            sp40 = gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xA78U, rand_next(0x50, 0x7F), NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->func_954(sp40, (rand_next(-0xF, 0xF) / 100.0f) + 1.0f);
        }
        if ((arg1->unk0.unk308 & 0x80) && (rand_next(0, 0x64) >= 0x47)) {
            sp40 = gDLL_6_AMSFX->vtbl->play_sound(arg0, objdata->unk3B8[rand_next(0xA, 0xE)], rand_next(0x50, 0x7F), NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->func_954(sp40, (rand_next(-0xF, 0xF) / 100.0f) + 1.0f);
        }
        arg1->unk0.animTickDelta = 0.01f;
        if (arg1->unk0.unk33A != 0) {
            sp4C = objdata->unk76C[0];
        }
        break;
    }
    if (sp4C != arg0->curModAnimId) {
        func_80023D30(arg0, sp4C, 0.0f, 0U);
    }
    return 0;
}


// offset: 0x14B70 | func: 99
static void dll_210_func_14B70(Object* arg0, ObjFSA_Data *arg1) {
    s16* temp_v0_2;

    arg0->ptr0x64->flags &= ~0x1000;
    arg0->srt.flags &= ~8;
    arg0->curModAnimIdLayered = -1;
    temp_v0_2 = func_80034804(arg0, 9);
    if (temp_v0_2 != NULL) {
        temp_v0_2[0] = 0;
        temp_v0_2[1] = 0;
        temp_v0_2[2] = 0;
    }
}

// offset: 0x14BE8 | func: 100
#ifndef NON_MATCHING
s32 dll_210_func_14BE8(Object* arg0, Player_Data* arg1, f32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_14BE8.s")
#else
// https://decomp.me/scratch/JVr7y
s32 dll_210_func_14BE8(Object* arg0, Player_Data* arg1, f32 arg2) {
    Object* temp_s2;
    s32 spA0;
    f32 temp;
    s32 var_v0_2;
    Vec3f sp8C;
    Vec3f sp80;
    Player_Data* temp_s1;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 temp_fv0;
    SRT sp54;
    ModelInstance* sp50;
    s16* sp4C;

    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x26;
    }
    temp_s1 = arg0->data;
    temp_s2 = temp_s1->unk858;
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0 != 0) { return temp_v0; }
    }
    func_800267A4(arg0);
    arg0->speed.f[1] = 0.0f;
    if (arg1->unk0.enteredAnimState != 0) {
        ((DLL_793_BWLog*)temp_s2->dll)->vtbl->func9(temp_s2, &arg0->srt.transl.x, &arg0->srt.transl.y, &arg0->srt.transl.z);
        if ((temp_s2->id == 0x72) || (temp_s2->id == 0x38C)) {
            gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0x64, 0xFF);
        } else {
            gDLL_2_Camera->vtbl->func8(0, 1);
        }
        spA0 = ((DLL_793_BWLog*)temp_s2->dll)->vtbl->func11(temp_s2);
        ((DLL_793_BWLog*)temp_s2->dll)->vtbl->func14(temp_s2, 3);
        switch (spA0) {
            case 1:
            var_v0_2 = 8;
            break;
            case 2:
            default:
            var_v0_2 = 9;
            break;
        }
        arg0->srt.yaw = temp_s2->srt.yaw;
        arg0->srt.pitch = 0;
        arg0->srt.roll = 0;
        func_80023D30(arg0, temp_s1->unk76C[var_v0_2], 0.0f, 1U);
        sp50 = arg0->modelInsts[arg0->modelInstIdx];
        func_8001A3FC(sp50, 0U, 0, 0.0f, arg0->srt.scale, &sp8C, &sp54.yaw);
        func_8001A3FC(sp50, 0U, 0, 1.0f, arg0->srt.scale, &sp80, &sp54.yaw);
        sp54.yaw = arg0->srt.yaw;
        sp54.pitch = 0;
        sp54.roll = 0;
        rotate_vec3(&sp54, sp80.f);
        sp80.f[0] += arg0->srt.transl.f[0];
        sp80.f[2] += arg0->srt.transl.f[2];
        arg0->srt.transl.f[1] -= sp8C.f[1];
        temp_fv0 = gDLL_27->vtbl->func_DF4(arg0, sp80.f[0], arg0->srt.transl.f[1], sp80.f[2], 20.0f);
        temp_s1->unk738.x = sp80.f[0];
        temp_s1->unk738.y = temp_fv0;
        temp_s1->unk738.z = sp80.f[2];
        temp_s1->unk744.y = arg0->srt.transl.f[1] - temp_s1->unk738.y;
        temp_s1->unk750 = spA0;
        arg0->srt.flags &= ~8;
        arg0->curModAnimIdLayered = -1;
        arg1->unk0.animTickDelta = 0.016f;
    }
    temp_fv0 = (1.0f - arg0->animProgress);
    arg0->srt.transl.y = temp_s1->unk738.y + (temp_s1->unk744.y * temp_fv0);
    sp54.transl.x = temp_fv0;
    sp4C = func_80034804(arg0, 5);
    temp_fv0 = sp54.transl.x;
    if (sp4C != NULL) {
        sp4C[0] = temp_s2->srt.pitch * temp_fv0;
        sp4C[2] = temp_s2->srt.roll * temp_fv0;
    }
    ((DLL_793_BWLog*)temp_s2->dll)->vtbl->func12(temp_s2, &sp70, &sp74, &sp78);
    gDLL_2_Camera->vtbl->func10(((temp_s1->unk738.x - sp70) * arg0->animProgress) + sp70, ((temp_s1->unk738.y - sp74) * arg0->animProgress) + sp74, temp= ((temp_s1->unk738.z - sp78) * arg0->animProgress) + sp78);
    if ((arg1->unk0.enteredAnimState == 0) && (arg1->unk0.unk33A != 0)) {
        if (sp4C != NULL) {
            sp4C[0] = 0;
            sp4C[2] = 0;
        }
        arg0->ptr0x64->flags &= ~0x1000;
        arg0->positionMirror.x = temp_s1->unk7EC.x;
        arg0->positionMirror.z = temp_s1->unk7EC.z;
        inverse_transform_point_by_object(arg0->positionMirror.x, 0.0f, arg0->positionMirror.z, arg0->srt.transl.f, &sp54.scale, &arg0->srt.transl.z, arg0->parent);
        if (temp_s1->unk750 == 1) {
            arg0->srt.yaw += 0x4000;
        } else {
            arg0->srt.yaw -= 0x4000;
        }
        func_80023D30(arg0, 0, 0.0f, 1U);
        func_80024DD0(arg0, 0, 0, 0);
        ((DLL_793_BWLog*)temp_s2->dll)->vtbl->func14(temp_s2, 0);
        dll_210_func_7260(arg0, (Player_Data* ) temp_s1);
        func_8002674C(arg0);
        temp_s1->unk858 = 0;
        return -1;
    }
    return 0;
}
#endif

// offset: 0x151A0 | func: 101
s32 dll_210_func_151A0(Object* arg0, Player_Data* arg1, f32 arg2) {
    CurveSetup* sp64;
    f32 var_fv0;
    s32 sp5C;
    s32 temp_v0_3;
    Player_Data* temp_s0;
    s32 v0;

    temp_s0 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x27;
    }
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0 != 0) { return temp_v0; }
    }
    arg0->speed.y = 0.0f;
    switch (arg0->curModAnimId) {
    case 0x40D:
        var_fv0 = arg1->unk0.unk284 / 60.0f;
        if (var_fv0 < 0.0f) {
            var_fv0 = -var_fv0;
        }
        if (var_fv0 < 0.1f) {
            var_fv0 = 0.1f;
        }
        if (var_fv0 > 1.0f) {
            var_fv0 = 1.0f;
        }
        if (arg1->unk0.unk284 > 10.0f) {
            arg1->unk0.unk278 = (var_fv0 * 0.1f) + 0.1f;
        } else if (arg1->unk0.unk284 < -10.0f) {
            arg1->unk0.unk278 = -0.1f - (var_fv0 * 0.1f);
        } else {
            arg1->unk0.unk278 = 0.0f;
        }
        func_8002493C(arg0, arg1->unk0.unk278, &arg1->unk0.animTickDelta);
        // incorrect cast?
        v0 = func_800053B0((UnkCurvesStruct*)&temp_s0->unk4D8.unk10, arg1->unk0.unk278 / gUpdateRateF);
        arg1->unk0.unk278 = 0.0f;
        if (v0 != 0) {
            func_80023D30(arg0, 0x40F, 0.0f, 0U);
            gDLL_2_Camera->vtbl->func6(0x54, 1, 1, 0, NULL, 0, 0xFF);
        } else {
            arg0->srt.transl.f[0] = temp_s0->unk550.f[0];
            arg0->srt.transl.f[1] = temp_s0->unk550.f[1];
            arg0->srt.transl.f[2] = temp_s0->unk550.f[2];
            arg0->srt.yaw = arctan2_f(-temp_s0->unk55C.f[0], -temp_s0->unk55C.f[2]);
            arg0->srt.pitch = arctan2_f(temp_s0->unk55C.f[1], sqrtf(SQ(temp_s0->unk55C.x) + SQ(temp_s0->unk55C.z)));
        }
        break;
    case 0x40F:
        arg1->unk0.animTickDelta = 0.015f;
        gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, arg2, 1);
        if (arg1->unk0.unk33A != 0) {
            return 2;
        }
        break;
    case 0x40E:
        arg1->unk0.animTickDelta = 0.015f;
        gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, arg2, 1);
        func_800053B0((UnkCurvesStruct*)&temp_s0->unk4D8.unk10, arg1->unk0.unk278 / gUpdateRateF);
        arg1->unk0.unk278 = 0.0f;
        arg0->srt.transl.f[0] = temp_s0->unk550.f[0];
        arg0->srt.transl.f[1] = temp_s0->unk550.f[1];
        arg0->srt.transl.f[2] = temp_s0->unk550.f[2];
        arg0->srt.yaw = arctan2_f(temp_s0->unk4D8.unk0.x, temp_s0->unk4D8.unk0.z);
        arg0->srt.pitch = arctan2_f(temp_s0->unk4D8.unk0.y, sqrtf(SQ(temp_s0->unk4D8.unk0.x) + SQ(temp_s0->unk4D8.unk0.z)));
        if (arg1->unk0.unk33A != 0) {
            func_80023D30(arg0, 0x40D, 0.0f, 0U);
        }
        break;
    default:
        sp5C = 0x1F;
        temp_v0_3 = gDLL_26_Curves->vtbl->func_1E4(arg0->srt.transl.f[0], arg0->srt.transl.f[1], arg0->srt.transl.f[2], &sp5C, 1, 0);
        if (temp_v0_3 != -1) {
            sp64 = gDLL_26_Curves->vtbl->func_39C(temp_v0_3);
            arg0->srt.transl.f[0] = sp64->base.x;
            arg0->srt.transl.f[1] = sp64->base.y;
            arg0->srt.transl.f[2] = sp64->base.z;
            arg0->srt.yaw = arctan2_f(temp_s0->unk4D8.unk0.x, temp_s0->unk4D8.unk0.z);
            arg0->srt.pitch = arctan2_f(temp_s0->unk4D8.unk0.y, sqrtf(SQ(temp_s0->unk4D8.unk0.x) + SQ(temp_s0->unk4D8.unk0.z)));
            temp_s0->unk578 = gDLL_26_Curves->vtbl->func_E40(sp64, &temp_s0->unk584, &temp_s0->unk5D4, &temp_s0->unk624, NULL);
            temp_s0->unk568 = 0;
            temp_s0->unk56C = &temp_s0->unk584;
            temp_s0->unk570 = &temp_s0->unk5D4;
            temp_s0->unk574 = &temp_s0->unk624;
            temp_s0->unk57C = func_80004C5C;
            temp_s0->unk580 = func_80004CE8;
            func_80005094((UnkCurvesStruct*)&temp_s0->unk4D8.unk10);
        }
        func_80023D30(arg0, 0x40E, 0.0f, 0U);
        gDLL_2_Camera->vtbl->func6(0x62, 1, 0, 0, NULL, 0x28, 0xFF);
        break;
    }

    return 0;
}


// offset: 0x15744 | func: 102
s32 dll_210_func_15744(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* sp34;
    f32 var_fv0;
    f32 var_fv1;
    s32 var_a0;
    s32 var_v0;
    s32 var_v0_2;

    sp34 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        arg1->unk0.unk270 = 0x28;
    }
    dll_210_func_A024(arg0, arg1);
    if (arg1->unk0.enteredAnimState != 0) {
        var_fv0 = sp34->unk680.unk0.x;
        var_a0 = 0;
        if (var_fv0 < 0.0f) {
            var_a0 = 1;
            var_fv0 = -var_fv0;
        }
        var_fv1 = sp34->unk680.unk0.z;
        var_v0_2 = 0;
        if (var_fv1 < 0.0f) {
            var_v0_2 = 1;
            var_fv1 = -var_fv1;
        }
        if (var_fv1 < var_fv0) {
            if (var_a0 != 0) {
                sp34->unk680.unk2E = 0;
            } else {
                sp34->unk680.unk2E = 1;
            }
        } else if (var_v0_2 != 0) {
            sp34->unk680.unk2E = 2;
        } else {
            sp34->unk680.unk2E = 3;
        }
        func_80023D30(arg0, 0x57, 0.0f, 0U);
        arg1->unk0.animTickDelta = 0.016f;
        gDLL_6_AMSFX->vtbl->play_sound(arg0, sp34->unk3B8[rand_next(0, 3) + 15], 0x7FU, NULL, NULL, 0, NULL);
    }
    if (arg1->unk0.unk33A != 0) {
        return -1;
    }
    return 0;
}

// offset: 0x158E0 | func: 103
typedef struct {
    s16 unk0;
    s8 unk2;
    s8 unk3;
} CameraStruct;

s32 dll_210_func_158E0(Object* arg0, ObjFSA_Data* arg1, f32 arg2) {
    u8 temp_v1;
    s32 var_a3;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    Object* sp58;
    s32 sp54;
    s32 sp50;
    Player_Data* objdata;
    f32 temp_fv0_2;
    CameraStruct sp44;

    if (arg1->enteredAnimState != 0) {
        arg1->unk270 = 0x29;
    }
    objdata = arg0->data;
    objdata->unk8BD |= 4;
    if (arg1->enteredAnimState != 0) {
        objdata->unk8A9 = 1;
        sp44.unk0 = 0;
        sp44.unk2 = 1;
        sp44.unk3 = 1;
        gDLL_2_Camera->vtbl->func6(0x55, 1, 0, 4, &sp44, 0, 0xFF);
        arg1->flags |= 0x4000;
        arg1->animExitAction = dll_210_func_16204;
        arg1->unk278 = 0.0f;
    }
    arg1->unk27C = 0.0f;
    sp54 = -1;
    sp50 = objdata->unk680.unk2C;
    sp58 = objdata->unk680.unk28;
    switch (arg0->curModAnimId) {
    case 0x5E:
        arg1->animTickDelta = 0.035f;
        if (!(arg1->unk30C & 0x8000)) {
            arg1->animTickDelta = -0.035f;
        }
        gDLL_18_objfsa->vtbl->func7(arg0, arg1, arg2, 1);
        if ((arg0->animProgress < 0.1f) && (arg1->animTickDelta < 0.0f)) {
            return 2;
        }
        if (arg1->unk33A != 0) {
            if (arg0->animProgress == 1.0f) {
                func_80023D30(arg0, 0x5F, 0.0f, 0U);
                break;
            }
            return 2;
        }
    case 0x73:
        break;
    case 0x5F:
        temp_fv0_2 = arg1->unk284;
        arg1->animTickDelta = 0.01f;
        arg1->unk278 = 0.0f;
        if (temp_fv0_2 > 10.0f) {
            objdata->unk680.unk2C++;
            func_80023D30(arg0, 0x48, 0.0f, 0U);
        } else if (temp_fv0_2 < -10.0f) {
            objdata->unk680.unk2C--;
            func_80023D30(arg0, 0x62, 0.0f, 0U);
        }
        if (!(arg1->unk30C & 0x8000)) {
            func_80023D30(arg0, 0x5E, 0.99f, 0U);
        }
        break;
    case 0x48:
        arg1->animTickDelta = 0.024f;
        gDLL_18_objfsa->vtbl->func7(arg0, arg1, arg2, 0);
        if (arg1->unk33A != 0) {
            sp50++;
            sp54 = 0x4A;
        }
        break;
    case 0x62:
        arg1->animTickDelta = 0.015f;
        gDLL_18_objfsa->vtbl->func7(arg0, arg1, arg2, 1);
        if (arg1->unk33A != 0) {
            sp50--;
            sp54 = 0x63;
        }
        break;
    case 0x4A:
    case 0x4B:
        objdata->flags |= 0x200;
        arg1->animTickDelta = 0.02f;
        if (sp58 != NULL) {
            arg1->animTickDelta += fsin16_precise(-arg0->srt.pitch) * 0.02f;
            var_a3 = 1;
        } else {
            arg1->unk278 = 0.0f;
            var_a3 = 0;
        }
        gDLL_18_objfsa->vtbl->func7(arg0, arg1, arg2, var_a3);
        if ((arg1->unk33A != 0) || (objdata->unk680.unk2D != 0)) {
            if ((arg1->unk284 > 10.0f) && (objdata->unk680.unk2D == 0)) {
                if (arg0->curModAnimId == 0x4A) {
                    sp50++;
                    sp54 = 0x4B;
                } else {
                    sp50--;
                    sp54 = 0x4A;
                }
            } else {
                sp50 = 0;
                arg1->unk33A = 0;
                if (arg0->curModAnimId == 0x4A) {
                    sp54 = 0x4E;
                } else {
                    sp54 = 0x4D;
                }
            }
        }
        if (sp58 != NULL) {
            transform_point_by_object(objdata->unk680.unk10.x, objdata->unk680.unk10.y, objdata->unk680.unk10.z, arg0->srt.transl.f, &sp64, &arg0->srt.transl.f[2], sp58);
            arg0->srt.transl.f[0] += objdata->unk680.unk0.x * 14.5f;
            arg0->srt.transl.f[2] += objdata->unk680.unk0.z * 14.5f;
            dll_210_func_7260(arg0, objdata);
            arg0->srt.pitch = arg1->unk4.relativeFloorPitchSmooth;
        }
        break;
    case 0x63:
    case 0x64:
        arg1->animTickDelta = 0.015f;
        if (sp58 != NULL) {
            arg1->animTickDelta += fsin16_precise(arg0->srt.pitch) * 0.015f;
            var_a3 = 1;
        } else {
            arg1->unk278 = 0.0f;
            var_a3 = 0;
        }
        gDLL_18_objfsa->vtbl->func7(arg0, arg1, arg2, var_a3);
        if (arg1->unk33A != 0) {
            if (arg1->unk284 < -10.0f) {
                if (arg0->curModAnimId == 0x63) {
                    sp50--;
                    sp54 = 0x64;
                } else {
                    sp50++;
                    sp54 = 0x63;
                }
            } else {
                sp50 = 0;
                arg1->unk33A = 0;
                sp54 = 0x65;
            }
        }
        if (sp58 != NULL) {
            transform_point_by_object(objdata->unk680.unk10.x, objdata->unk680.unk10.y, objdata->unk680.unk10.z, arg0->srt.transl.f, &sp64, &arg0->srt.transl.f[2], sp58);
            arg0->srt.transl.f[0] += objdata->unk680.unk0.x * 9.0f;
            arg0->srt.transl.f[2] += objdata->unk680.unk0.z * 9.0f;
            dll_210_func_7260(arg0, objdata);
            arg0->srt.pitch = arg1->unk4.relativeFloorPitchSmooth;
        }
        break;
    case 0x4D:
    case 0x4E:
    case 0x65:
        arg1->animTickDelta = 0.028f;
        gDLL_18_objfsa->vtbl->func7(arg0, arg1, arg2, 1);
        if (arg1->unk33A != 0) {
            return 2;
        }
        break;
    default:
        objdata->unk680.unk2C = 0;
        arg0->srt.yaw = arctan2_f(objdata->unk680.unk0.x, objdata->unk680.unk0.z);
        if (sp58 != NULL) {
            transform_point_by_object(objdata->unk680.unk10.x, objdata->unk680.unk10.y, objdata->unk680.unk10.z, arg0->srt.transl.f, &sp64, &arg0->srt.transl.f[2], sp58);
            arg0->srt.transl.f[0] += objdata->unk680.unk0.x * 11.45f;
            arg0->srt.transl.f[2] += objdata->unk680.unk0.z * 11.45f;
            dll_210_func_7260(arg0, objdata);
        } else {
            arg0->srt.transl.f[0] = objdata->unk680.unk10.x + (objdata->unk680.unk0.x * 11.45f);
            arg0->srt.transl.f[2] = objdata->unk680.unk10.z + (objdata->unk680.unk0.z * 11.45f);
            dll_210_func_7260(arg0, objdata);
        }
        func_80023D30(arg0, 0x5E, 0.0f, 0U);
        break;
    }

    if (sp58 != NULL) {
        temp_v1 = ((DLL_Unknown *)sp58->dll)->vtbl->func[7].withFiveArgsS32(sp58, arg0, (s8) objdata->unk680.unk2C, &sp60, &sp5C);
        if (temp_v1 == 1) {
            arg0->srt.transl.f[0] = sp58->srt.transl.f[0] + sp60;
            arg0->srt.transl.f[2] = sp58->srt.transl.f[2] + sp5C;
            dll_210_func_7260(arg0, objdata);
        } else if (temp_v1 == 2) {
            objdata->unk680.unk2D = 1;
        }
    }
    if (sp54 != -1) {
        objdata->unk680.unk2C = sp50;
        func_80023D30(arg0, sp54, 0.0f, 0U);
    }
    return 0;
}

// offset: 0x16204 | func: 104
static void dll_210_func_16204(Object *obj, ObjFSA_Data *fsa) {
    fsa->flags &= ~0x4000;
}

// offset: 0x16220 | func: 105
s32 dll_210_func_16220(Object* arg0, Player_Data* arg1, f32 arg2) {
    Object* temp_v0;
    Object* temp_v0_2;
    f32 var_fv0;
    Player_Data* temp_s0;

    temp_s0 = arg0->data;
    func_800267A4(arg0);
    arg1->unk0.unk27C = 0.0f;
    arg1->unk0.animExitAction = dll_210_func_1660C;
    temp_s0->unk8A9 = 1;
    if (arg0->curModAnimId == 0xC7) {
        arg1->unk0.animTickDelta = 0.01f;
        arg1->unk0.unk278 = 0.0f;
        if (arg0->animProgress > 0.5f) {
            if (temp_s0->unk708->srt.flags & 0x4000) {
                temp_s0->unk710 -= 0.04f * arg2;
            } else {
                gDLL_28_ScreenFade->vtbl->fade(0x3C, 1);
            }
            temp_v0 = temp_s0->unk708;
            temp_v0->srt.flags |= 0x4000;
            func_800267A4(temp_s0->unk708);
            if (temp_s0->unk710 < 0.0f) {
                temp_s0->unk710 = 0.0f;
            }
        } else {
            temp_s0->unk710 = 1.0f;
            arg0->srt.yaw += (s32) (func_80031DD8(arg0, temp_s0->unk708, 0) * (s32) arg2) >> 4;
        }
        temp_v0_2 = temp_s0->unk708;
        temp_v0_2->srt.scale = temp_v0_2->def->scale * (0.5f + (temp_s0->unk710 * 0.5f));
        if ((arg1->unk0.unk33A != 0) && (gDLL_28_ScreenFade->vtbl->is_complete != NULL)) {
            gDLL_3_Animation->vtbl->func17(0, arg0, -1);
            gDLL_28_ScreenFade->vtbl->fade_reversed(0x3C, 1);
            arg1->unk0.animStateTime = 0;
        }
    } else {
        var_fv0 = (temp_s0->unk708->srt.transl.f[1] - arg0->srt.transl.f[1]) / 70.0f;
        if (var_fv0 > 0.2f) {
            var_fv0 += 0.4f;
        }
        func_80023D30(arg0, 0xC7, var_fv0, 0U);
        temp_s0->unk710 = 1.0f;
    }
    return 0;
}

// offset: 0x164DC | func: 106
s32 dll_210_func_164DC(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* sp24;

    sp24 = arg0->data;
    arg1->unk0.animExitAction = dll_210_func_1660C;
    arg1->unk0.unk27C = 0.0f;
    func_800267A4(arg0);
    sp24->unk8A9 = 1;
    if (arg0->curModAnimId == 0xC7) {
        arg1->unk0.animTickDelta = 0.02f;
        arg1->unk0.unk278 = 0.0f;
        sp24->unk708->srt.flags |= 0x4000;
        func_800267A4(sp24->unk708);
        if (arg0->animProgress > 0.8f) {
            sp24->unk708 = NULL;
            return 2;
        }
        sp24->unk708->srt.scale = sp24->unk708->def->scale * (1.0f - arg0->animProgress);
    } else {
        func_80023D30(arg0, 0xC7, 0.0f, 0U);
    }
    return 0;
}

// offset: 0x1660C | func: 107
static void dll_210_func_1660C(Object* obj, ObjFSA_Data* fsa) {
    func_8002674C(obj);
}

// offset: 0x16648 | func: 108
s32 dll_210_func_16648(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* objdata = arg0->data;
    s32 pad;
    Vec3f sp7C;
    Vec3f sp70;
    f32 sp6C;
    f32 temp_fa1;
    f32 var_fv0;
    s16 pad_sp62;
    s16 sp60;
    ModelInstance* sp5C = arg0->modelInsts[arg0->modelInstIdx];
    s32 pad2;
    Vec3f sp4C;
    u8 sp4B;

    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    // @fake
    if ((_bss_200 && _bss_200) && _bss_200) {}
    _bss_202 = _bss_200;
    sp4B = 0;
    switch (_bss_200) {
    case 0:
        if (arg1->unk0.unk33A != 0) {
            arg0->positionMirror.f[0] = objdata->unk7EC.x;
            arg0->positionMirror.f[1] = objdata->unk7EC.y;
            arg0->positionMirror.f[2] = objdata->unk7EC.z;
            inverse_transform_point_by_object(arg0->positionMirror.f[0], arg0->positionMirror.f[1], arg0->positionMirror.f[2], arg0->srt.transl.f, &arg0->srt.transl.f[1], &arg0->srt.transl.f[2], arg0->parent);
            func_80023D30(arg0, _data_69C[1], 0.0f, 1U);
            arg1->unk0.animTickDelta = 0.01f;
            _bss_202 = _bss_200 = 1;
            func_8001A3FC(sp5C, 0U, 0, 0.0f, arg0->srt.scale, &sp7C, &sp60);
            arg0->srt.transl.f[1] -= sp7C.f[1];
            temp_fa1 = (_bss_1B0[2] + (objdata->unk6B0.unk0.y - objdata->unk7EC.y));
            arg0->speed.f[1] = sqrtf(-temp_fa1 * -5.6f);
            arg0->speed.f[1] = sqrtf(-temp_fa1 * -0.34f);
            sp4C.f[0] = *_bss_1F8 * objdata->unk6B0.unkC.x;
            sp4C.f[1] = *_bss_1F8 * objdata->unk6B0.unkC.y;
            sp4C.f[2] = *_bss_1F8 * objdata->unk6B0.unkC.z;
            sp70.f[0] = objdata->unk6B0.unk1C.x + sp4C.x;
            sp70.f[1] = objdata->unk6B0.unk1C.y + sp4C.y;
            sp70.f[2] = objdata->unk6B0.unk1C.z + sp4C.z;
            arg1->unk0.unk2EC.f[0] = sp70.f[0] - arg0->srt.transl.f[0];
            arg1->unk0.unk2EC.f[1] = 0.0f;
            arg1->unk0.unk2EC.f[2] = sp70.f[2] - arg0->srt.transl.f[2];
            _bss_204 = arg0->srt.transl.f[0];
            _bss_208 = arg0->srt.transl.f[2];
        } else {
            arg0->speed.f[1] = 0.0f;
            gDLL_18_objfsa->vtbl->func10(arg0, &arg1->unk0, arg2, 0.1f);
        }
        break;
    case 1:
        temp_fa1 = _bss_1B0[2] + objdata->unk6B0.unk0.y;
        arg0->speed.f[1] += -0.17f * arg2;
        var_fv0 = (objdata->unk7EC.y - objdata->unk6B0.unk0.z) / (temp_fa1 - objdata->unk6B0.unk0.z);
        if (var_fv0 < 0.0f) {
            var_fv0 = 0.0f;
        } else if (var_fv0 > 1.0f) {
            var_fv0 = 1.0f;
        }
        arg0->srt.transl.f[0] = (arg1->unk0.unk2EC.f[0] * var_fv0) + _bss_204;
        arg0->srt.transl.f[2] = (arg1->unk0.unk2EC.f[2] * var_fv0) + _bss_208;
        if ((temp_fa1 - 1.0f) <= objdata->unk7EC.y) {
            if (objdata->unk848 == 0) {
                objdata->unk848 = gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x768U, 0x7FU, NULL, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->func_954(objdata->unk848, ((f32) rand_next(-0xF, 0xF) / 100.0f) + 1.0f);
            }
            _bss_200 = 7;
            sp4B = 1;
            sp6C = 0.035f;
            func_80023D30(arg0, 0x10, 0.0f, 1U);
            arg1->unk0.animTickDelta = 0.035f;
            arg0->srt.transl.f[0] = objdata->unk6B0.unk1C.x;
            arg0->srt.transl.f[1] = objdata->unk6B0.unk1C.y;
            arg0->srt.transl.f[2] = objdata->unk6B0.unk1C.z;
            arg0->speed.f[1] = 0.0f;
        }
        break;
    case 7:
    case 8:
        if (arg1->unk0.unk33A != 0) {
            arg0->positionMirror.f[0] = objdata->unk7EC.x;
            arg0->positionMirror.f[1] = objdata->unk7EC.y;
            arg0->positionMirror.f[2] = objdata->unk7EC.z;
            inverse_transform_point_by_object(arg0->positionMirror.f[0], arg0->positionMirror.f[1], arg0->positionMirror.f[2], arg0->srt.transl.f, &arg0->srt.transl.f[1], &arg0->srt.transl.f[2], arg0->parent);
            dll_210_func_7260(arg0, objdata);
            func_80023D30(arg0, _data_6C0[0], 0.0f, 1U);
            return 0x2E;
        }
        break;
    case 2:
        if (arg1->unk0.unk33A != 0) {
            if (arg1->unk0.unk284 > 5.0f) {
                arg0->positionMirror.f[0] = objdata->unk7EC.x;
                arg0->positionMirror.f[1] = objdata->unk7EC.y;
                arg0->positionMirror.f[2] = objdata->unk7EC.z;
                inverse_transform_point_by_object(arg0->positionMirror.f[0], arg0->positionMirror.f[1], arg0->positionMirror.f[2], arg0->srt.transl.f, &arg0->srt.transl.f[1], &arg0->srt.transl.f[2], arg0->parent);
                dll_210_func_7260(arg0, objdata);
                func_80023D30(arg0, _data_6C0[0], 0.0f, 1U);
                return 0x2E;
            }
            if (arg1->unk0.unk284 < -5.0f) {
                return 0x2F;
            }
            _bss_200 = 3;
            sp6C = 0.008f;
        }
        break;
    case 3:
        if (arg1->unk0.unk284 > 5.0f) {
            arg0->positionMirror.f[0] = objdata->unk7EC.x;
            arg0->positionMirror.f[1] = objdata->unk7EC.y;
            arg0->positionMirror.f[2] = objdata->unk7EC.z;
            inverse_transform_point_by_object(arg0->positionMirror.f[0], arg0->positionMirror.f[1], arg0->positionMirror.f[2], arg0->srt.transl.f, &arg0->srt.transl.f[1], &arg0->srt.transl.f[2], arg0->parent);
            dll_210_func_7260(arg0, objdata);
            func_80023D30(arg0, _data_6C0[0], 0.0f, 1U);
            return 0x2E;
        }
        if (arg1->unk0.unk284 < -5.0f) {
            return 0x2F;
        }
        break;
    default:
        _bss_200 = 0;
        sp6C = 0.029f;
        arg1->unk0.unk2F8 = objdata->unk6B0.unk54 - arg0->srt.yaw;
        if (objdata->unk6B0.unk45 == 0) {
            arg1->unk0.unk2F8 += 32768.0f;
        }
        if (arg1->unk0.unk2F8 > 32768.0f) {
            arg1->unk0.unk2F8 += -65535.0f;
        }
        if (arg1->unk0.unk2F8 < -32768.0f) {
            arg1->unk0.unk2F8 += 65535.0f;
        }
        arg1->unk0.unk2A0 = 0.0f;
        break;
    }

    if (_bss_202 != _bss_200) {
        func_80023D30(arg0, _data_69C[_bss_200], 0.0f, sp4B);
        arg1->unk0.animTickDelta = sp6C;
    }
    dll_210_func_7260(arg0, objdata);
    return 0;
}


// offset: 0x16EB4 | func: 109
s32 dll_210_func_16EB4(Object* arg0, Player_Data* arg1, f32 arg2) {
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    Vec3f sp80;
    Vec3f sp74;
    ModelInstance* sp70;
    Player_Data* temp_s0;
    s16 pad_sp6A;
    s16 sp68;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    Object* temp_a0;
    s32 pad;

    temp_s0 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        if ((arg0->curModAnimId == *_data_6C0) || (arg0->curModAnimId == *_data_6C8)) {
            _bss_200 = 8;
        } else {
            _bss_200 = 9;
        }
        // @fake
        if (&arg0->srt.transl.x) {}
        if (&arg0->srt.transl.y) {}
        if (&arg0->srt.transl.z) {}
    }
    {
        s32 temp_v0 = dll_210_func_EFB4(arg0, arg1, arg2);
        if (temp_v0) { return temp_v0; }
    }
    sp64 = arg0->srt.transl.x;
    sp60 = arg0->srt.transl.y;
    sp5C = arg0->srt.transl.z;
    arg0->speed.f[1] = 0.0f;
    sp94 = arg1->unk0.unk284 / 60.0f;
    if (sp94 < 0.0f) {
        sp94 = -sp94;
    }
    if (sp94 < 0.1f) {
        sp94 = 0.1f;
    }
    // @fake
    if (!temp_s0) {}
    if (sp94 > 0.8f) {
        sp94 = 0.8f;
    }
    sp70 = arg0->modelInsts[arg0->modelInstIdx];
    sp90 = 0.0f;
    sp8C = arg1->unk0.animTickDelta;
    _bss_202 = _bss_200;
    switch (_bss_200) {
    case 8:
    case 9:
    case 12:
    case 13:
        temp_a0 = temp_s0->unk6B0.unk38;
        ((DLL_Unknown *)temp_a0->dll)->vtbl->func[8].withFiveArgsCustom(temp_a0, temp_s0->unk6B0.unk48, &arg0->srt.transl.f[0], &arg0->srt.transl.f[1], &arg0->srt.transl.f[2]);
        arg0->curModAnimIdLayered = -1;
        sp8C = 0.0f;
        if (_bss_200 & 1) {
            _bss_200 = 1;
        } else {
            _bss_200 = 0;
        }
        break;
    case 6:
    case 7:
    case 10:
    case 11:
        return 0x2F;
    case 4:
    case 5:
        temp_a0 = temp_s0->unk6B0.unk38;
        ((DLL_Unknown *)temp_a0->dll)->vtbl->func[8].withFiveArgsCustom(temp_a0, temp_s0->unk6B0.unk48, &arg0->srt.transl.f[0], &arg0->srt.transl.f[1], &arg0->srt.transl.f[2]);
        if (arg1->unk0.unk284 > 5.0f) {
            func_800240BC(arg0, 0.0f);
        } else if (arg1->unk0.unk284 < -5.0f) {
            func_800240BC(arg0, 0.0f);
        } else if ((arg1->unk0.unk310 & 0x8000) && (main_get_bits(0x1F6) == 0)) {
            func_80023D30(arg0, 0x12, 0.0f, 1);
            arg1->unk0.animTickDelta = 0.2f;
            arg1->unk0.unk278 = 0.0f;
            arg0->speed.f[1] = 0.0f;
            return 0x2F;
        } else {
            break;
        }
    default:
        if (arg0->animProgress == 0.0f) {
            if (arg1->unk0.unk284 > 5.0f) {
                if (((temp_s0->unk6B0.unk48 < 0.3f) && (temp_s0->unk6B0.unk45 == 1)) || ((temp_s0->unk6B0.unk48 > 6.7f) && (temp_s0->unk6B0.unk45 == 0))) {
                    func_80023D30(arg0, 0x12, 0.0f, 1U);
                    arg1->unk0.animTickDelta = 0.2f;
                    arg1->unk0.unk278 = 0.0f;
                    arg0->speed.f[1] = 0.0f;
                    return 0x2F;
                }
                sp8C = (sp94 * 0.033999998f) + 0.015f;
                if (_bss_200 >= 2) {
                    if (_bss_200 & 1) {
                        _bss_200 = 1;
                    } else {
                        _bss_200 = 0;
                    }
                }
            } else if (arg1->unk0.unk284 < -5.0f) {
                pad_sp6A = temp_s0->unk6B0.unk45;
                temp_s0->unk6B0.unk45 = pad_sp6A == 0;
                temp_s0->unk6B0.unk50 = -temp_s0->unk6B0.unk50;
                arg0->srt.yaw += 0x8000;
                sp90 = 0.99f;
                sp8C = -((sp94 * 0.013000001f) + 0.015f);
                if (_bss_200 & 1) {
                    _bss_200 = 2;
                } else {
                    _bss_200 = 3;
                }
            } else if (func_80024E2C(arg0) == 0) {
                sp8C = 0.01f;
                if (((_bss_200 & 1) != 0) && (_bss_200 != 5)) {
                    _bss_200 = 5;
                } else if (((_bss_200 & 1) == 0) && (_bss_200 != 4)) {
                    _bss_200 = 4;
                }
                break;
            }
        }
        if (arg0->animProgress == 1.0f) {
            if (arg1->unk0.unk284 < -5.0f) {
                pad_sp6A = temp_s0->unk6B0.unk45;
                temp_s0->unk6B0.unk45 = pad_sp6A == 0;
                temp_s0->unk6B0.unk50 = -temp_s0->unk6B0.unk50;
                arg0->srt.yaw += 0x8000;
                sp8C = -((sp94 * 0.01f) + 0.025f);
                if (_bss_200 < 2) {
                    _bss_200 += 2;
                    sp90 = 0.99f;
                }
            } else {
                sp8C = 0.0f;
                if (_bss_200 < 2) {
                    _bss_200 ^= 1;
                    sp90 = 0.0f;
                }
                temp_a0 = temp_s0->unk6B0.unk38;
                ((DLL_Unknown *)temp_a0->dll)->vtbl->func[9].withThreeArgsCustom(temp_a0, &temp_s0->unk6B0.unk4C, temp_s0->unk6B0.unk50);
                temp_s0->unk6B0.unk48 = temp_s0->unk6B0.unk4C;
                arg0->animProgress = 0.0f;
            }
        }
        if (arg1->unk0.unk308 & 1) {
            temp_s0->unk848 = gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x768U, rand_next(0x32, 0x7F), NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->func_954(temp_s0->unk848, (rand_next(-0xF, 0xF) / 100.0f) + 1.0f);
        }
        if (sp8C < 0.0f) {
            sp8C = -((sp94 * 0.013000001f) + 0.015f);
        } else if (sp8C > 0.0f) {
            sp8C = (sp94 * 0.033999998f) + 0.015f;
        }
        temp_a0 = temp_s0->unk6B0.unk38;
        ((DLL_Unknown *)temp_a0->dll)->vtbl->func[8].withFiveArgsCustom(temp_a0, temp_s0->unk6B0.unk48, &arg0->srt.transl.x, &arg0->srt.transl.y, &arg0->srt.transl.z);
        sp94 = temp_s0->unk6B0.unk4C;
        temp_a0 = temp_s0->unk6B0.unk38;
        ((DLL_Unknown *)temp_a0->dll)->vtbl->func[9].withThreeArgsCustom(temp_a0, &sp94, temp_s0->unk6B0.unk50 * arg0->animProgress);
        temp_a0 = temp_s0->unk6B0.unk38;
        ((DLL_Unknown *)temp_a0->dll)->vtbl->func[8].withFiveArgsCustom(temp_a0, sp94, &sp64, &sp60, &sp5C);
        temp_a0 = temp_s0->unk6B0.unk38;
        ((DLL_Unknown *)temp_a0->dll)->vtbl->func[10].withThreeArgsCustom2(temp_a0, sp94, 1.0f);
        break;
    }

    gDLL_2_Camera->vtbl->func10(sp64, sp60, sp5C);
    arg1->unk0.animTickDelta = sp8C;
    if (_bss_202 != _bss_200) {
        func_80023D30(arg0, _data_6B0[_bss_200], sp90, 1U);
        if ((_bss_200 < 2) && (temp_s0->unk6B0.unk46 == 0)) {
            func_8001A3FC(sp70, 0U, 0, 0.0f, arg0->srt.scale, &sp80, &sp68);
            func_8001A3FC(sp70, 0U, 0, 1.0f, arg0->srt.scale, &sp74, &sp68);
            temp_s0->unk6B0.unk28.x = sp74.f[0] - sp80.f[0];
            temp_s0->unk6B0.unk28.z = sp74.f[2] - sp80.f[2];
            temp_s0->unk6B0.unk50 = sqrtf(SQ(temp_s0->unk6B0.unk28.x) + SQ(temp_s0->unk6B0.unk28.z));
            if (temp_s0->unk6B0.unk45 == 1) {
                temp_s0->unk6B0.unk50 = -temp_s0->unk6B0.unk50;
            }
            temp_s0->unk6B0.unk46 = 1;
        }
    }
    dll_210_func_7260(arg0, arg0->data);
    return 0;
}

// offset: 0x178A0 | func: 110
s32 dll_210_func_178A0(Object* arg0, Player_Data* arg1, f32 arg2) {
    dll_210_func_7260(arg0, arg0->data);
    return 0xE;
}

// offset: 0x178EC | func: 111
s32 dll_210_func_178EC(Object* arg0, Player_Data* arg1, f32 arg2) {
    Player_Data* temp_s2;

    temp_s2 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        func_80023D30(arg0, 0xEF, 0.0f, 1U);
        arg1->unk0.animTickDelta = 0.015f;
        arg1->unk0.unk278 = 0.0f;
        arg1->unk0.unk27C = 0.0f;
        arg0->speed.f[0] = 0.0f;
        arg0->speed.f[1] = 0.0f;
        arg0->speed.f[2] = 0.0f;
    }
    if ((temp_s2->unk708 != NULL) && (arg0->animProgress > 0.66f)) {
        ((DLL_Unknown *)temp_s2->unk708->dll)->vtbl->func[7].withFiveArgsCustom2(
            temp_s2->unk708,
            arg0,
            2.0f * -fsin16_precise(arg0->srt.yaw),
            0x40400000,
            2.0f * -fcos16_precise(arg0->srt.yaw)
        );
        temp_s2->unk708->srt.flags &= ~0x4000;
        func_8002674C(temp_s2->unk708);
        temp_s2->unk708 = NULL;
    }
    if (arg1->unk0.unk33A != 0) {
        gDLL_1_UI->vtbl->func_2B8(0);
        return 2;
    }
    return 0;
}

// offset: 0x17A88 | func: 112
s32 dll_210_func_17A88(Object* arg0, Player_Data* arg1, f32 arg2) {
    arg1->unk0.unk341 = 3;
    if (arg1->unk0.enteredAnimState != 0) {
        func_80023D30(arg0, 0x88, 0.25f, 0U);
    }

    arg1->unk0.animTickDelta = 0.01f;
    arg1->unk0.unk278 = 0.0f;
    gDLL_18_objfsa->vtbl->func7(arg0, &arg1->unk0, arg2, 2);
    if (arg1->unk0.unk33A != 0) {
        return dll_210_func_A018() + 1;
    }
    return 0;
}

// offset: 0x17B5C | func: 113
s32 dll_210_func_17B5C(Object* arg0, Player_Data* arg1, f32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_17B5C.s")

// offset: 0x17C14 | func: 114
s32 dll_210_func_17C14(Object* arg0, Player_Data* arg1, f32 arg2);
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
s32 dll_210_func_18E80(Object* player, Player_Data* objdata, f32 arg2) {
    if (objdata->unk0.unk310 & 0x8000) {
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
// https://decomp.me/scratch/TtS1w
s32 dll_210_func_18EAC(Object* arg0, Player_Data* arg1, f32 arg2) {
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
    Player_Data* temp_s1;

    temp_s1 = arg0->data;
    if (arg1->unk0.enteredAnimState != 0) {
        if (arg1->unk0.target != NULL) {
            func_80023D30(arg0, 0x43E, 0.0f, 0);
            dll_210_func_6DD8(arg0, temp_s1, *_bss_220);
            arg1->unk0.animTickDelta = 0.015f;
        } else if (temp_s1->unk8A8 != 0) {
            func_80023D30(arg0, 0x43D, 0.0f, 0);
            arg1->unk0.animTickDelta = 0.04f;
        } else {
            func_80023D30(arg0, 0x448, 0.0f, 0);
            if (arg0->id == 0x1F) {
                arg1->unk0.animTickDelta = 0.035f;
            } else {
                arg1->unk0.animTickDelta = 0.024f;
            }
        }
        arg1->unk0.animExitAction = dll_210_func_1AAD8;
        dll_210_func_A024(arg0, arg1);
        temp_s1->unk830 = 0.0f;
        temp_s1->unk82C = 0.0f;
        temp_a0 = arg0->linkedObject;
        *_bss_34 = 0;
        _bss_30 = 0.0f;
        _bss_28 = 0.0f;
        _bss_20 = 0.0f;
        _bss_2C = 0.0f;
        ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs((s32)temp_a0, 0);
    }
    switch (arg0->curModAnimId) {
    case 0x43D:
        if (arg1->unk0.unk33A != 0) {
            func_80023D30(arg0, 0x43E, 0.0f, 0);
            dll_210_func_6DD8(arg0, (Player_Data* ) temp_s1, 0x2D);
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xBA4U, 0x7FU, &temp_s1->unk848, NULL, 0, NULL);
            arg1->unk0.animTickDelta = 0.015f;
        }
        break;
    case 0x449:
        if (arg1->unk0.unk33A != 0) {
            if (arg1->unk0.target != NULL) {
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
            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[7].withOneS32OneF32((s32)temp_a0, 0x3E19999A);
        }
        if (arg1->unk0.unk33A != 0) {
            func_80023D30(arg0, 0x43E, 0.0f, 0);
            dll_210_func_6DD8(arg0, (Player_Data* ) temp_s1, 0x2D);
            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xBA4U, 0x7FU, &temp_s1->unk848, NULL, 0, NULL);
            arg1->unk0.animTickDelta = 0.015f;
        }
        break;
    case 0x43E:
        if (arg1->unk0.target != NULL) {
            if (arg1->unk0.target->unk74 != NULL) {
                var_fv0 = ((f32 *)arg1->unk0.target->unk74)[3] - arg0->srt.transl.x;
                sp8C = ((f32 *)arg1->unk0.target->unk74)[4] - arg0->srt.transl.y;
                var_fv1 = ((f32 *)arg1->unk0.target->unk74)[5] - arg0->srt.transl.z;
            } else {
                var_fv0 = arg1->unk0.target->srt.transl.x - arg0->srt.transl.x;
                sp8C = arg1->unk0.target->srt.transl.y - arg0->srt.transl.y;
                var_fv1 = arg1->unk0.target->srt.transl.z - arg0->srt.transl.z;
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
                var_fa0 = arg1->unk0.unk284 / 50.0f;
                if (var_fa0 < -1.45f) {
                    var_fa0 = -1.45f;
                } else if (var_fa0 > 1.45f) {
                    var_fa0 = 1.45f;
                }
            } else {
                var_fa0 = arg1->unk0.unk284 / 60.0f;
                if (var_fa0 < -1.0f) {
                    var_fa0 = -1.0f;
                } else if (var_fa0 > 1.0f) {
                    var_fa0 = 1.0f;
                }
            }
            var_fa0 -= temp_s1->unk830;
            temp_s1->unk830 += var_fa0 * 0.1f * arg2;
            var_fv1 = arg1->unk0.unk288 / 60.0f;
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
        sp68.scale = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[16].withOneArgS32((s32)temp_a0);
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
                            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs((s32)temp_a0, 1);
                            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6ACU, 0x60U, NULL, NULL, 0, NULL);
                            var_ft4 = _bss_2C;
                        } else {
                            var_ft4 = 120.0f;
                        }
                    }
                } else if (sp68.scale == 1.0f) {
                    arg1->unk0.animTickDelta = 0.02f;
                    if (var_ft4 > 300.0f) {
                        _bss_2C = var_ft4;
                        if (sp98 >= 9) {
                            temp_a0 = arg0->linkedObject;
                            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs((s32)temp_a0, 2);
                            gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x6ACU, 0x7FU, NULL, NULL, 0, NULL);
                            var_ft4 = _bss_2C;
                        } else {
                            var_ft4 = 300.0f;
                        }
                    }
                } else {
                    arg1->unk0.animTickDelta = 0.027f;
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
                        ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs((s32)temp_a0, temp_ft5);
                    }
                    gDLL_6_AMSFX->vtbl->func_954(temp_s1->unk848, sp80);
                    gDLL_6_AMSFX->vtbl->func_860(temp_s1->unk848, 127.0f * sp80);
                }
                temp_fa1 = _bss_20;
                temp_fa1 -= arg2;
                _bss_20 = temp_fa1;
                if (temp_fa1 <= 0.0f) {
                    gDLL_17_partfx->vtbl->spawn(arg0->linkedObject, PARTICLE_3EC, &sp68, PARTFXFLAG_2, -1, NULL);
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
                            _data_7C0->vtbl->func[0].withSixArgs((s32)arg0->linkedObject, arg0->id == 0, 0, 0x10404, -1, 0);
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
                        gDLL_17_partfx->vtbl->spawn(arg0->linkedObject, PARTICLE_525, &sp68, PARTFXFLAG_1, -1, NULL);
                    }
                    if (rand_next(0, 2) == 0) {
                        gDLL_17_partfx->vtbl->spawn(arg0->linkedObject, PARTICLE_526, &sp68, PARTFXFLAG_1, -1, NULL);
                    }
                    if (rand_next(0, 2) == 0) {
                        gDLL_17_partfx->vtbl->spawn(arg0->linkedObject, PARTICLE_529, &sp68, PARTFXFLAG_1, -1, NULL);
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
        if ((arg1->unk0.target == NULL) && (dll_210_func_1A9D4(arg0, &temp_s1->aimX, &temp_s1->aimY, &temp_s1->aimZ, temp_s1->unk82C, temp_s1->unk830) != 0)) {
            temp_s1->flags = (s32) (temp_s1->flags | 0x400);
        }
        if ((((arg1->unk0.target != NULL) && !(temp_s1->unk764 & 0x8000)) || ((arg1->unk0.target == NULL) && (temp_s1->unk768 & 0x8000))) && (sp98 != 0) && (*_bss_220 != 0x5CE)) {
            gDLL_13_Expgfx->vtbl->func4(arg0->linkedObject);
            sp68.transl.x = arg0->linkedObject->srt.transl.x;
            sp68.transl.y = arg0->linkedObject->srt.transl.y;
            sp68.transl.z = arg0->linkedObject->srt.transl.z;
            for (sp9C = 0; sp9C < 0x14; sp9C++) {
                gDLL_17_partfx->vtbl->spawn(arg0->linkedObject, PARTICLE_3ED, &sp68, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            }
            if (sp68.scale == 0.0f) {
                sp98 = 1;
            } else if (sp68.scale == 1.0f) {
                sp98 = 3;
            } else {
                sp98 = 9;
            }
            dll_210_add_magic(arg0, -sp98);
            ((void (*)(Object*, Player_Data*, f32)) *_bss_224)(arg0, arg1, temp_s1->unk830);
            arg1->unk0.animTickDelta = 0.02f;
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
            ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withTwoArgs((s32)temp_a0, 0);
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
        if ((arg1->unk0.target == NULL) && (dll_210_func_1A9D4(arg0, &temp_s1->aimX, &temp_s1->aimY, &temp_s1->aimZ, temp_s1->unk82C, temp_s1->unk830) != 0)) {
            temp_s1->flags |= 0x400;
        }
        if (arg1->unk0.unk33A != 0) {
            temp_v0_4 = *_bss_34;
            temp_v0_4--;
            if ((temp_v0_4 < 0) || (*_bss_34 = temp_v0_4, (*_bss_220 == 0x777))) {
                *_bss_34 = 0;
                if (arg1->unk0.target != NULL) {
                    return 0x36;
                }
                func_80023D30(arg0, 0x43E, 0.0f, 0);
                if (temp_s1->unk830 > 0.0f) {
                    func_80025540(arg0, 0x441, temp_s1->unk830 * 1023.0f);
                } else {
                    func_80025540(arg0, 0x440, -temp_s1->unk830 * 1023.0f);
                }
                arg1->unk0.animTickDelta = 0.015f;
                gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xBA4U, 0x7FU, &temp_s1->unk848, NULL, 0, NULL);
            } else {
                sp68.transl.x = arg0->linkedObject->srt.transl.x;
                sp68.transl.y = arg0->linkedObject->srt.transl.y;
                sp68.transl.z = arg0->linkedObject->srt.transl.z;
                for (sp9C = 0; sp9C < 0x14; sp9C++) {
                    gDLL_17_partfx->vtbl->spawn(arg0->linkedObject, PARTICLE_3ED, &sp68, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                }
                ((void (*)(Object*, Player_Data*, f32)) *_bss_224)(arg0, arg1, temp_s1->unk830);
                arg1->unk0.animTickDelta = 0.02f;
                func_80023D30(arg0, 0x43F, 0.0f, 0);
            }
        }
        break;
    default:
        break;
    }

    if (arg1->unk0.target == NULL && ((temp_s1->flags & 0x400000 && temp_s1->unk766 & 0x4000) || !(temp_s1->flags & 0x401000)) && arg0->curModAnimId != 0x449) {
        func_80023D30(arg0, 0x449, 0.0f, 0);
        arg1->unk0.animTickDelta = 0.04f;
        temp_v0 = gDLL_2_Camera->vtbl->func3();
        if ((temp_v0 != 0x54) && (temp_v0 != 0x5E)) {
            gDLL_2_Camera->vtbl->func6(0x54, 0, 1, 0, NULL, 0x3C, 0xFE);
        }
    }
    if (arg1->unk0.target != NULL) {
        gDLL_18_objfsa->vtbl->func11(arg0, &arg1->unk0, arg2, 4);
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

    temp_v0 = vi_get_current_size();
    temp_t6 = GET_VIDEO_WIDTH(temp_v0) >> 1;
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
void dll_210_func_1AAD8(Object* arg0, ObjFSA_Data *arg1) {
    Object* temp_a0;
    s16* temp_v0;
    s32 i;
    Player_Data* temp_s0;

    temp_s0 = arg0->data;
    temp_s0->unk87C = -1;
    temp_s0->flags &= ~0x400;
    temp_a0 = arg0->linkedObject;
    ((DLL_Unknown*)temp_a0->dll)->vtbl->func[14].withThreeArgs((s32)temp_a0, 0, (s32)arg0);
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
    Player_Data* objdata = player->data;
    objdata->flags &= ~0x800;
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
s32 dll_210_func_1C9D8(Object* player, Player_Data* arg1, s32 arg2) {
    Player_Data* objdata = player->data;

    //NOTE: not sure whether arg1 is really a Player_Data struct
    arg1->unk0.unk341 = 3;
    if (objdata->stats->health > 0) {
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
    Player_Data* objdata = player->data;
    return objdata->unk858;
}

// offset: 0x1CC5C | func: 152 | export: 69
void dll_210_func_1CC5C(Object* player, s32 arg1) {
    Player_Data* objdata = player->data;

    if (arg1 != 0) {
        objdata->flags |= 0x200000;
        return;
    }
    objdata->flags &= 0xFFDFFFFF;
}

// offset: 0x1CC94 | func: 153 | export: 68
void dll_210_func_1CC94(Object* player) {
    Player_Data* objdata = player->data;
    objdata->flags &= ~2;
}

// offset: 0x1CCB0 | func: 154 | export: 50
s32 dll_210_func_1CCB0(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk87C;
}

// offset: 0x1CCC0 | func: 155 | export: 51
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CCC0.s")

// offset: 0x1CD04 | func: 156 | export: 22
void dll_210_set_health(Object* player, s32 health) {
    Player_Data* objdata;

    objdata = player->data;
    if (health < 0) {
        health = 0;
    } else if (objdata->stats->healthMax < health) {
        health = objdata->stats->healthMax;
    }

    objdata->stats->health = health;
}

// offset: 0x1CD3C | func: 157 | export: 23
void dll_210_set_health_max(Object* player, s32 healthMax) {
    Player_Data* objdata = player->data;

    if (healthMax < 0) {
        healthMax = 0;
    } else if (healthMax > MAX_HEALTH) {
        healthMax = MAX_HEALTH;
    }

    objdata->stats->healthMax = healthMax;
}

// offset: 0x1CD6C | func: 158 | export: 24
void dll_210_add_health(Object* player, s32 amount) {
    Player_Data* objdata = player->data;
    s32 newHealth;

    newHealth = objdata->stats->health;
    newHealth += amount;
    if (newHealth < 0) {
        newHealth = 0;
    } else if (objdata->stats->healthMax < newHealth) {
        newHealth = objdata->stats->healthMax;
    }

    objdata->stats->health = newHealth;
}

// offset: 0x1CDAC | func: 159 | export: 25
void dll_210_increase_health_max(Object* player, s32 amount) {
    Player_Data* objdata;
    s32 newHealthMax;

    objdata = player->data;
    newHealthMax = objdata->stats->healthMax;
    newHealthMax += amount;

    if (newHealthMax < 0){
        newHealthMax = 0;
    } else if (newHealthMax > MAX_HEALTH){
        newHealthMax = MAX_HEALTH;
    }

    objdata->stats->healthMax = newHealthMax;
}

// offset: 0x1CDE4 | func: 160 | export: 26
s8 dll_210_get_health(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->stats->health;
}

// offset: 0x1CDF8 | func: 161 | export: 27
s8 dll_210_get_health_max(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->stats->healthMax;
}

// offset: 0x1CE0C | func: 162 | export: 13
void dll_210_set_magic_max(Object* player, s32 magicMax) {
    Player_Data* objdata;

    objdata = player->data;
    if (objdata->unk8BB == 0) {
        return;
    }

    if (magicMax < 0) {
        magicMax = 0;
    } else if (magicMax > MAX_MAGIC) {
        magicMax = MAX_MAGIC;
    }

    objdata->stats->magicMax = magicMax;
}

// offset: 0x1CE48 | func: 163 | export: 15
void dll_210_increase_magic_max(Object* player, s32 amount) {
    Player_Data* objdata = player->data;
    s32 newMagicMax;

    if (objdata->unk8BB == 0) {
        return;
    }

    newMagicMax = objdata->stats->magicMax;
    newMagicMax += amount;
    if (newMagicMax < 0) {
        newMagicMax = 0;
    } else if (newMagicMax > MAX_MAGIC) {
        newMagicMax = MAX_MAGIC;
    }

    objdata->stats->magicMax = newMagicMax;
}

// offset: 0x1CE8C | func: 164 | export: 17
s16 dll_210_get_magic_max(Object* player) {
    Player_Data* objdata;

    objdata = player->data;
    if (objdata->unk8BB == 0) {
        return 0;
    }
    return objdata->stats->magicMax;
}

// offset: 0x1CEB4 | func: 165 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CEB4.s")

// offset: 0x1CEFC | func: 166 | export: 14
//Plays sound effect when magic refilled?
//NOTE: Patched in Dinomod to prevent undebounced sound!
//https://dinosaurpla.net/wiki/Dinomod_Enhanced/Differences/DLLs/210_KrystalSabre#dll_210_add_magic
void dll_210_add_magic(Object* player, s32 amount) {
    Player_Data* objdata = player->data;
    PlayerStats* stats;
    s32 magic;


    if (objdata->unk8BB == 0) {
        return;
    }

    stats = objdata->stats;
    magic = stats->magic;
    magic += amount;
    if (magic < 0) {
        magic = 0;
    } else if (stats->magicMax < magic) {
        magic = stats->magicMax;
    }
    stats->magic = magic;
    if (amount > 0) {
        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_5EB_Magic_Refill_Chime, MAX_VOLUME, 0, 0, 0, 0);
    }
}

// offset: 0x1CFA4 | func: 167 | export: 16
s16 dll_210_get_magic(Object* player) {
    Player_Data* objdata;

    objdata = player->data;
    if (objdata->unk8BB == 0) {
        return 0;
    }

    return objdata->stats->magic;
}

// offset: 0x1CFCC | func: 168 | export: 70
s8 dll_210_func_1CFCC(Object* player) {
    Player_Data* objdata = player->data;
    
    if (objdata->unk0.unk4.unk25C & 0x10) {
        return objdata->unk0.unk4.unk68.unk50[0];
    }

    return -1;
}

// offset: 0x1CFF4 | func: 169 | export: 62
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1CFF4.s")

// offset: 0x1D04C | func: 170 | export: 18
void dll_210_set_scarabs(Object* player, s32 scarabs) {
    Player_Data* objdata = player->data;

    if (scarabs < 0) {
        scarabs = 0;
    } else if (scarabs > MAX_SCARABS) {
        scarabs = MAX_SCARABS;
    }

    objdata->stats->scarabs = scarabs;
}

// offset: 0x1D07C | func: 171 | export: 19
void dll_210_add_scarab(Object* player, s32 amount) {
    s32 newScarabCount;
    Player_Data* objdata = player->data;

    newScarabCount = objdata->stats->scarabs;
    newScarabCount += amount;
    if (objdata->stats->unkA < amount) {
        objdata->stats->unkA = amount;
    }

    if (newScarabCount < 0) {
        newScarabCount = 0;
    } else if (newScarabCount > MAX_SCARABS) {
        newScarabCount = MAX_SCARABS;
    }

    objdata->stats->scarabs = newScarabCount;

    if (player->id != 0) {
        main_set_bits(0x3E4, newScarabCount);
    } else {
        main_set_bits(0x600, newScarabCount);
    }
}

// offset: 0x1D128 | func: 172 | export: 20
u16 dll_210_get_scarabs(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->stats->scarabs;
}

// offset: 0x1D13C | func: 173 | export: 21
s8 dll_210_func_1D13C(Object* player) {
    Player_Data* objdata = player->data;
    s8 previous;

    previous = objdata->stats->unkA;
    objdata->stats->unkA = 0;
    return previous;
}

// offset: 0x1D154 | func: 174 | export: 28
void dll_210_func_1D154(Object* player, s32 arg1) {
    Player_Data* objdata = player->data;
    objdata->unk8A9 = arg1 ? 4 : 2;
}

// offset: 0x1D178 | func: 175 | export: 29
void dll_210_func_1D178(Object* player, s32 arg1) {
    Player_Data* objdata = player->data;
    objdata->unk8A9 = arg1 ? 1 : 0;
}

// offset: 0x1D198 | func: 176 | export: 30
s32 dll_210_func_1D198(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk8A9 == 2;
}

// offset: 0x1D1B0 | func: 177 | export: 31
void dll_210_func_1D1B0(Object* player, s32 arg1) {
    Player_Data* objdata = player->data;
    objdata->unk8A9 = arg1;
}

// offset: 0x1D1C0 | func: 178 | export: 32
u8 dll_210_func_1D1C0(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk8A9;
}

// offset: 0x1D1D0 | func: 179 | export: 78
s32 dll_210_func_1D1D0(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk864;
}

// offset: 0x1D1E0 | func: 180 | export: 79
f32 dll_210_func_1D1E0(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk7FC;
}

// offset: 0x1D1F0 | func: 181 | export: 8
s32 dll_210_func_1D1F0(Object* player, Object** arg1) {
    Player_Data* objdata = player->data;
    *arg1 = objdata->unk708;
    return objdata->unk708 != NULL;
}

// offset: 0x1D210 | func: 182 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D210.s")

// offset: 0x1D288 | func: 183 | export: 10
s32 dll_210_func_1D288(Object* player, Object** arg1) {
    Player_Data* objdata = player->data;
    *arg1 = objdata->unk868;
    return objdata->unk868 != 0;
}

// offset: 0x1D2A8 | func: 184 | export: 11
s32 dll_210_func_1D2A8(Object* arg0, Object* arg1) {
    Player_Data* sp24;
    Object* var_v1;

    sp24 = arg0->data;
    if (arg1 != NULL) {
        sp24->unk868 = arg1;
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &sp24->unk0, 5);
    } else {
        var_v1 = sp24->unk868;
        if (var_v1 != NULL) {
            var_v1->unkE0 = 0;
            sp24->unk868 = NULL;
            sp24->unk870 = 0;
            gDLL_18_objfsa->vtbl->set_anim_state(arg0, &sp24->unk0, 1);
        }
    }

    var_v1 = sp24->unk868;
    return var_v1 != NULL;
}

// offset: 0x1D368 | func: 185 | export: 36
u8 dll_210_func_1D368(Object* player, f32* arg1) {
    Player_Data* objdata = player->data;

    *arg1 = objdata->unk800;
    return objdata->unk8B7;
}

// offset: 0x1D380 | func: 186 | export: 37
void dll_210_func_1D380(Object* player, s32 arg1) {
    Player_Data* objdata = player->data;
    objdata->unk8B7 = arg1;
}

// offset: 0x1D390 | func: 187 | export: 38
s32 dll_210_func_1D390(Object* player, s32 arg1) {
    Player_Data* objdata = player->data;
    return objdata->unk8BA & arg1;
}

// offset: 0x1D3A4 | func: 188 | export: 39
void dll_210_func_1D3A4(Object *arg0, s32 arg1, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D3A4.s")

// offset: 0x1D40C | func: 189 | export: 40
s32 dll_210_func_1D40C(Object* player, u32 arg1) {
    Player_Data* objdata = player->data;
    if (arg1 >= 8) {
        return 0;
    }
    return objdata->unk8BB & (1 << arg1);
}

// offset: 0x1D438 | func: 190 | export: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D438.s")

// offset: 0x1D4C8 | func: 191 | export: 44
s32 dll_210_func_1D4C8(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk354.unk1E == 1;
}

// offset: 0x1D4E0 | func: 192 | export: 33
void dll_210_func_1D4E0(Object* arg0, s32 arg1) {
    Player_Data* sp24;

    sp24 = arg0->data;
    if (arg1 != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &sp24->unk0, 0x44);
        sp24->flags |= 1;
    } else {
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, &sp24->unk0, 1);
        sp24->flags &= ~1;
    }
}

// offset: 0x1D588 | func: 193 | export: 35
s32 dll_210_func_1D588(s32 arg0) {
    return 1;
}

// offset: 0x1D598 | func: 194 | export: 34
s32 dll_210_func_1D598(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->flags & 1;
}

// offset: 0x1D5B0 | func: 195 | export: 42
u8 dll_210_func_1D5B0(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk8B8;
}

// offset: 0x1D5C0 | func: 196 | export: 43
int dll_210_func_1D5C0(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk0.animState == 0x20 || objdata->unk0.animState == 0x21;
}

// offset: 0x1D5E8 | func: 197 | export: 47
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D5E8.s")
#else
u8 dll_210_func_1D5E8(Object* player, s32 arg1, s8* arg2) {
    Player_Data* objdata = player->data;

    *arg2 = objdata->unk680.unk2E;
    return (objdata->unk0.animState == 0x28 && objdata->unk680.unk28 == arg1);
}
#endif

// offset: 0x1D620 | func: 198 | export: 60
s32 dll_210_func_1D620(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk0.unk4.unk25C & 0x10;
}

// offset: 0x1D638 | func: 199 | export: 61
void dll_210_func_1D638(Object* player, s32 arg1) {
    Player_Data* objdata = player->data;

    if (arg1 != 0) {
        objdata->flags |= 0x10;
        return;
    }
    objdata->flags &= ~0x10;
}

// offset: 0x1D668 | func: 200 | export: 58
void dll_210_func_1D668(Object* player, f32 arg1) {
    Player_Data* objdata = player->data;
    objdata->unk808 = arg1;
}

// offset: 0x1D680 | func: 201 | export: 59
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D680.s")

// offset: 0x1D6E0 | func: 202 | export: 71
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/210_player/dll_210_func_1D6E0.s")

// offset: 0x1D754 | func: 203 | export: 52
s32 dll_210_func_1D754(Object* player) {
    Player_Data* objdata = player->data;
    objdata->unk854 = 0;
    return 0;
}

// offset: 0x1D768 | func: 204 | export: 53
Object* dll_210_func_1D768(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk0.target;
}

// offset: 0x1D778 | func: 205 | export: 54
Unk80032CF8 * dll_210_func_1D778(Object* player) {
    Player_Data* objdata = player->data;
    return &objdata->unk354;
}

// offset: 0x1D788 | func: 206 | export: 56
f32 dll_210_func_1D788(Object* player) {
    Player_Data* objdata = player->data;
    return objdata->unk0.unk278;
}

// offset: 0x1D798 | func: 207 | export: 55
void dll_210_func_1D798(Object* player, Player_Data* objdata, void** arg2, s8* arg3, Vec3f** arg4) {
    Player_Data* objdata2 = player->data;

    *arg3 = 0x7F;
    *arg2 = objdata2->unk89C;
    *arg4 = &objdata2->unk39C;
}

// offset: 0x1D7C4 | func: 208 | export: 57
s32 dll_210_func_1D7C4(Object* player) {
  Player_Data* objdata;
  objdata = player->data;
  return (s32)((s8*) objdata) + 4;
}

// offset: 0x1D7D4 | func: 209 | export: 72
void dll_210_func_1D7D4(Object* player, s16 arg1) {
    Player_Data* objdata = player->data;
    objdata->unk886 = arg1;
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
    Player_Data* objdata;

    objdata = player->data;
    if (objdata->flags & 0x400) {
        *arg1 = objdata->aimX;
        *arg2 = objdata->aimY;
        return 1;
    }
    return 0;
}

// offset: 0x1D8B8 | func: 214 | export: 76
void dll_210_func_1D8B8(Object* player) {
    Player_Data* objdata = player->data;

    objdata->unk8BE = 1;
    objdata->unk840 = -0.2f;
    objdata->unk844 = 0.0f;
}

// offset: 0x1D8EC | func: 215
void dll_210_func_1D8EC(Object* arg0, Player_Data* arg1, s32 arg2) {
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
        sp28 = temp_v0->unk52;
    } else {
        sp28 = arg0->unkA8;
    }
    sp2C = (arg0->unk74->y - arg0->srt.transl.y) - arg1;
    sp20 = fsin16_precise(arg0->srt.yaw);
    gDLL_3_Animation->vtbl->func31(-sp20 * sp28, sp2C, -fcos16_precise(arg0->srt.yaw) * sp28);
}

// offset: 0x1DC48 | func: 218
void dll_210_func_1DC48(Object* obj) {
    s8 i;
    Iceblast_Setup* objsetup;

    for (i = 0; i < 3; i++) {
        if (_bss_210[i] != 0) {
            continue;
        }

        objsetup = obj_alloc_create_info(sizeof(Iceblast_Setup), OBJ_iceblast);
        if (objsetup == NULL) {
            break;
        }

        objsetup->base.x = obj->positionMirror.x;
        objsetup->base.y = obj->positionMirror.y;
        objsetup->base.z = obj->positionMirror.z;
        objsetup->base.loadParamA = 2;
        objsetup->base.loadParamB = 1;
        objsetup->base.loadDistance = 0xFF;
        objsetup->base.fadeDistance = 0xFF;
        objsetup->timer = i * 0xA;
        if (obj->id != 0) {
            objsetup->unk1C = 0;
        } else {
            objsetup->unk1C = 0x8000;
        }
        _bss_210[i] = obj_create(&objsetup->base, 5, obj->mapID, -1, obj->parent);
    }
}

// offset: 0x1DD94 | func: 219
Object *dll_210_func_1DD94(Object* obj, s32 arg1) {
    LFXEmitter_Setup* objsetup;

    objsetup = obj_alloc_create_info(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    objsetup->base.loadParamA = 2;
    objsetup->base.loadParamB = 1;
    objsetup->base.loadDistance = 0xFF;
    objsetup->base.fadeDistance = 0xFF;
    objsetup->base.x = obj->srt.transl.x;
    objsetup->base.y = obj->srt.transl.y;
    objsetup->base.z = obj->srt.transl.z;
    objsetup->unk20 = 0;
    objsetup->unk1E = arg1;
    objsetup->unk22 = -1;
    return obj_create(&objsetup->base, 5, obj->mapID, -1, obj->parent);
}

// offset: 0x1DE50 | func: 220
void dll_210_func_1DE50(s32 arg0, s32 arg1, s32 arg2) { }

// offset: 0x1DE64 | func: 221
void dll_210_func_1DE64(UNK_TYPE_32 *arg0) { }
