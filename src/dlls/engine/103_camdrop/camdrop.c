#include "dlls/engine/2_camcontrol.h"
#include "sys/main.h"
#include "sys/memory.h"

typedef struct {
    f32 unk0;
    f32 unk4;
} BSS0;

/*0x0*/ static BSS0* bss_0;

// offset: 0x0 | ctor
void dll_103_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_103_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_103_func_18(CamControl_Data* camData, s32 arg1, void* action) {
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, ALLOC_NAME("camdrop"));
    bss_0->unk0 = 100.0f;
    bss_0->unk4 = camData->player->srt.transl.y - 200.0f;
}

// offset: 0x98 | func: 1 | export: 1
void dll_103_func_98(CamControl_Data* camData) {
    Object* sp24 = camData->player;
    bss_0->unk0 -= 0.2f * gUpdateRateF;
    if (bss_0->unk0 < 20.0f) {
        bss_0->unk0 = 20.0f;
    }

    camData->srt.transl.x = sp24->srt.transl.x - (fsin16_precise(sp24->srt.yaw) * 5.0f);
    camData->srt.transl.y = bss_0->unk4;
    camData->srt.transl.z = sp24->srt.transl.z - (fcos16_precise(sp24->srt.yaw) * 5.0f);
    camData->srt.yaw = 0;
    camData->srt.pitch = -0x4000;
    camData->srt.roll = 0;
}

// offset: 0x1A4 | func: 2 | export: 2
void dll_103_func_1A4(CamControl_Data* camData) {
    mmFree(bss_0);
}

// offset: 0x1E4 | func: 3 | export: 3
void dll_103_func_1E4(void* arg0, s32 arg1) {

}
