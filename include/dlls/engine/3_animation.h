#ifndef _DLLS_3_H
#define _DLLS_3_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

#define ANIMCURVES_SCENES_MAX 0x2D //Terminology mightn't be correct
#define ANIMCURVES_ACTORS_MAX 0x10
#define ANIMCURVES_KEYFRAME_CHANNELS 19
#define ANIMCURVES_IS_OBJSEQ2CURVE_INDEX 0x8000

typedef struct {
/*0*/ f32 value;
/*4*/ s8 interpolation;
/*5*/ s8 channel;
/*6*/ s16 timeOffset;
} AnimCurvesKeyframe;

typedef struct {
/*0*/ s8 type;
/*1*/ s8 delay;
/*2*/ union {
        s8 param[2];
        s16 params;
    };
} AnimCurvesEvent;

DLL_INTERFACE(DLL_3_animation) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ void (*func1)(s32, s32);
/*2*/ void (*func2)(s32, s32);
/*3*/ s8 (*func3)(s32 arg0);
/*4*/ s32 (*func4)(Object* object, s32 delayByte);
/*5*/ void (*func5)();
/*6*/ void (*func6)(s32 state, void *setup); // TODO: make ObjAnimSetup
/*7*/ void (*func7)(s32 state);
/*8*/ void (*func8)(s32 state);
/*9*/ void (*func9)();
/*10*/ UnknownDLLFunc func10;
/*11*/ s32 (*func11)(s32 arg0);
/*12*/ s32 (*func12)(void);
/*13*/ s32 (*func13)(void);
/*14*/ void (*func14)(s32 arg0);
/*15*/ s32 (*func15)(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
/*16*/ void (*func16)(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
/*17*/ void (*func17)(s32 objectSeqIndex, Object* object, s32);
/*18*/ void (*func18)(s32);
/*19*/ void (*func19)(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
/*20*/ void (*func20)(Object *arg0, s32 arg1);
// /*21*/ void (*func21)(ANIMUnknown* arg0, s32 arg1);
/*21*/ void (*func21)(void* arg0, s32 arg1);
/*22*/ s8 (*func22)(void);
/*23*/ void (*func23)(s8 arg0);
/*24*/ s16 (*func24)(void);
/*25*/ void (*func25)(s16 arg0);
/*26*/ s16 (*func26)(void);
/*27*/ void (*func27)(s16 arg0);
/*28*/ void (*func28)(s32 arg0, s32 arg1);
/*29*/ UnknownDLLFunc func29;
/*30*/ s32 (*func30)(s32 arg0, void *arg1, s32 arg2);
/*31*/ s32 (*func31)(f32 arg0, f32 arg1, f32 arg2);
/*32*/ s32 (*func32)(s32 arg0, s32 arg1);
};

#endif //_DLLS_3_H
