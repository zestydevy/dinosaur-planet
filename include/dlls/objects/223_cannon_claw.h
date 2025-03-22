#ifndef _DLLS_223_H
#define _DLLS_223_H

#include <PR/ultratypes.h>
#include "game/objects/object.h"
#include "dll_def.h"

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ u8 _unk18[0x1a - 0x18];
/*1A*/ s16 bitstringEntry;
} CannonClawCreateInfo;

DLL_INTERFACE_BEGIN(223_cannon_claw)
    void (*func_18)(Object *a0, Object *a1, void *a2);
    void (*func_4C)(Object *s0);
    void (*func_200)(void *a0);
    void (*func_20C)(Object *a0, s32 a1, s32 a2, s32 a3, s32 a4, s8 a5);
    void (*func_26C)(void *a0, void *a1);
    s32 (*func_27C)(void *a0);
    s32 (*func_28C)(void *a0, void *a1);
DLL_INTERFACE_END()

#endif //_DLLS_223_H
