#ifndef _DLLS_25_H
#define _DLLS_25_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "dlls/engine/26_curves.h"
#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/math.h"

typedef struct Dll25BssDC {
    s8 unk0[8];
    f32 unk8[4];
    s16 unk18;
    union {
        struct {
            s8 unk1A;
            s8 unk1B;
        } bytes;
        s16 word;
    } unk1A;
    u8 unk1C[4];
} Dll25BssDC;

typedef struct Dll25BssE0 {
    s8 unk0[8];
    f32 unk8[4];
    s16 unk18;
    union {
        struct {
            s8 unk1A;
            s8 unk1B;
        } bytes;
        s16 word;
    } unk1A;
    u16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 pad26[0x28 - 0x26];
} Dll25BssE0;

DLL_INTERFACE(DLL_25) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func_D8)(void);
/*1*/ s32 (*func_1158)(f32 *arg0, Dll25BssDC *arg1);
/*2*/ s32 (*func_12FC)(Vec3 arg0);
/*3*/ s32 (*func_1474)(Vec3 arg0, s32 arg1);
/*4*/ s32 (*func_15B0)(Vec3 arg0, s32 arg1, s32 arg2);
// returns a CurveSetup of type 0x24
/*5*/ CurveSetup *(*func_1864)(Vec3 arg0, s32 arg1, s32 arg2);
// returns a CurveSetup of type 0x24
/*6*/ CurveSetup *(*func_1A2C)(Vec3 arg0, s32 arg1, s32 arg2);
/*7*/ s32 (*func_16D4)(Vec3f *arg0, Vec3f *arg1, s32 arg2);
/*8*/ s32 (*func_1D30)(UnkCurvesStruct *arg0, CurveSetup *arg1, CurveSetup *arg2, CurveSetup *arg3);
/*9*/ s32 (*func_21F4)(UnkCurvesStruct *arg0, CurveSetup *arg1);
/*10*/ s32 (*func_27A8)(UnkCurvesStruct *arg0, CurveSetup *arg1);
/*11*/ void (*func_29FC)(UnkCurvesStruct* arg0);
// returns a CurveSetup of type 0x22
/*12*/ CurveSetup *(*func_2A50)(Object *obj, s32 arg1);
// returns a CurveSetup of type 0x22
/*13*/ CurveSetup *(*func_2BC4)(Object *obj, s32 arg1);
// returns a CurveSetup of type 0x22
/*14*/ CurveSetup *(*func_2CF8)(Object *obj, s32 arg1);
};

#endif //_DLLS_25_H


