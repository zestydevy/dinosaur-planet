#ifndef _DLL_11_H
#define _DLL_11_H

#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_11_newlfx) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(Object *, Object *, NewLfxStruct *, s32, s32, s32);
/*1*/ void (*func1)(void);
/*2*/ void (*func2)(void);
/*3*/ void *(*func3)(s32, u8*);
/*4*/ UnknownDLLFunc func4;
/*5*/ UnknownDLLFunc func5;
/*6*/ UnknownDLLFunc func6;
/*7*/ UnknownDLLFunc func7;
};

#endif // _DLL_11_H
