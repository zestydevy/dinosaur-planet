#include "dlls/engine/2_camcontrol.h"
#include "sys/main.h"
#include "sys/memory.h"

typedef struct {
    f32 unk0;
    f32 unk4;
} CamDrop;

/*0x0*/ static CamDrop* sState;

// offset: 0x0 | ctor
void dll_103_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_103_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_103_func_18(Cam* cam, s32 arg1, void* data) {
    sState = mmAlloc(sizeof(CamDrop), ALLOC_TAG_CAM_COL, ALLOC_NAME("camdrop"));
    sState->unk0 = 100.0f;
    sState->unk4 = cam->player->srt.transl.y - 200.0f;
}

// offset: 0x98 | func: 1 | export: 1
void dll_103_func_98(Cam* cam) {
    Object* sp24 = cam->player;
    sState->unk0 -= 0.2f * gUpdateRateF;
    if (sState->unk0 < 20.0f) {
        sState->unk0 = 20.0f;
    }

    cam->srt.transl.x = sp24->srt.transl.x - (fsin16_precise(sp24->srt.yaw) * 5.0f);
    cam->srt.transl.y = sState->unk4;
    cam->srt.transl.z = sp24->srt.transl.z - (fcos16_precise(sp24->srt.yaw) * 5.0f);
    cam->srt.yaw = 0;
    cam->srt.pitch = -0x4000;
    cam->srt.roll = 0;
}

// offset: 0x1A4 | func: 2 | export: 2
void dll_103_func_1A4(Cam* cam) {
    mmFree(sState);
}

// offset: 0x1E4 | func: 3 | export: 3
void dll_103_func_1E4(void* arg0, s32 arg1) {

}
