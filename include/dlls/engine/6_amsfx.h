#ifndef _DLLS_6_H
#define _DLLS_6_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct UnkDE8 {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u16 pad8;
    u16 unkA;
    u16 unkC;
} UnkDE8;

DLL_INTERFACE(DLL_6_AMSFX) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*dll_6_func_338)(void);
    /*1*/ void (*dll_6_func_480)(Object *obj);
    /*2*/ s32 (*dll_6_func_48C)(Object* obj, u16 soundID, u8 volume, u32* arg3, char *arg4, s32 arg5, char *arg6);
    /*3*/ void (*dll_6_func_7E4)(u8 arg0);
    /*4*/ void (*dll_6_func_860)(s32 arg0, u8 arg1);
    /*5*/ void (*dll_6_func_954)(s32 arg0, f32 arg1);
    /*6*/ void (*dll_6_func_A1C)(s32 arg0);
    /*7*/ void (*dll_6_func_A6C)(Object *obj);
    /*8*/ s32 (*dll_6_func_B48)(u32 arg0);
    /*9*/ s32 (*dll_6_func_BBC)(Object* arg0, s32 arg1, s32 arg2, Object **arg3, s32 arg4);
    /*10*/ s32 (*dll_6_func_DCC)(s32 arg0, UNK_TYPE_32 arg1, s32* arg2, s32* arg3);
    /*11*/ void (*dll_6_func_DE8)(u16 arg0, UnkDE8* arg1);
    /*12*/ s32 (*dll_6_func_EE4)(s32 arg0, UNK_TYPE_32 arg1);
    /*13*/ void (*dll_6_func_F2C)(Object* arg0, u16 arg1, s32* arg2, char *arg3, s32 arg4);
    /*14*/ void (*dll_6_func_10D0)(Object* obj, u16 soundID, u32*);
    /*15*/ void (*dll_6_func_1174)(Object* arg0, u16 arg1, u8 arg2, u32* arg3);
    /*16*/ void (*dll_6_func_1218)(Object *obj);
    /*17*/ void (*dll_6_func_1504)(s32 arg0, Object* arg1, Object* arg2, f32 arg3);
    /*18*/ void (*dll_6_func_16E0)(void);
    /*19*/ void (*dll_6_func_1C10)(u8 arg0);
    // arg2 might be passed in as a f32
    /*20*/ void (*dll_6_func_1320)(Object* arg0, u16 arg1, s32 arg2, f32 arg3, f32 arg4, u32* arg5) ;
};

#endif //_DLLS_6_H
