#ifndef _SYS_GFX_MODGFX_H
#define _SYS_GFX_MODGFX_H

#include "dll_def.h"
#include "sys/objects.h"

// TODO: This might be the same interface for all modgfx DLLs
DLL_INTERFACE(DLL_IModgfx) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ s32 (*func0)(Object*, s32, SRT*, s32, s32, void*);
};

#endif
