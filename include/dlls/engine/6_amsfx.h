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
    /*0*/ void (*func_338)(void);
    /*1*/ void (*func_480)(Object *obj);
    // Play sound using source object for panning and distance falloff
    /*2*/ u32 (*play_sound)(Object* obj, u16 soundID, u8 volume, u32* arg3, char *arg4, s32 arg5, char *arg6);
    /*3*/ void (*func_7E4)(u8 arg0);
    /*4*/ void (*func_860)(u32 arg0, u8 arg1);
    /*5*/ void (*func_954)(u32 arg0, f32 arg1);
    /*6*/ void (*func_A1C)(u32 arg0);
    /*7*/ void (*func_A6C)(Object *obj);
    /*8*/ s32 (*func_B48)(u32 arg0);
    /*9*/ s32 (*func_BBC)(Object* arg0, s32 arg1, s32 arg2, Object **arg3, s32 arg4);
    /*10*/ s32 (*func_DCC)(s32 arg0, UNK_TYPE_32 arg1, s32* arg2, s32* arg3);
    /*11*/ void (*func_DE8)(u16 arg0, UnkDE8* arg1);
    /*12*/ s32 (*func_EE4)(s32 arg0, UNK_TYPE_32 arg1);
    /*13*/ void (*func_F2C)(Object* arg0, u16 arg1, u32* arg2, char *arg3, s32 arg4);
    /*14*/ void (*func_10D0)(Object* obj, u16 soundID, u32*);
    /*15*/ void (*func_1174)(Object* arg0, u16 arg1, u8 arg2, u32* arg3);
    /*16*/ void (*func_1218)(Object *obj);
    /*17*/ void (*func_1504)(s32 arg0, Object* arg1, Object* arg2, f32 arg3);
    /*18*/ void (*func_16E0)(void);
    /*19*/ void (*water_falls_set_flags)(u8 arg0);
    // arg2 might be passed in as a f32
    /*20*/ void (*func_1320)(Object* arg0, u16 arg1, s32 arg2, f32 arg3, f32 arg4, u32* arg5) ;
};

#endif //_DLLS_6_H
