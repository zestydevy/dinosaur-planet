#ifndef _SYS_GFX_PROJGFX_H
#define _SYS_GFX_PROJGFX_H

#include "dll_def.h"
#include "sys/objects.h"

DLL_INTERFACE(DLL_IProjgfx) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ s32 (*func0)(Object*, s32, SRT*, s32, s32, s32, s32);
};

#endif
