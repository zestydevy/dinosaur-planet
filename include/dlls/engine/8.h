#ifndef _DLL_8_H
#define _DLL_8_H

#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"
#include "PR/gbi.h"

DLL_INTERFACE(DLL_8) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(Object *, Object *, void *, s32, u16);
/*1*/ void (*func1)(void);
/*2*/ void (*func2)(void);
/*3*/ void (*func3)(Gfx **);
/*4*/ UnknownDLLFunc func4;
/*5*/ UnknownDLLFunc func5;
/*6*/ UnknownDLLFunc func6;
/*7*/ UnknownDLLFunc func7;
/*8*/ UnknownDLLFunc func8;
/*9*/ UnknownDLLFunc func9;
/*10*/ UnknownDLLFunc func10;
/*11*/ UnknownDLLFunc func11;
/*12*/ UnknownDLLFunc func12;
};

#endif // _DLL_8_H
