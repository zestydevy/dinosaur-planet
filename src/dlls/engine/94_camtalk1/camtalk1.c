#include "dlls/engine/94_camtalk1.h"

void dll_94_ctor(s32 arg0) {
}

void dll_94_dtor(s32 arg0) {
}

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

void dll_94_func_64(UNK_TYPE_32 arg0) {
}

void dll_94_func_70(UNK_TYPE_32 arg0) {
}

void dll_94_func_7C(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1) {
}
