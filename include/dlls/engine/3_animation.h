#ifndef _DLLS_3_H
#define _DLLS_3_H

#include "PR/ultratypes.h"
#include "sys/gfx/animseq.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_3_animation) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ void (*func1)(s32 arg0, s32 arg1); // @bug: missing 3rd param
/*2*/ void (*func2)(s32 arg0, s32 arg1);
/*3*/ s8 (*func3)(s32 arg0);
/*4*/ s32 (*func4)(Object* object, s32 updateRate);
/*5*/ void (*func5)(void);
/*6*/ void (*func6)(AnimObj_Data* arg0, AnimObj_Setup* setup);
/*7*/ void (*func7)(AnimObj_Data* state);
/*8*/ void (*func8)(AnimObj_Data* state);
/*9*/ void (*func9)(void);
/*10*/ s16 (*func10)(Object* animObj);
/*11*/ s32 (*func11)(s32 arg0);
/*12*/ s32 (*func12)(void);
/*13*/ s32 (*func13)(void);
/*14*/ void (*func14)(s32 arg0);
/*15*/ s32 (*func15)(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
/*16*/ void (*func16)(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
/*17*/ s32 (*func17)(s32 objectSeqIndex, Object* object, s32 enabledActors);
/*18*/ void (*func18)(s32 arg0);
/*19*/ void (*func19)(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
/*20*/ void (*func20)(Object *arg0, s32 arg1);
/*21*/ void (*func21)(AnimObj_Data* state, s32 arg1);
/*22*/ s8 (*func22)(void);
/*23*/ void (*func23)(s8 arg0);
/*24*/ s16 (*func24)(void);
/*25*/ void (*func25)(s16 arg0);
/*26*/ s16 (*func26)(void);
/*27*/ void (*func27)(s16 arg0);
/*28*/ void (*func28)(s32 arg0, s32 arg1);
/*29*/ s32 (*func29)(Object* arg0, AnimObj_Data* arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6);
/*30*/ s32 (*func30)(s32 arg0, Object *arg1, s32 arg2); //Assigns an objectID to use for VariableObject in the item collection sequence?
/*31*/ s32 (*func31)(f32 arg0, f32 arg1, f32 arg2);
/*32*/ s32 (*func32)(s32 arg0, s32 arg1);
};

#endif //_DLLS_3_H
