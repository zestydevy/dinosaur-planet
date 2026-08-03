#ifndef _DLLS_7_H
#define _DLLS_7_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/envfx.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_7_newday) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(Object* arg0, Object* arg1, EnvFxAction* arg2, s32 arg3);
/*1*/ void (*func1)(void);
/*2*/ void (*func2)(void); // @bug: does not match implementation signature
/*3*/ void (*func3)(Gfx** gdl, Mtx**  arg1, u8 arg2);
/*4*/ void (*func4)(f32* timeSeconds);
/*5*/ void (*func5)(f32* arg0);
/*6*/ void (*func6)(f32 arg0);
/*7*/ void (*func7)(s32* param1);
/*8*/ s32 (*func8)(f32* outTime); // check if night-time
/*9*/ void (*func9)(f32 time);
/*10*/ s32 (*func10)(void);
/*11*/ void (*convert_ticks_to_real_time)(f32 ticksF, s16* hours, s16* minutes, s16* seconds);
/*12*/ s32 (*func12)(void);
/*13*/ void (*func13)(Gfx** gdl, Mtx** arg1);
/*14*/ void (*func14)(u8* arg0, u8* arg1, u8* arg2);
/*15*/ void (*func15)(u8* arg0, u8* arg1, u8* arg2, u8* arg3, u8* arg4, u8* arg5);
/*16*/ void (*func16)(u8 arg0);
/*17*/ s32 (*func17)(void);
/*18*/ void (*func18)(u16* arg0, u16* arg1, u16* arg2, u16* arg3);
/*19*/ void (*func19)(u16* arg0, u16* arg1, u16 arg2, u16 arg3);
/*20*/ void (*func20)(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
/*21*/ void (*func21)(u8 arg0);
/*22*/ u8 (*func22)(void);
/*23*/ s32 (*func23)(Gfx** gdl);
};

#endif //_DLLS_7_H
