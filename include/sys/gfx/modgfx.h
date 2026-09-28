#ifndef _SYS_GFX_MODGFX_H
#define _SYS_GFX_MODGFX_H

#include "dll_def.h"
#include "sys/objects.h"

typedef struct {
    s32 unk0;
    u16 pad4;
    s16 unk6;
    u16 pad8;
    s16 unkA;
    u16 padC;
    s16 unkE;
} ModgfxArg5;

// TODO: This might be the same interface for all modgfx DLLs
DLL_INTERFACE(DLL_IModgfx) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ s32 (*func0)(Object*, s32, SRT*, u32, s32, ModgfxArg5*);
};

#endif
