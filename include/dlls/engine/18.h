#ifndef _DLL_18_H
#define _DLL_18_H

#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_18) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(Object *, void *, s32, s32);
/*1*/ void (*func1)(Object *, void *, f32, f32, UNK_TYPE_32, UNK_TYPE_32);
/*2*/ void (*func2)(Object *, void *, UNK_PTR *);
/*3*/ void (*func3)(Object *);
// state is a Player_Data struct but including it causes an unmatch (TODO: use correct struct here)
/*4*/ void (*func4)(Object *obj, void *state, s32 arg2);
/*5*/ UnknownDLLFunc func5;
/*6*/ UnknownDLLFunc func6;
/*7*/ UnknownDLLFunc func7;
/*8*/ UnknownDLLFunc func8;
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
/*21*/ UnknownDLLFunc func21;
/*22*/ UnknownDLLFunc func22;
};

#endif // _DLL_18_H
