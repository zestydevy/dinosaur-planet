#ifndef _DLL_9_H
#define _DLL_9_H

#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"
#include "PR/gbi.h"

DLL_INTERFACE_BEGIN(9_newclouds)
/*0*/ void (*func0)(Object *, Object *, void *, s32);
/*1*/ void (*func1)(void);
/*2*/ void (*func2)(s32, s32);
/*3*/ void (*func3)(void);
/*4*/ void (*func4)(Gfx **);
/*5*/ UnknownDLLFunc func5;
/*6*/ UnknownDLLFunc func6;
/*7*/ UnknownDLLFunc func7;
/*8*/ UnknownDLLFunc func8;
DLL_INTERFACE_END()

#endif // _DLL_9_H
