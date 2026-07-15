#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/99_camcannon.h"
#include "sys/memory.h"

typedef struct {
    Object* unk0;
} CamCannon;

/*0x0*/ static CamCannon* sState;

// offset: 0x0 | ctor
void camcannon_ctor(void* dll) { }

// offset: 0xC | dtor
void camcannon_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void camcannon_func_18(Cam* cam, s32 arg1, CamCannon_Params* data) {
    sState = mmAlloc(sizeof(CamCannon), ALLOC_TAG_CAM_COL, ALLOC_NAME("camcannon"));
    if (data != NULL) {
        sState->unk0 = data->unk0;
    } else {
        sState->unk0 = 0;
    }
    cam->srt.pitch = 0xAF0;
}

// offset: 0x94 | func: 1 | export: 1
void camcannon_func_94(Cam* cam) {
    if (sState->unk0 != NULL) {
        s16 temp_v0 = -sState->unk0->srt.yaw - cam->srt.yaw;
        cam->srt.yaw += ((f32) temp_v0 / 5.0f);
        cam->srt.transl.x = sState->unk0->srt.transl.x - (mathSinfInterp(sState->unk0->srt.yaw) * 60.0f);
        cam->srt.transl.y = sState->unk0->srt.transl.y + 80.0f;
        cam->srt.transl.z = sState->unk0->srt.transl.z - (mathCosfInterp(sState->unk0->srt.yaw) * 60.0f);
    }
}

// offset: 0x1CC | func: 2 | export: 2
void camcannon_func_1CC(Cam* cam) {

}

// offset: 0x1D8 | func: 3 | export: 3
void camcannon_func_1D8(void* arg0, s32 arg1) {

}
