#ifndef _DLLS_12_H
#define _DLLS_12_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_12_minic) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*func0)(Object*,Object*,void*,s32,u16);
    /*1*/ void (*func1)(void);
    /*2*/ void (*func2)(void);
    /*3*/ UnknownDLLFunc func3;
    /*4*/ void (*func4)(void);
    /*5*/ UnknownDLLFunc func5;
    /*6*/ void (*func6)(u8);
    /*7*/ UnknownDLLFunc func7;
    /*8*/ UnknownDLLFunc func8;
    /*9*/ UnknownDLLFunc func9;
};

#endif //_DLLS_12_H
