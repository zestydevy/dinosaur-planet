#ifndef _DLL_57_H
#define _DLL_57_H

#include "sys/lfx.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_57) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ void (*func1)(LightAction* laction);
/*2*/ void (*func2)(s32* r1, s32* g1, s32* b1, s32* r2, s32* g2, s32* b2);
/*3*/ void (*func3)(void);
};

#endif // _DLL_57_H
