#ifndef _DLLS_13_EXPGFX_H__
#define _DLLS_13_EXPGFX_H__

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE_BEGIN(13_expgfx)
    /*0*/ void (*func0)(void);
    /*1*/ UnknownDLLFunc func1;
    /*2*/ void (*func2)(s32 arg0, u8 arg1, s32 arg2, s32 arg3);
    /*3*/ UnknownDLLFunc func3;
    /*4*/ UnknownDLLFunc func4;
    /*5*/ UnknownDLLFunc func5;
    /*6*/ void (*func6)(Object *arg0, Gfx **gdl, Mtx **mtxs, Vertex **vertices, s32 arg4, s32 arg5, s32 arg6);
    /*7*/ UnknownDLLFunc func7;
    /*8*/ UnknownDLLFunc func8;
    /*9*/ void (*func9)(s32 arg0);
    /*10*/ s8 (*func10)(Object *obj);
    /*11*/ UnknownDLLFunc func11;
    /*12*/ UnknownDLLFunc func12;
    /*13*/ s32 (*func13)(Object *obj); // This is not be correct, expgfx only has 10 exported funcs but I don't know where this belongs
    // Logs of expgfx funcs are used in func_800436DC which also calls an object dll at func 13
DLL_INTERFACE_END()

#endif //_DLLS_13_EXPGFX_H__
