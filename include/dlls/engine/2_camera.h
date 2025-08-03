#ifndef _DLLS_2_H
#define _DLLS_2_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE_BEGIN(2_camera)
/*0*/ void (*func0)(Object *, f32, f32, f32);
/*1*/ void (*func1)(u8);
/*2*/ Object *(*func2)();
/*3*/ UnknownDLLFunc func3;
/*4*/ UnknownDLLFunc func4;
/*5*/ UnknownDLLFunc func5;
/*6*/ void (*func6)(s32, s32, s32, s32, void *, s32, s32);
/*7*/ UnknownDLLFunc func7;
/*8*/ void (*func8)(s32, s32);
/*9*/ void (*func9)(s32, s32);
/*10*/ UnknownDLLFunc func10;
/*11*/ UnknownDLLFunc func11;
/*12*/ UnknownDLLFunc func12;
/*13*/ UnknownDLLFunc func13;
/*14*/ UnknownDLLFunc func14;
/*15*/ UnknownDLLFunc func15;
/*16*/ UnknownDLLFunc func16;
/*17*/ UnknownDLLFunc func17;
/*18*/ UnknownDLLFunc func18;
/*19*/ void (*func19)(void);
/*20*/ UnknownDLLFunc func20;
/*21*/ void (*func21)(s32, s32);
/*22*/ UnknownDLLFunc func22;
/*23*/ UnknownDLLFunc func23;
/*24*/ UnknownDLLFunc func24;
/*25*/ UnknownDLLFunc func25;
/*26*/ UnknownDLLFunc func26;
DLL_INTERFACE_END()

#endif //_DLLS_2_H
