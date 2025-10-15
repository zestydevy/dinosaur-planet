#ifndef _DLLS_27_H
#define _DLLS_27_H

#include "PR/ultratypes.h"
#include "sys/objhits.h"
#include "sys/math.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

// size:0x260
typedef struct {
    u32 unk0;
    Vec3f *unk4;
    Vec3f unk8[4];
    Vec3f unk38[4];
    Unk80027934 unk68;
    Object *unkD4;
    Vec3f *unkD8;
    f32 *unkDC;
    Vec3f unkE0[4];
    Vec3f unk110[4];
    Func_80059C40_Struct unk140;
    s16 unk194;
    s16 unk196;
    s16 unk198;
    s16 unk19A;
    f32 unk19C;
    f32 unk1A0;
    f32 unk1A4;
    f32 unk1A8;
    f32 unk1AC;
    f32 unk1B0;
    f32 unk1B4;
    f32 unk1B8;
    f32 unk1BC[4];
    f32 unk1CC[4];
    f32 unk1DC[4];
    f32 unk1EC[4];
    f32 unk1FC[4];
    f32 unk20C[4];
    f32 unk21C[4];
    f32 unk22C[4];
    AABBs32 unk23C;
    u8 unk254;
    u8 _unk255[0x257 - 0x255];
    s8 unk257;
    u8 unk258;
    s8 unk259;
    u8 unk25A;
    u8 unk25B;
    s8 unk25C;
    s8 unk25D;
} DLL27_Data;

//Just a placeholder name - seems to have something to do with applying procedural head turn animations
DLL_INTERFACE(DLL_27_head_turn) {
    /*:*/ DLL_INTERFACE_BASE(DLL); 
    /*0*/ void (*head_turn_func_18)(DLL27_Data *data, s32 a1, s32 a2, s32 a3);
    /*1*/ void (*head_turn_func_84)(DLL27_Data *data, s32 a1, Vec3f *a2, f32 *a3, s32 a4);
    /*2*/ void (*head_turn_func_c0)(DLL27_Data *data, s32 arg1, Vec3f *arg2, f32 *arg3, u8 *arg4);
    /*3*/ void (*head_turn_func_1e8)(Object *object, DLL27_Data *data, f32 delayFloat);
    /*4*/ void (*head_turn_func_5a8)(Object *object, DLL27_Data *data);
    /*5*/ void (*head_turn_func_624)(Object *object, DLL27_Data *data, f32 delayFloat);
    /*6*/ Func_80057F1C_Struct* (*head_turn_func_c7c)(Object *object, f32 arg1, f32 arg2, s32 *arg3, s32 arg4);
    /*7*/ void (*head_turn_func_fb8)(Object *object, DLL27_Data *data);
    /*8*/ f32 (*head_turn_func_df4)(Object *object, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
};

#endif //_DLLS_27_H
