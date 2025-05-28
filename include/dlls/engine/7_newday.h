#ifndef _DLLS_7_H
#define _DLLS_7_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE_BEGIN(7_newday)
    /*0*/ UnknownDLLFunc func0;
    /*1*/ UnknownDLLFunc func1;
    /*2*/ UnknownDLLFunc func2;
    /*3*/ UnknownDLLFunc func3;
    /*4*/ UnknownDLLFunc func4;
    /*5*/ UnknownDLLFunc func5;
    /*6*/ UnknownDLLFunc func6;
    /*7*/ UnknownDLLFunc func7;
    /*8*/ s32 (*func8)(s32); //check if night-time?
    /*9*/ UnknownDLLFunc func9;
    /*10*/ UnknownDLLFunc func10;
    /*11*/ void (*convert_ticks_to_real_time)(f32 ticks, s16 *hours, s16 *minutes, s16 *seconds);
    /*12*/ UnknownDLLFunc func12;
    /*13*/ UnknownDLLFunc func13;
    /*14*/ UnknownDLLFunc func14;
    /*15*/ UnknownDLLFunc func15;
    /*16*/ UnknownDLLFunc func16;
    /*17*/ UnknownDLLFunc func17;
    /*18*/ UnknownDLLFunc func18;
    /*19*/ UnknownDLLFunc func19;
    /*20*/ UnknownDLLFunc func20;
    /*21*/ UnknownDLLFunc func21;
    /*22*/ UnknownDLLFunc func22;
    /*23*/ UnknownDLLFunc func23;
DLL_INTERFACE_END()

#endif //_DLLS_7_H
