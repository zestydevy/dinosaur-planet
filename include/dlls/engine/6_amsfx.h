#ifndef _DLLS_6_H
#define _DLLS_6_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE_BEGIN(6_AMSFX)
    /*0*/ UnknownDLLFunc func0;
    /*1*/ void (*func1)(Object *obj);
    /** 2 - Play sound using source object for panning and distance falloff */
    s32 (*func2)(Object* obj, u16 soundID, s8 volume, s32, s32, s32, s32);
    /*3*/ UnknownDLLFunc func3;
    /*4*/ void (*func4)(s32, u8);
    /*5*/ void (*func5)(s32, f32);
    /*6*/ void (*func6)(s32);
    /*7*/ UnknownDLLFunc func7;
    /*8*/ UnknownDLLFunc func8;
    /*9*/ UnknownDLLFunc func9;
    /*10*/ UnknownDLLFunc func10;
    /*11*/ UnknownDLLFunc func11;
    /*12*/ UnknownDLLFunc func12;
    /*13*/ UnknownDLLFunc func13;
    /*14*/ UnknownDLLFunc func14;
    /*15*/ UnknownDLLFunc func15;
    /*16*/ void (*func16)(Object *obj);
    /*17*/ UnknownDLLFunc func17;
    /*18*/ UnknownDLLFunc func18;
    /*19*/ UnknownDLLFunc func19;
    /*20*/ UnknownDLLFunc func20;
DLL_INTERFACE_END()

#endif //_DLLS_6_H
