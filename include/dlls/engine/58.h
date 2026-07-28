#ifndef _DLL_58_H
#define _DLL_58_H

#include "sys/lfx.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_58) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ void (*func1)(LightAction *);
/*2*/ void (*func2)(void);
/*3*/ void (*func3)(s32);
/*4*/ void (*func4)(s32, s32, s32, s32);
};

#endif // _DLL_58_H
