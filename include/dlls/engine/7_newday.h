#ifndef _DLLS_7_H
#define _DLLS_7_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"
#include "PR/gbi.h"

DLL_INTERFACE(DLL_7_newday) {
/*:*/ DLL_INTERFACE_BASE(DLL);
// Unmatched
/*0*/ void (*func0)(Object *arg0, Object *arg1, void *arg2, s32 arg3);
// Unmatched
/*1*/ void (*func1)(void);
// Unmatched
/*2*/ void (*func2)(void);
// Unmatched
/*3*/ void (*func3)(Gfx **gdl, Mtx **arg1, u8 arg2);
/*4*/ void (*func4)(f32* timeSeconds);
// Unmatched
/*5*/ void (*func5)(f32* arg0);
/*6*/ UnknownDLLFunc func6;
/*7*/ void (*func7)(s32 *param1);
/*8*/ s32 (*func8)(f32* outTime); // check if night-time
/*9*/ void (*func9)(f32 time);
/*10*/ s32 (*func10)(void);
/*11*/ void (*convert_ticks_to_real_time)(f32 ticksF, s16 *hours, s16 *minutes, s16 *seconds);
// Unmatched
/*12*/ s32 (*func12)(void);
// Unmatched
/*13*/ void (*func13)(Gfx **gdl, Mtx **arg1);
/*14*/ UnknownDLLFunc func14;
/*15*/ UnknownDLLFunc func15;
/*16*/ UnknownDLLFunc func16;
/*17*/ s32 (*func17)(void);
/*18*/ UnknownDLLFunc func18;
/*19*/ UnknownDLLFunc func19;
/*20*/ UnknownDLLFunc func20;
/*21*/ UnknownDLLFunc func21;
/*22*/ u8 (*func22)(void);
// Unmatched
/*23*/ s32 (*func23)(Gfx **gdl);
};

#endif //_DLLS_7_H
