#ifndef _SYS_NEWSHADOWS_H
#define _SYS_NEWSHADOWS_H

#include "PR/ultratypes.h"
#include "ultra64.h"
#include "PR/gbi.h"

#include "types.h"
#include "game/objects/object.h"
#include "sys/map.h"

typedef struct Unk80092CA8 {
    s16 unk0[3];
} Unk80092CA8;

typedef struct Unk800B98A0 {
    u8 unk0[4000];
} Unk800B98A0;

// Might be a list of Vec3fs?
typedef struct Unk800B98A8 {
    u8 unk0[8000];
} Unk800B98A8;

typedef struct Unk800B98B0 {
    u8 unk0[6400];
} Unk800B98B0;

typedef struct Unk800BB158 {
    u8 unk0[4800];
} Unk800BB158;

typedef struct Unk800BB160 {
    u8 unk0[9600];
} Unk800BB160;

// Should probably have size of 0x24
// Might be the same as UnkFunc80052300Arg3
typedef struct Unk800BB168 {
    u8 unk0[11200];
} Unk800BB168;

// Might be incorrect definition but size should be correct
typedef struct Unk8004FA58 {
    Vec4f pos;
    u8 unk10;
} Unk8004FA58;

// Might be incorrect definition but size should be correct
typedef struct Unk8004FA58_Arg5 {
    s16 x;
    s16 y;
    s16 z;
    s16 pad6;
    s16 unk8;
    s16 unkA;
    u8 padC;
    u8 padD;
    s8 padE;
    s8 unkF;
} Unk8004FA58_Arg5;

// Size: 0x2C
typedef struct Unk80092C48 {
    f32 unk0[8];
    u8 pad20[0xC];
} Unk80092C48;

typedef struct UnkFunc80051D68Arg3 {
        f32 unk0;
        s16 unk4;
        s16 unk6;
        s16 unk8;
        s16 unkA[6];
        s16 unk16[6];
        u8 pad22[0x2F - 0x22];
        s8 unk30;
        u8 pad31[3];
} UnkFunc80051D68Arg3;

typedef struct UnkFunc80052300Arg3 {
    Vec3f unk0[3];
} UnkFunc80052300Arg3;

typedef struct Unk8004EEC0 {
    u8 pad0[0x10];
    Gfx *gdl;
    u8 pad14[0x30 - 0x14];
    s32 unk30;
    u8 pad34[0x41 - 0x34];
    s8 unk41;
} Unk8004EEC0;

typedef struct Unk8004E540 {
    u8 pad0[0x18];
    f32 x;
    f32 y;
    f32 z;
    u8 pad24[0x37 - 0x24];
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
} Unk8004E540;

// xyz pair
extern f32 D_80092BD0;
extern f32 D_80092BD4;
extern f32 D_80092BD8;
extern s32 D_80092BDC;
extern s32 D_80092BE0;
extern f32 D_80092BE4;
extern s8 D_80092BE8;
extern s8 D_80092BF4;
extern s8 D_80092BF8;
extern s8 D_80092BFC;

// Some sort of toggle list
extern s8 D_80092C00;
extern s8 D_80092C04;
extern s8 D_80092C08;
extern s8 D_80092C0C;
extern s8 D_80092C10;
extern s8 D_80092C14;
extern s16 D_80092C18;
extern s16 D_80092C1C;
extern s16 D_80092C20;
extern s16 D_80092C24;
extern s16 D_80092C28;
extern s16 D_80092C2C;
extern s16 D_80092C30;
extern s16 D_80092C34;
extern s16 D_80092C38;
extern f32 D_80092C3C;
extern s32 D_80092C40;
extern f32 D_80092C44;
extern Unk80092C48 D_80092C48;
extern Unk80092C48 D_80092C74;
extern s32 D_80092CA0;
extern s32 D_80092CA4;
extern Unk80092CA8 D_80092CA8;
extern Unk80092CA8 D_80092CB0;
extern Unk8004FA58 D_800BA1A0;
extern f32 D_800B97E0[24];
extern f32 D_800B9840[24];
extern Gfx *D_800B98A0[2];
extern Unk800B98A8 *D_800B98A8[2];
extern Unk8004FA58_Arg5 *D_800B98B0[2];
extern Unk800B98A8 *D_800BB140;
extern Unk800B98A8 *D_800BB144;
extern Unk8004FA58_Arg5 *D_800BB148;
extern Unk8004FA58_Arg5 *D_800BB14C;
extern Gfx *D_800BB150;
extern Gfx *D_800BB154;
extern Gfx *D_800BB158[2];
extern Unk8004FA58_Arg5 *D_800BB160[2];
extern Unk800BB168 *D_800BB168[2];
extern s8 D_800BB170;
extern Unk800BB168* D_800BB174;
extern Unk800BB168* D_800BB178;
extern Unk8004FA58_Arg5* D_800BB17C;
extern Unk8004FA58_Arg5* D_800BB180;
extern Gfx* D_800BB184;
extern Gfx* D_800BB188;
extern f32 D_800BB18C;
extern Texture* D_800BB190;

s32 func_80051D68(Object* arg0, s16 arg1, s16 arg2, UnkFunc80051D68Arg3* arg3, s32 arg4, Vec4f* arg5);

// defined in newshadows.c but used before declared
s32 func_8004FA4C(void);
s32 func_80051F64(s16 arg0, s16 arg1, s16 *arg2, s16 *arg3);
void func_80051C54(Vec3f* A, Vec3f* B, Vec3f* C, Vec3f* D);
void func_80051944(s32 arg0, Object* arg1, Vec3f* arg2, f32 arg3, s16 arg4);
void func_800516BC(Object* obj, Vec3f* arg1, f32 arg2);
void func_800511E8(Object*, s32, Vec3f*, Unk8004FA58*);
f32 func_800528AC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3);
s32 func_80052300(Object* arg0, UnkFunc80051D68Arg3 *arg1, Unk8004FA58* arg2, UnkFunc80052300Arg3* arg3, s32 arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8);
void func_80050B88(Object* arg0, Vec3f* arg1, Unk8004FA58* arg2, Unk8004FA58* arg3, Unk8004FA58* arg4, Unk8004FA58* arg5, s16* arg6, s16* arg7, f32 arg8, s16* arg9, s32 argA);
s32 func_8004F378(Unk8004FA58_Arg5* arg0, Gfx* arg1, ObjectStruct64* arg2, Object* arg3, s32 arg4, s32* arg5);
s32 func_80051CFC(Vec3f* arg0, Vec3f* arg1);
s32 func_800502AC(Object* arg0, Vec3f *arg1, Unk8004FA58* arg2, s32 arg3, Vec3f *arg4, Unk8004FA58_Arg5 *arg5, Unk8004FA58* arg6, s32 arg7);
s32 func_8004EEC0(Unk8004FA58_Arg5 *arg0, Gfx* gdl, ObjectStruct64* arg2, Object *arg3, s32 arg4, s32* arg5);
s32 func_8004FA58(Object* arg0, s32 arg1, Unk8004FA58 *arg2, s32 arg3, Vec3f *arg4, Unk8004FA58_Arg5 *arg5, Unk8004FA58* arg6, s32 max);

// other funcs that are used
u32 func_80041DA4(void);
void func_8005BCE0(Object*, s16, s32, s32, s32, s32, s32, f32, s32);

#endif
