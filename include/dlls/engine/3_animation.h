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
/*3*/ UnknownDLLFunc func3;
/*4*/ s32 (*func4)(Object* object, s32 delayByte);
/*5*/ void (*func5)();
/*6*/ void (*func6)(s32 state, s32 createInfo);
/*7*/ void (*func7)(s32 state);
/*8*/ void (*func8)(s32 state);
/*9*/ void (*func9)();
/*10*/ UnknownDLLFunc func10;
/*11*/ UnknownDLLFunc func11;
/*12*/ UnknownDLLFunc func12;
/*13*/ UnknownDLLFunc func13;
/*14*/ UnknownDLLFunc func14;
/*15*/ UnknownDLLFunc func15;
/*16*/ UnknownDLLFunc func16;
/*17*/ void (*func17)(s32 objectSeqIndex, Object* object, s32);
/*18*/ void (*func18)(s32);
/*19*/ UnknownDLLFunc func19;
/*20*/ UnknownDLLFunc func20;
/*21*/ UnknownDLLFunc func21;
/*22*/ UnknownDLLFunc func22;
/*23*/ UnknownDLLFunc func23;
/*24*/ UnknownDLLFunc func24;
/*25*/ UnknownDLLFunc func25;
/*26*/ UnknownDLLFunc func26;
/*27*/ UnknownDLLFunc func27;
/*28*/ UnknownDLLFunc func28;
/*29*/ UnknownDLLFunc func29;
/*30*/ UnknownDLLFunc func30;
/*31*/ UnknownDLLFunc func31;
/*32*/ UnknownDLLFunc func32;
};

#endif //_DLLS_3_H
