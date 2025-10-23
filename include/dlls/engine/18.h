#ifndef _DLL_18_H
#define _DLL_18_H

#include "dll_def.h"
#include "dlls/engine/27.h"
#include "game/objects/object.h"
#include "types.h"

typedef struct {
    u8 _unk0[2];
    u16 soundID;
} DLL18_Func_11BC_Struct;

struct DLL18_Data;

typedef s16 (*dll18_callback)(Object*, struct DLL18_Data*, f32);
typedef void (*dll18_callback2)(Object*, struct DLL18_Data*);

// size:0x34C
typedef struct DLL18_Data {
    u32 unk0;
    DLL27_Data unk4;
    s16 unk264;
    s16 unk266;
    s16 unk268;
    s16 unk26A;
    s16 unk26C;
    s16 unk26E;
    s16 unk270;
    s8 unk272;
    s8 unk273;
    Vec3f *unk274;
    f32 unk278;
    f32 unk27C;
    f32 unk280;
    f32 unk284;
    f32 unk288;
    f32 unk28C;
    f32 unk290;
    f32 unk294;
    f32 unk298;
    f32 unk29C;
    f32 unk2A0;
    f32 unk2A4;
    s32 unk2A8;
    f32 unk2AC;
    f32 unk2B0;
    f32 unk2B4;
    f32 unk2B8;
    u8 _unk2BC[0x2C8 - 0x2BC];
    Object *unk2C8;
    u8 _unk2CC[0x2EC - 0x2CC];
    Vec3f unk2EC;
    f32 unk2F8;
    dll18_callback2 unk2FC;
    u8 _unk300[0x304 - 0x300];
    s32 unk304;
    u32 unk308;
    s32 unk30C;
    s32 unk310;
    u8 _unk314[0x322 - 0x314];
    s16 unk322;
    s16 unk324;
    s16 unk326;
    s16 unk328;
    s16 unk32A;
    s16 unk32C;
    s16 unk32E;
    s32 unk330;
    s32 unk334;
    s8 unk338;
    s8 unk339;
    s8 unk33A;
    u8 _unk33B[0x33D - 0x33B];
    u8 unk33D;
    u8 _unk33E;
    u8 unk33F;
    s8 unk340;
    s8 unk341;
    u8 unk342;
    s8 unk343;
    u8 _unk344[0x348 - 0x344];
    s8 unk348;
    u8 _unk349;
    u8 unk34A;
    u8 _unk34B;
} DLL18_Data;

DLL_INTERFACE(DLL_18) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(Object *obj, DLL18_Data *data, s32 arg2, s32 arg3);
/*1*/ void (*func1)(Object *obj, DLL18_Data *data, f32 arg2, f32 arg3, dll18_callback *arg4, dll18_callback *arg5);
/*2*/ void (*func2)(Object *obj, DLL18_Data *data, dll18_callback *arg2);
/*3*/ void (*func3)(SRT *srt);
/*4*/ void (*func4)(Object *obj, DLL18_Data *data, s32 arg2);
/*5*/ void (*func5)(Object *obj, DLL18_Data *data, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
/*6*/ void (*func6)(Object *obj, DLL18_Data *data, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
/*7*/ void (*func7)(Object *obj, DLL18_Data *data, f32 arg2, s32 arg3);
/*8*/ void (*func8)(Object *obj, DLL18_Data *data, f32 arg2, f32 arg3, f32 arg4);
/*9*/ void (*func9)(Object *obj, DLL18_Data *data, f32 arg2, f32 arg3);
/*10*/ void (*func10)(Object *obj, DLL18_Data *data, f32 arg2, f32 arg3);
/*11*/ void (*func11)(Object *obj, DLL18_Data *data, s32 arg2, s32 arg3);
/*12*/ void (*func12)(Object *obj, DLL18_Data *data, s32 arg2, s32 arg3, DLL18_Func_11BC_Struct *arg4);
/*13*/ void (*func13)(Object *obj, DLL18_Data *data, s32 arg2, s32 arg3, DLL18_Func_11BC_Struct *arg4, f32 arg5, u8 volume);
/*14*/ void (*func14)(Object *obj, DLL18_Data *data, s32 arg2);
/*15*/ void (*func15)(Object *obj, DLL18_Data *data, f32 arg2, f32 arg3);
/*16*/ void (*func16)(Object *obj, DLL18_Data *data, f32 arg2, f32 arg3);
/*17*/ void (*func17)(Object *obj, DLL18_Data *data);
/*18*/ void (*func18)(Object *obj, DLL18_Data *data, s32 arg2, s32 arg3);
/*19*/ void (*func19)(Object *obj, DLL18_Data *data, s32 arg2, s32 arg3);
/*20*/ void (*func20)(Object *obj, DLL18_Data *data, s32 arg2, s32 arg3, u32 arg5, u32 arg6);
/*21*/ void (*func21)(Object *obj, DLL18_Data *data, s32 arg2, s32 arg3, u32 arg4);
/*22*/ void (*func22)(DLL18_Data *data, void *arg1);
};

#endif // _DLL_18_H
