#include "PR/ultratypes.h"
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

void dll_94_ctor(s32 arg0) { }

void dll_94_dtor(s32 arg0) { }

void dll_94_func_18(TalkCamera* camA, s32 unused, TalkCamera* camB) {
    if (camB != NULL) {
        camA->camera.srt.transl.x = camB->camera.srt.transl.x;
        camA->camera.srt.transl.y = camB->camera.srt.transl.y;
        camA->camera.srt.transl.z = camB->camera.srt.transl.z;
        camA->camera.srt.yaw = camB->camera.srt.yaw;
        camA->camera.srt.pitch = camB->camera.srt.pitch;
        camA->camera.srt.roll = camB->camera.srt.roll;
        camA->fov = camB->fov;
    }
}

void dll_94_func_64(s32 arg0) { }

void dll_94_func_70(s32 arg0) { }

void dll_94_func_7C(s32 arg0, s32 arg1) { }
