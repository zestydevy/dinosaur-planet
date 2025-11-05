#ifndef _DLLS_26_H
#define _DLLS_26_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"
#include "sys/curves.h"

typedef struct {
    u32 uID;
    CurveSetup *setup;
} CurveNode;

DLL_INTERFACE(DLL_26_curves) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func_18)(void);
/*1*/ void (*func_34)(CurveSetup* arg0);
/*2*/ void (*func_10C)(CurveSetup* setup);
/*3*/ CurveNode *(*func_1BC)(s32 *count);
/*4*/ s32 (*func_1E4)(f32 x, f32 y, f32 z, s32 *arg3, s32 arg4, s32 arg5);
/*5*/ s32 (*func_3F00)(Object *arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4);
/*6*/ CurveSetup *(*func_39C)(s32 curveUID);
/*7*/ f32 (*func_14F4)(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32* arg5, f32* arg6, f32* arg7);
/*8*/ f32 (*func_174C)(Object* arg0, s32 arg1);
/*9*/ f32 (*func_17F4)(f32 arg0, f32 arg1, s32 arg2);
/*10*/ void (*func_14C8)(CurveSetup** arg0, CurveSetup** arg1);
/*11*/ s32 (*func_1880)(CurveSetup* arg0);
/*12*/ s32 (*func_1924)(CurveSetup *setup);
/*13*/ s32 (*func_19C8)(CurveSetup* arg0, s32 *arg1, s32 arg2, s32* arg3);
/*14*/ s32 (*func_1B90)(CurveSetup* arg0, s32 arg1, s32 arg2, s32* arg3);
/*15*/ s32 (*func_218C)(s32 arg0);
/*16*/ s32 (*func_21FC)(s32 arg1, s32 arg2, s32* arg3);
/*17*/ s32 (*func_277C)(f32 arg0, f32 arg1, f32 arg2, s32 arg3);
/*18*/ s32 (*func_291C)(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4);
/*19*/ s32 (*func_35AC)(f32 arg0, f32 arg1, f32 arg2);
/*20*/ s32 (*func_438)(CurveSetup* arg0, s32 arg1);
/*21*/ s32 (*func_4F0)(CurveSetup* arg0, s32 arg1, s32* arg2);
/*22*/ s32 (*func_854)(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4);
/*23*/ s32 (*func_590)(CurveSetup* arg0, s32 arg1);
/*24*/ s32 (*func_374C)(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4);
/*25*/ s32 (*func_3930)(CurveSetup *arg0, f32 arg1, f32 arg2, s32 *arg3);
/*26*/ void (*func_A34)(CurveSetup* arg0, s32* arg1);
/*27*/ s32 (*func_B8C)(s32* arg0, f32 *arg1, f32 *arg2, f32 *arg3);
/*28*/ s32 (*func_D8C)(CurveSetup* arg0);
/*29*/ s32 (*func_E40)(CurveSetup* arg0, f32* arg1, f32* arg2, f32* arg3, s8* arg4);
/*30*/ s32 (*func_2C20)(s32 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);
/*31*/ CurveSetup *(*func_3018)(CurveSetup *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4);
/*32*/ void (*func_31E0)(CurveSetup *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4);
/*33*/ f32 (*func_32D8)(CurveSetup *arg0, CurveSetup *arg1, f32 *arg2, f32 *arg3, f32 arg4, f32 arg5);
/*34*/ s32 (*func_4288)(UnkCurvesStruct* arg0, Object* arg1, f32 arg2, s32 *arg3, s32 arg4);
/*35*/ s32 (*func_4704)(UnkCurvesStruct* arg0);
/*36*/ s32 (*func_4CB4)(UnkCurvesStruct* arg0, s32 arg1);
/*37*/ void (*func_4FA8)(UnkCurvesStruct* arg0, CurveSetup* arg1);
/*38*/ s32 (*func_50E4)(UnkCurvesStruct* arg0, s32 arg1);
/*39*/ s32 (*func_5698)(CurveSetup* setup, s32 arg1, s32 arg2);
/*40*/ s32 (*func_577C)(CurveSetup* setup, s32 arg1, s32 arg2);
/*41*/ s32 (*func_5860)(UnkCurvesStruct* arg0, s32 arg1, s32 arg2);
};

#endif //_DLLS_26_H
