#ifndef _SYS_SEGMENT_53F00_H
#define _SYS_SEGMENT_53F00_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/math.h"

// size: 0x18
typedef struct {
    f32 unk0[4]; // maybe Vec4f and not an array
    struct Object *unk10;
    s8 unk14;
} Func_80057F1C_Struct;

// size: 0x54
typedef struct Func_80059C40_Struct {
    struct Object *unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    Vec4f unk1C;
    Vec3f unk2C;
    Vec3f unk38;
    f32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s8 unk50;
    s8 unk51;
    s8 unk52;
} Func_80059C40_Struct;

// size: 0x10
typedef struct Unk8005341C {
    Object *unk0;
    s16 unk4;
    MtxF *unk8;
    MtxF *unkC;
} Unk8005341C;

typedef struct UnkFunc80051D68Arg3 {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA[3]; // x pos?
    s16 unk10[3]; // y pos?
    s16 unk16[3]; // z pos?
    s16 unk1C[8];
    UNK_TYPE_8 pad2C;
    UNK_TYPE_8 pad2D;
    s8 unk2E;
    s8 unk2F;
    u8 unk30;
    s8 unk31;
    u8 unk32;
    u8 pad33;
} UnkFunc80051D68Arg3;

void func_80058F3C(void);
void fit_aabb_around_cubes(AABBs32 *aabb, Vec3f *posArray1, Vec3f *posArray2, f32 *cubeRadiusArray, s32 arrayLength);
void func_800533D8(s32*, UnkFunc80051D68Arg3**);
void func_80053408(Vec3s32 **);
void func_80053750(Object*, AABBs32*, u8);
Unk8005341C *func_8005341C(s32*);
void func_80053300(void);
void func_800591EC();
void func_8005B5B8(Object*, Object*, s32);
void func_80059038(s32 animatorID, Object* parentObject, s32 enableLines);
s32 func_80057F1C(Object* arg0, f32 x, f32 y, f32 z, Func_80057F1C_Struct*** arg4, s32 arg5, s32 arg6);
s32 func_80059C40(Vec3f* arg0, Vec3f* arg1, f32 arg2, s32 arg3, Func_80059C40_Struct* arg4, Object* arg5, s8 arg6, s8 arg7, u8 arg8, s8 arg9);
s32 func_80058680(Object* arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4, u8 arg5);
s32 func_80056BCC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3, Vec3f* arg4, f32 arg5);
s32 func_80058B1C(Object* arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4, u8 arg5);
s32 func_800588D4(Object* arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4, u8 arg5);
void func_80058FE8(void);
void func_800596BC(ObjDef* arg0);

#endif
