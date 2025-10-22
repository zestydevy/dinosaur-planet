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
    /*0*/ void (*curves_func_18)(void);
    /*1*/ void (*curves_func_34)(CurveSetup *);
    /*2*/ void (*curves_func_10c)(CurveSetup *);
    /*3*/ CurveNode *(*curves_func_1bc)(s32 *count);
    /*4*/ s32 (*curves_func_1e4)(f32 x, f32 y, f32 z, s32 *arg3, s32 arg4, s32 arg5);
    /*5*/ s32 (*curves_func_3f00)(Object* arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4);
    /*6*/ CurveSetup *(*curves_func_39c)(s32 curveUID);
    /*7*/ f32 (*curves_func_14f4)(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32* arg5, f32* arg6, f32* arg7);
    /*8*/ f32 (*curves_func_174c)(Object* arg0, s32 arg1);
    /*9*/ f32 (*curves_func_17f4)(f32 arg0, f32 arg1, s32 arg2);
    /*10*/ void (*curves_func_14c8)(CurveSetup** arg0, CurveSetup** arg1);
    /*11*/ s32 (*curves_func_1880)(CurveSetup* arg0);
    /*12*/ s32 (*curves_func_1924)(CurveSetup* arg0);
    /*13*/ s32 (*curves_func_19c8)(CurveSetup* arg0, s32 *arg1, s32 arg2, s32* arg3);
    /*14*/ s32 (*curves_func_1b90)(CurveSetup* arg0, s32 arg1, s32 arg2, s32* arg3);
    /*15*/ s32 (*curves_func_218c)(s32 arg0);
    /*16*/ s32 (*curves_func_21fc)(s32 arg1, s32 arg2, s32* arg3);
    /*17*/ s32 (*curves_func_277c)(f32 arg0, f32 arg1, f32 arg2, s32 arg3);
    /*18*/ s32 (*curves_func_291c)(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4);
    /*19*/ s32 (*curves_func_35ac)(f32 arg0, f32 arg1, f32 arg2);
    /*20*/ s32 (*curves_func_438)(CurveSetup* arg0, s32 arg1);
    /*21*/ s32 (*curves_func_4f0)(CurveSetup* arg0, s32 arg1, s32* arg2);
    /*22*/ s32 (*curves_func_854)(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4);
    /*23*/ s32 (*curves_func_590)(CurveSetup* arg0, s32 arg1);
    /*24*/ s32 (*curves_func_374c)(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4);
    /*25*/ s32 (*curves_func_3930)(CurveSetup* arg0, f32 arg1, f32 arg2, s32* arg3);
    /*26*/ void (*curves_func_a34)(CurveSetup* arg0, s32* arg1);
    /*27*/ s32 (*curves_func_b8c)(s32* arg0, f32 *arg1, f32 *arg2, f32 *arg3);
    /*28*/ s32 (*curves_func_d8c)(CurveSetup* arg0);
    /*29*/ s32 (*curves_func_e40)(CurveSetup* arg0, f32* arg1, f32* arg2, f32* arg3, s8* arg4);
    /*30*/ s32 (*curves_func_2c20)(s32*, f32, f32, f32, f32*, f32*, f32*);
    /*31*/ CurveSetup *(*curves_func_3018)(CurveSetup* arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4);
    /*32*/ void (*curves_func_31e0)(CurveSetup *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4);
    /*33*/ f32 (*curves_func_32d8)(CurveSetup *arg0, CurveSetup *arg1, f32 *arg2, f32 *arg3, f32 arg4, f32 arg5);
    /*34*/ s32 (*curves_func_4288)(UnkCurvesStruct* arg0, Object* arg1, f32 arg2, s32 *arg3, s32 arg4);
    /*35*/ s32 (*curves_func_4704)(UnkCurvesStruct* arg0);
    /*36*/ s32 (*curves_func_4cb4)(UnkCurvesStruct* arg0, s32 arg1);
    /*37*/ void (*curves_func_4fa8)(UnkCurvesStruct* arg0, CurveSetup* arg1);
    /*38*/ s32 (*curves_func_50e4)(UnkCurvesStruct* arg0, s32 arg1);
    /*39*/ s32 (*curves_func_5698)(CurveSetup* arg0, s32 arg1, s32 arg2);
    /*40*/ s32 (*curves_func_577c)(CurveSetup* arg0, s32 arg1, s32 arg2);
    /*41*/ s32 (*curves_func_5860)(UnkCurvesStruct* arg0, s32 arg1, s32 arg2);
};

#endif //_DLLS_26_H
