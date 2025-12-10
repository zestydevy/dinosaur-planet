#ifndef _DLL_14_H
#define _DLL_14_H

#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"
#include "PR/gbi.h"

DLL_INTERFACE(DLL_14_modgfx) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ UnknownDLLFunc func1;
/*2*/ void (*func2)(s32 arg0, s32 arg1, s32 arg2);
/*3*/ UnknownDLLFunc func3;
/*4*/ void (*func4)(Object *obj);
/*5*/ void (*func5)(Object *obj);
/*6*/ void (*func6)(Gfx **, Mtx **, Vertex **, s32, Object *);
/*7*/ void (*func7)(s16*);
/*8*/ UnknownDLLFunc func8;
/*9*/ UnknownDLLFunc func9;
/*10*/ void (*func10)(Object*);
/*11*/ UnknownDLLFunc func11;
/*12*/ UnknownDLLFunc func12;
};

#endif // _DLL_14_H
