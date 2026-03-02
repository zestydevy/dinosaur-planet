#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/94_camtalk1.h"
#include "unktypes.h"

// offset: 0x0 | ctor
void camtalk1_ctor(s32 arg0) { }

// offset: 0xC | dtor
void camtalk1_dtor(s32 arg0) { }

// offset: 0x18 | func: 0 | export: 0
void camtalk1_setup(CamControl_Data* camData, UNK_TYPE_32 unused, CamControl_Data* arg2) {
    if (arg2 != NULL) {
        camData->srt.transl.x = arg2->srt.transl.x;
        camData->srt.transl.y = arg2->srt.transl.y;
        camData->srt.transl.z = arg2->srt.transl.z;
        camData->srt.yaw = arg2->srt.yaw;
        camData->srt.pitch = arg2->srt.pitch;
        camData->srt.roll = arg2->srt.roll;
        camData->fov = arg2->fov;
    }
}

// offset: 0x64 | func: 1 | export: 1
void camtalk1_control(CamControl_Data* camData) { }

// offset: 0x70 | func: 2 | export: 2
void camtalk1_free(CamControl_Data* camData) { }

// offset: 0x7C | func: 3 | export: 3
void camtalk1_func_7C(UNK_PTR* arg0, s32 arg1) { }
