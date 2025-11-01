#ifndef _DLLS_24_H
#define _DLLS_24_H

#include "PR/ultratypes.h"
#include "dlls/engine/27.h"
#include "sys/math.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_24_waterfx) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func_6E8)();
/*1*/ void (*func_564)(SRT *transform, u16 arg1, Vec3f *arg2, DLL27_Data *arg3, f32 arg4);
/*2*/ void (*func_C7C)(Gfx** gdl, Mtx** arg1);
/*3*/ void (*func_174C)(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
/*4*/ void (*func_1CC8)(f32 arg0, f32 arg1, f32 arg2, s16 arg3, f32 arg4, s32 arg5);
/*5*/ void (*func_1B28)(f32 arg0, f32 arg1, f32 arg2, s16 arg3, f32 arg4);
/*6*/ void (*func_24C)(void);
/*7*/ void (*func_1C88)(s32 arg0, f32 arg1);
};

#endif //_DLLS_24_H
