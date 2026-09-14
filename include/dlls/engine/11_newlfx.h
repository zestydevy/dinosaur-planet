#ifndef _DLL_11_H
#define _DLL_11_H

#include "dll_def.h"
#include "game/objects/object.h"
#include "sys/lfx.h"

// a "tmove" ?
typedef struct {
    Vec3f unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} NewLfxStruct;

DLL_INTERFACE(DLL_11_newlfx) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*DoAction)(Object* arg0, Object* arg1, LightAction* action, s32 arg3, s32 arg4, SRT* arg5);
/*1*/ void (*Free)(void);
/*2*/ void (*Tick)(void);
/*3*/ NewLfxStruct* (*Func_828)(s32 arg0, s8* count);
/*4*/ f32 (*FindClosestLight)(Vec3f* pos, f32 maxDist);
/*5*/ f32 (*Func_9D8)(s32 arg0, f32* arg1);
/*6*/ s32 (*Func_BB8)(s32 arg0, s16 arg1, s16 arg2);
};

#define dll_newlfx (gDLL_11_Newlfx->vtbl)

#endif // _DLL_11_H
