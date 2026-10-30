#ifndef _DLLS_57_H
#define _DLLS_57_H

#include "sys/lfx.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_57_trklight) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*Init)(void);
/*1*/ void (*DoAction)(LightAction* laction);
/*2*/ void (*GetLights)(s32* r1, s32* g1, s32* b1, s32* r2, s32* g2, s32* b2);
/*3*/ void (*Tick)(void);
};

#define dll_trklight (gDLL_57_trklight->vtbl)

#endif // _DLL_57_H
