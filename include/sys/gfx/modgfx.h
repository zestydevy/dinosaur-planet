#ifndef _SYS_GFX_MODGFX_H
#define _SYS_GFX_MODGFX_H

#include "dll_def.h"
#include "sys/objects.h"

DLL_INTERFACE(DLL_IModgfx) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ s32 (*Spawn)(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data);
};

#endif
