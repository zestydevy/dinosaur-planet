#ifndef _DLLS_27_H
#define _DLLS_27_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "types.h"
#include "sys/math.h"
#include "game/objects/object.h"

//Just a placeholder name - seems to have something to do with applying procedural head turn animations
DLL_INTERFACE(DLL_27_head_turn) {
    /*:*/ DLL_INTERFACE_BASE(DLL); 
    /*0*/ void (*head_turn_func_18)(Vec3f*, s32, s32, s32);
    /*1*/ UnknownDLLFunc head_turn_func_84;
    /*2*/ void (*head_turn_func_c0)(Vec3f*, s32, s32, s32, s32);
    /*3*/ void (*head_turn_func_1e8)(Object* object, Vec3f* vector, f32 delayFloat);
    /*4*/ void (*head_turn_func_5a8)(Object* object, Vec3f* vector);
    /*5*/ void (*head_turn_func_624)(Object* object, Vec3f* vector, f32 delayFloat);
    /*6*/ UnknownDLLFunc head_turn_func_c7c;
    /*7*/ void (*head_turn_func_fb8)(Object* object, void*);
    /*8*/ UnknownDLLFunc head_turn_func_df4;
};

#endif //_DLLS_27_H
