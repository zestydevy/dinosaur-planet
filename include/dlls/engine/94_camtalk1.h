#ifndef _DLLS_94_H
#define _DLLS_94_H

#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"
#include "sys/camera.h"

typedef struct {
/*0000*/    Camera camera;
/*0060*/    s32 unk60;
/*0064*/    s32 unk64;
/*0068*/    s32 unk68;
/*006C*/    s32 unk6C;
/*0070*/    s32 unk70;
/*0074*/    s32 unk74;
/*0078*/    s32 unk78;
/*007C*/    s32 unk7C;
/*0080*/    s32 unk80;
/*0084*/    s32 unk84;
/*0088*/    s32 unk88;
/*008C*/    s32 unk8C;
/*0090*/    s32 unk90;
/*0094*/    s32 unk94;
/*0098*/    s32 unk98;
/*009C*/    s32 unk9C;
/*00A0*/    Object* aimAtObject;
/*00A4*/    f32 unkA4; //x
/*00A8*/    f32 unkA8; //y
/*00AC*/    f32 unkAC; //z
/*00AC*/    f32 fov;
} TalkCamera;

DLL_INTERFACE(DLL_94_camtalk1) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(TalkCamera* camA, UNK_TYPE_32 unused, TalkCamera* camB);
/*1*/ void (*func1)(UNK_TYPE_32 arg0);
/*2*/ void (*func2)(UNK_TYPE_32 arg0);
/*3*/ void (*func3)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1);
};

#endif //_DLLS_94_H
