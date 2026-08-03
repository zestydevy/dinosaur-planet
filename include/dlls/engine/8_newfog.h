#ifndef _DLL_8_H
#define _DLL_8_H

#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/envfx.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_8_newfog) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(Object *, Object *, EnvFxAction *, s32, u16);
/*1*/ void (*func1)(void);
/*2*/ void (*func2)(void);
/*3*/ void (*func3)(Gfx **);
/*4*/ UnknownDLLFunc func4;
/*5*/ void (*func5)(Gfx**);
/*6*/ void (*func6)(s32*, s32*, s32*);
/*7*/ UnknownDLLFunc func7;
/*8*/ UnknownDLLFunc func8;
/*9*/ UnknownDLLFunc func9;
/*10*/ UnknownDLLFunc func10;
/*11*/ UnknownDLLFunc func11;
/*12*/ UnknownDLLFunc func12;
};

#endif // _DLL_8_H
