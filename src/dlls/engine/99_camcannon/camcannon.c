#include "dlls/engine/2_camcontrol.h"
#include "sys/memory.h"

typedef struct {
    Object* unk0;
} CamCannon_Params;

typedef struct {
    Object* unk0;
} BSS0;

/*0x0*/ static BSS0* bss_0;

// offset: 0x0 | ctor
void dll_99_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_99_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_99_func_18(Cam* cam, s32 arg1, CamCannon_Params* action) {
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, ALLOC_NAME("camcannon"));
    if (action != NULL) {
        bss_0->unk0 = action->unk0;
    } else {
        bss_0->unk0 = 0;
    }
    cam->srt.pitch = 0xAF0;
}

// offset: 0x94 | func: 1 | export: 1
void dll_99_func_94(Cam* cam) {
    if (bss_0->unk0 != NULL) {
        s16 temp_v0 = -bss_0->unk0->srt.yaw - cam->srt.yaw;
        cam->srt.yaw += ((f32) temp_v0 / 5.0f);
        cam->srt.transl.x = bss_0->unk0->srt.transl.x - (fsin16_precise(bss_0->unk0->srt.yaw) * 60.0f);
        cam->srt.transl.y = bss_0->unk0->srt.transl.y + 80.0f;
        cam->srt.transl.z = bss_0->unk0->srt.transl.z - (fcos16_precise(bss_0->unk0->srt.yaw) * 60.0f);
    }
}

// offset: 0x1CC | func: 2 | export: 2
void dll_99_func_1CC(Cam* cam) {

}

// offset: 0x1D8 | func: 3 | export: 3
void dll_99_func_1D8(CamCannon_Params* arg0, s32 arg1) {

}
