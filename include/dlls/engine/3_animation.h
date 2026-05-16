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
/*4*/ s32 (*tick_obj)(Object* animObj, s32 updateRate);
/*5*/ void (*update_camera)(void);
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
/*17*/ s32 (*start_obj_sequence)(s32 objectSeqIndex, Object* object, s32 enabledActors);
/*18*/ void (*end_obj_sequence)(s32 arg0);
/*19*/ void (*set_camera_module)(s32 module, s32 arg1, s32 arg2, s32 arg3);
/*20*/ void (*preempt_sequence_time)(Object *arg0, s32 arg1);
/*21*/ void (*func21)(AnimObj_Data* state, s32 arg1);
/*22*/ s8 (*func22)(void);
/*23*/ void (*func23)(s8 arg0);
/*24*/ s16 (*get_anim_counter1)(void);
/*25*/ void (*set_anim_counter1)(s16 value);
/*26*/ s16 (*get_anim_counter2)(void);
/*27*/ void (*set_anim_counter2)(s16 value);
/*28*/ void (*func28)(s32 arg0, s32 arg1);
/*29*/ s32 (*func29)(Object* arg0, AnimObj_Data* arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6);
/*30*/ s32 (*set_variable_obj)(s32 objID, Object *arg1, s32 arg2); //Assigns an objectID to use for VariableObject in sequences
/*31*/ s32 (*func31)(f32 arg0, f32 arg1, f32 arg2);
/*32*/ s32 (*func32)(s32 arg0, s32 arg1);
};

#endif //_DLLS_3_H
