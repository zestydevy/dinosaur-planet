#ifndef _SYS_CURVES_H__
#define _SYS_CURVES_H__

#include "common.h"

typedef f32 (*unk_curve_func)(Vec4f*, f32, f32*); // TODO: first arg is actually f32[4]
typedef void (*unk_curve_func_2)(f32*, f32*);

typedef struct UnkInnerCurvesStruct {
    s32 unk0;
    s32 pad4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    u8 pad14[0x2C - 0x14];
    s8 unk2C;
    s8 unk2D;
    u8 unk2E;
} UnkInnerCurvesStruct;

typedef struct {
    /* 0000 */ f32 unk0x0;
    /* 0004 */ f32 unk0x4;
    /* 0008 */ f32 unk0x8;
    /* 000C */ f32 unk0xC;
    /* 0010 */ s32 unk0x10;
    /* 0014 */ f32 unk0x14;
    /* 0018 */ f32 unk0x18[19]; // unsure of length
    /* 0064 */ f32 unk0x64;
    /* 0068 */ f32 unk0x68;
    /* 006C */ f32 unk0x6C;
    /* 0070 */ f32 unk0x70;
    /* 0074 */ f32 unk0x74;
    /* 0078 */ f32 unk0x78;
    /* 007C */ f32 unk0x7C;
    /* 0080 */ s32 unk0x80;
    /* 0084 */ f32 *unk0x84;
    /* 0088 */ f32 *unk0x88;
    /* 008C */ f32 *unk0x8C;
    /* 0090 */ s32 unk0x90;
    /* 0094 */ unk_curve_func unk0x94;
    /* 0098 */ s32 unk0x98;
    s32 pad0x9C;
    UnkInnerCurvesStruct *unk0xA0;
    UnkInnerCurvesStruct *unk0xA4;
    f32 unk0xA8[4];
    f32 unk0xB8[4];
    f32 unk0xC8[4];
    f32 unk0xD8[4];
    f32 unk0xE8[4];
    f32 unk0xF8[4];
} UnkCurvesStruct;

void func_80005094(UnkCurvesStruct *arg0);
f32 func_80004C5C(Vec4f *a0, f32 a1, f32 *a2);
void func_80004CE8(Vec4f *in, Vec4f *out);
void func_8000523C(UnkCurvesStruct *arg0);

#endif
