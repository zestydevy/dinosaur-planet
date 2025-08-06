#ifndef _DLLS_7_H
#define _DLLS_7_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"
#include "PR/gbi.h"

DLL_INTERFACE_BEGIN(7_newday)
/*0*/ void (*func0)(Object *, Object *, void *, s32);
/*1*/ void (*func1)(void);
/*2*/ void (*func2)(void);
/*3*/ void (*func3)(Gfx **gdl, Mtx **arg1, u8 arg2);
/*4*/ UnknownDLLFunc func4;
/*5*/ UnknownDLLFunc func5;
/*6*/ UnknownDLLFunc func6;
/*7*/ UnknownDLLFunc func7;
/*8*/ s32 (*func8)(s32); // check if night-time?
/*9*/ void (*func9)(f32);
/*10*/ UnknownDLLFunc func10;
/*11*/ void (*convert_ticks_to_real_time)(f32 ticks, s16 *hours, s16 *minutes, s16 *seconds);
/*12*/ UnknownDLLFunc func12;
/*13*/ void (*func13)(Gfx **gdl, Mtx **arg1);
/*14*/ UnknownDLLFunc func14;
/*15*/ UnknownDLLFunc func15;
/*16*/ UnknownDLLFunc func16;
/*17*/ UnknownDLLFunc func17;
/*18*/ UnknownDLLFunc func18;
/*19*/ UnknownDLLFunc func19;
/*20*/ UnknownDLLFunc func20;
/*21*/ UnknownDLLFunc func21;
/*22*/ UnknownDLLFunc func22;
/*23*/ s32 (*func23)(Gfx **gdl);
DLL_INTERFACE_END()

#endif //_DLLS_7_H
