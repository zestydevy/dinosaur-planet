#ifndef _DLLS_58_H
#define _DLLS_58_H

#include "sys/lfx.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_58_skylight) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*Init)(void);
/*1*/ void (*DoAction)(LightAction* laction);
/*2*/ void (*Tick)(void);
/*3*/ void (*Func3)(s32);
/*4*/ void (*Func4)(s32, s32, s32, s32);
};

#define dll_skylight (gDLL_58_skylight->vtbl)

#endif // _DLL_58_H
