#ifndef _DLL_53_H
#define _DLL_53_H

#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/gfx/animseq.h"
#include "sys/objexpr.h"
#include "dll_def.h"

// size: 0x4B8
typedef struct {
    f32 unk0;
    Vec3f unk4;
    Vec3f unk10;
    HeadAnimation unk1C[30];
    s16 unk454[30];
    s32 unk490;
    s32 unk494;
    u8 unk498;
    u8 unk499;
    Object* unk49C;
    Object* unk4A0;
    s16 unk4A4;
    s16 unk4A6;
    u8 unk4A8;
    u8 unk4A9;
    s32 unk4AC;
    s32 unk4B0;
    s32 unk4B4;
} MoveLibData;

// unknown size
typedef struct {
    u8 _unk0[0x2C - 0x0];
    s8 unk2C;
    s8 unk2D;
} DLL53Func17F4Arg1;

// unknown size
typedef struct {
    Vec3f unk0;
    Vec3f unkC;
    Vec3f unk18;
    Vec3f unk24;
    f32 unk30;
    f32 unk34;
} DLL53Func17F4Arg2;

// Temp DLL 1 (see main.c)
DLL_INTERFACE(DLL_53) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*func0)(Object* arg0, MoveLibData* arg1);
    /*1*/ void (*func1)(MoveLibData* arg0, Object* arg1);
    /*2*/ void (*func2)(Object* arg0, MoveLibData* arg1, s16 arg2, s16 arg3, s32 arg4);
    /*3*/ void (*func3)(Object* arg0, MoveLibData* arg1, s32 arg2);
    /*4*/ s32 (*func4)(Object* arg0, AnimObj_Data* arg1, MoveLibData* arg2, s16 arg3, s16 arg4);
    /*5*/ void (*func5)(MoveLibData* arg0, s32 arg1, s32 arg2);
    /*6*/ void (*func6)(MoveLibData* arg0, s16* arg1, s16* arg2, s32 arg3);
    /*7*/ s32 (*func7)(s32 arg0, SRT* arg1);
    /*8*/ f32 (*func8)(Object* arg0, s32 arg1);
    /*9*/ s32 (*func9)(s32 arg0, SRT* arg1);
    /*10*/ s32 (*func10)(Object* arg0, SRT* arg1, f32 arg2, s32 arg3, f32* arg4, u8* arg5);
    /*11*/ s32 (*func11)(Object* arg0, UnkCurvesStruct* arg1, DLL53Func17F4Arg2* arg2, f32 arg3, u8 arg4, f32* arg5, s32* arg6);
    /*12*/ s32 (*func12)(Object* arg0, s32 arg1, Vec3f* arg2);
};

#endif // _DLL_53_H
