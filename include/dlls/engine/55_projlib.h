#ifndef _DLL_55_H
#define _DLL_55_H

#include "game/objects/object.h"
#include "sys/math.h"
#include "dll_def.h"

// Temp DLL 2 (see main.c)
DLL_INTERFACE(DLL_55_projlib) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ s32 (*Func_18)(Object* obj, Vec3f* arg1, f32 arg2, f32 arg3, s32 arg4, Vec3f* arg5);
    /*1*/ s16 (*Func_304)(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 arg6, f32 arg7, s32 arg8);
    /*2*/ f32 (*Func_580)(s32 arg0, f32 arg1, f32 arg2);
};

#endif
