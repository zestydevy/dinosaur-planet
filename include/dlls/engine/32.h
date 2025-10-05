#ifndef _DLL_32_H
#define _DLL_32_H

#include "dll_def.h"
#include "PR/gbi.h"
#include "types.h"

DLL_INTERFACE(DLL_32) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ UnknownDLLFunc func0;
/*1*/ UnknownDLLFunc func1;
/*2*/ void (*func2)(Object *, s16, UNK_TYPE_32);
/*3*/ void (*func3)(Object *, f32, s32, s32, s32);
/*4*/ void (*func4)(Gfx **, Mtx **, Object *);
/*5*/ void (*func5)(Gfx **, Mtx **, Object *);
/*6*/ UnknownDLLFunc func6;
};

#endif // _DLL_32_H
