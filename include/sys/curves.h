#ifndef _SYS_CURVES_H__
#define _SYS_CURVES_H__

#include "game/objects/object.h"
#include "sys/math.h"

typedef f32 (*unk_curve_func)(Vec4f*, f32, f32*); // TODO: first arg is actually f32[4]
typedef void (*unk_curve_func_2)(f32*, f32*);

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18; // curveType?  2) KTrex, 3) RedEye, 1A) camera?, 1B) camera?, 1D) ThornTail, 1F) crawlSpace, 22) Kyte, 24) Tricky
/*19*/ s8 unk19;
/*1A*/ s8 unk1A;
/*1B*/ s8 unk1B;
/*1C*/ s32 unk1C[4];
// TODO: struct split here?
/*2C*/ s8 unk2C;
/*2D*/ s8 unk2D;
/*2E*/ u8 unk2E; 
/*2E*/ s8 unk2F; 
// TODO: some of this depends on the curve type (unk19 not unk18?)
/*30*/ union {
    struct {
        s32 unk30;
        s16 unk34;
    } type15;
    struct {
        /*30*/ s16 unk30; // gameBit
        /*32*/ s16 usedBit; // gameBit
    } type22; // kyte?
    struct {
        s16 unk30; // gameBit
        s16 unk32; // gameBit
    } type24; // tricky?
    struct {
        /*30*/ s8 unk30; 
        /*31*/ s8 unk31;
        /*32*/ s8 unk32;
        /*33*/ s8 unk33;
        /*34*/ s8 unk34; // TODO: s8[4][4] ?
        /*35*/ s8 unk35;
        /*36*/ s8 unk36;
        /*37*/ s8 unk37;
    } type26;
};

} CurveSetup;

// NOTE: If the size of this struct changes, SnowHorn_Data must also be adjusted!
typedef struct {
    /* 0000 */ f32 unk0x0;
    /* 0004 */ f32 unk0x4; // curveProgress? (lerp t-value from 0 to 100?)
    /* 0008 */ f32 unk0x8;
    /* 000C */ f32 unk0xC;
    /* 0010 */ s32 unk0x10;
    /* 0014 */ f32 unk0x14[20];
    /* 0064 */ f32 unk0x64;
    /* 0068 */ f32 unk0x68; //lerp_result_x?
    /* 006C */ f32 unk0x6C; //lerp_result_y?
    /* 0070 */ f32 unk0x70; //lerp_result_z?
    /* 0074 */ f32 unk0x74; //some x component
    /* 0078 */ f32 unk0x78; //some y component
    /* 007C */ f32 unk0x7C; //some z component
    /* 0080 */ s32 unk0x80;
    /* 0084 */ f32 *unk0x84;
    /* 0088 */ f32 *unk0x88;
    /* 008C */ f32 *unk0x8C;
    /* 0090 */ s32 unk0x90;
    /* 0094 */ unk_curve_func unk0x94;
    /* 0098 */ unk_curve_func_2 unk0x98;
    CurveSetup *unk0x9C;
    CurveSetup *unk0xA0;
    CurveSetup *unk0xA4;
    f32 unk0xA8[4];
    f32 unk0xB8[4];
    f32 unk0xC8[4];
    f32 unk0xD8[4];
    f32 unk0xE8[4];
    f32 unk0xF8[4];
} UnkCurvesStruct;

typedef struct Unk80006784 {
    f32 unk0[6];
} Unk80006784;

typedef struct Unk80006908 {
    Vec4f unk0;
    Vec4f unk10;
    f32 unk20;
} Unk80006908;

void func_80005094(UnkCurvesStruct *arg0);
f32 func_80004C5C(Vec4f *a0, f32 a1, f32 *a2);
void func_80004CE8(Vec4f *in, Vec4f *out);
void func_8000523C(UnkCurvesStruct *arg0);
f32 func_80006E04(Unk80006784 *, f32);
f32 func_80006CFC(Unk80006784 *, f32, f32);
f32 func_80004A60(Vec4f *a0, f32 a1, f32 *a2);
s32 func_800053B0(UnkCurvesStruct* arg0, f32 arg1);
void func_8000598C(f32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4, f32* arg5, s32 arg6, unk_curve_func_2 arg7);

#endif
