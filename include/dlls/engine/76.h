#ifndef _DLLS_76_H
#define _DLLS_76_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll_def.h"
#include "types.h"
#include "unktypes.h"

DLL_INTERFACE_BEGIN(76)
    /*0*/ void (*func0)(s32, s32, s32, s32, s32, f32, u32*, s32, s32, s32, s32, s32);
    /*1*/ UnknownDLLFunc func1;
    /*2*/ void (*func2)(Gfx **gdl, Mtx **mtxs);
DLL_INTERFACE_END()

#endif //_DLLS_76_H
