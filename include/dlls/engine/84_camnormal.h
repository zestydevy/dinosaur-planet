#ifndef _DLLS_84_H
#define _DLLS_84_H

#include "PR/ultratypes.h"
#include "dlls/engine/2_camcontrol.h"
#include "dll_def.h"

// TODO: this is partially a CameraAction?
typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    s8 unk5;
    u8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 _unkD[0x19 - 0xD];
    u8 unk19;
    u16 unk1A;
    u16 unk1C;
    u8 unk1E;
    u8 unk1F;
} CamNormal_Params;

DLL_INTERFACE(DLL_84_camnormal) {
/*:*/ DLL_INTERFACE_BASE(DLL_ICamControlModule);
/*4*/ void (*func4)(Cam* cam, Object* arg1);
/*5*/ void (*func5)(Cam* cam, f32 arg1, f32 arg2);
/*6*/ void (*func6)(Cam* cam, Object* arg1, f32 arg2, f32 arg3);
/*7*/ void (*func7)(f32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4);
/*8*/ void (*func8)(Cam* cam, s32 arg1, s32 arg2, f32* arg3, f32* arg4);
};

#endif
