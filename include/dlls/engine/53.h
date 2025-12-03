#ifndef _DLL_53_H
#define _DLL_53_H

#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

// Temp DLL 1 (see main.c)
DLL_INTERFACE(DLL_53) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*func0)(Object*, void*);
    /*1*/ void (*func1)(void*, Object*);
    /*2*/ void (*func2)(Object*, void*, s32, s32, s32);
    /*3*/ void (*func3)(Object*, void*, s32);
    /*4*/ s32 (*func4)(Object*, AnimObj_Data*, void*, s16, s32);
    /*5*/ void (*func5)(void*, s32, s32);
    /*6*/ void (*func6)(void*, void*, void*, s32);
    /*7*/ void (*func7)(s32, SRT*);
    /*8*/ UnknownDLLFunc func8;
    /*9*/ void (*func9)(s32, SRT*);
};

#endif // _DLL_53_H
