#ifndef _DLL_17_H
#define _DLL_17_H

#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_17) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ void (*func1)(Object *obj, s32, SRT *, s32, s32, f32 *);
/*2*/ UnknownDLLFunc func2;
};

#endif // _DLL_17_H
