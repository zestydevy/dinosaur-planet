#ifndef _DLL_75_H
#define _DLL_75_H

#include "dll_def.h"
#include "types.h"

typedef struct {
/*0*/    s16 unk0;  //width?
/*2*/    s16 unk2;  //y?
/*4*/    u8 flags;
/*5*/    u8 type; 
/*6*/    s8 unk6;
/*8*/    s16 min;
/*A*/    s16 max;
/*C*/    s16 value;
} FrontEndSlider; //Maybe checkbox as well?

typedef struct {
/*0*/    s16 unk0;  //width?
/*2*/    s16 unk2;  //y?
/*4*/    u8 flags;
/*5*/    u8 type;
/*6*/    s8 unk6;
/*8*/    s16 min;
/*A*/    s16 max;
/*C*/    s16 value;
/*10*/   char** unk10; //strings
/*14*/   u8 unk14;
} FrontEndOther; //Maybe list/choices? 0x18

DLL_INTERFACE(DLL_75) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ UnknownDLLFunc func0;
/*1*/ UnknownDLLFunc func1;
/*2*/ UnknownDLLFunc func2;
// /*3*/ void (*func3)(FrontEndSlider* slider);
/*3*/ void (*func3)(s32 slider);
/*4*/ UnknownDLLFunc func4;
/*5*/ UnknownDLLFunc func5;
/*6*/ UnknownDLLFunc func6;
/*7*/ UnknownDLLFunc func7;
// /*7*/ s32 (*func7)(FrontEndSlider* slider, s32 enable);
// /*8*/ s32 (*func8)(FrontEndSlider* slider); //get_initial_value/get_value?
/*8*/ s32 (*func8)(s32 slider); //get_initial_value/get_value?
/*9*/ UnknownDLLFunc func9;
/*10*/ UnknownDLLFunc func10;
/*11*/ UnknownDLLFunc func11;
/*12*/ UnknownDLLFunc func12;
/*13*/ UnknownDLLFunc func13;
/*14*/ UnknownDLLFunc func14;
/*15*/ UnknownDLLFunc func15;
/*16*/ UnknownDLLFunc func16;
/*17*/ UnknownDLLFunc func17;
/*18*/ UnknownDLLFunc func18;
/*19*/ UnknownDLLFunc func19;
/*20*/ UnknownDLLFunc func20;
};

#endif // _DLL_75_H
