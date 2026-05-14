#ifndef _DLLS_24_H
#define _DLLS_24_H

#include "PR/ultratypes.h"
#include "dlls/engine/27.h"
#include "sys/math.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_24_waterfx) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*tick)(u8 updateRate); // @bug: does not match signature of implementation
/*1*/ void (*func_564)(SRT *transform, u16 arg1, Vec3f *arg2, DLL27_Data *arg3, f32 arg4);
/*2*/ void (*print)(Gfx** gdl, Mtx** arg1);
/*3*/ void (*spawn_splash)(f32 x, f32 y, f32 z, f32 size);
/*4*/ void (*spawn_circular_ripple)(f32 x, f32 y, f32 z, s16 yaw, f32 arg4, s32 decayRate);
/*5*/ void (*spawn_movement_ripple)(f32 x, f32 y, f32 z, s16 yaw, f32 arg4);
/*6*/ void (*init)(void);
/*7*/ void (*set_circular_ripple_scale)(s32 useDefault, f32 scale);
};

#endif //_DLLS_24_H
