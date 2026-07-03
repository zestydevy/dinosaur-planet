#include "dlls/engine/2_camcontrol.h"

// Active camera during non-talk sequences

// offset: 0x0 | ctor
void camseq_ctor(s32 arg0) { }

// offset: 0xC | dtor
void camseq_dtor(s32 arg0) { }

// offset: 0x18 | func: 0 | export: 0
void camseq_setup(Cam* cam, s32 arg1, Cam* data) {
    if (data != NULL) {
        cam->srt.transl.x = data->srt.transl.x;
        cam->srt.transl.y = data->srt.transl.y;
        cam->srt.transl.z = data->srt.transl.z;
        cam->srt.yaw = data->srt.yaw;
        cam->srt.pitch = data->srt.pitch;
        cam->srt.roll = data->srt.roll;
        cam->fov = data->fov;
    }
}

// offset: 0x64 | func: 1 | export: 1
void camseq_control(Cam* cam) { }

// offset: 0x70 | func: 2 | export: 2
void camseq_free(Cam* cam) { }

// offset: 0x7C | func: 3 | export: 3
void camseq_func_7C(void* arg0, s32 arg1) { }
