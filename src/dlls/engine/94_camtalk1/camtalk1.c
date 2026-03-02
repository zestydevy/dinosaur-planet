#include "dlls/engine/94_camtalk1.h"

#include "dlls/engine/2_camcontrol.h"

// offset: 0x0 | ctor
void dll_94_ctor(s32 arg0) { }

// offset: 0xC | dtor
void dll_94_dtor(s32 arg0) { }

// offset: 0x18 | func: 0 | export: 0
void dll_94_func_18(TalkCamera* camA, UNK_TYPE_32 unused, TalkCamera* camB) {
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

// offset: 0x64 | func: 1 | export: 1
void dll_94_func_64(UNK_TYPE_32 arg0) { }

// offset: 0x70 | func: 2 | export: 2
void dll_94_func_70(UNK_TYPE_32 arg0) { }

// offset: 0x7C | func: 3 | export: 3
void dll_94_func_7C(CameraAction* camAction, s32 arg1) { }
