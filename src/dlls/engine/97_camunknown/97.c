#include "dlls/engine/2_camcontrol.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/memory.h"

// TODO: figure out what this camera is/when it's used

typedef struct {
    u8 _unk0[0x4 - 0x0];
    f32 unk4;
} BSS0;

/*0x0*/ static BSS0* bss_0;

// offset: 0x0 | ctor
void dll_97_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_97_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_97_func_18(Cam* cam, s32 arg1, void* action) {
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, NULL);
    bss_0->unk4 = 0.0f;
}

// offset: 0x74 | func: 1 | export: 1
void dll_97_func_74(Cam* cam) {
    Object* sp4C;
    f32 sp48;
    f32 pad_sp44;
    s32 pad_sp40;
    f32 var_fv0;
    f32 sp38;
    f32 sp28[4];
    s16 sp26;

    sp28[0] = 0.0f;
    sp28[2] = 0.0f;
    sp28[3] = 0.0f;
    sp28[1] = 1.0f;
    sp38 = curves_hermite(sp28, bss_0->unk4, NULL);
    sp4C = cam->player;
    sp26 = (0x8000 - sp4C->srt.yaw);
    sp26 += (s32) (14560.0f * sp38);
    sp48 = fcos16_precise(sp26);
    var_fv0 = fsin16_precise(sp26);
    cam->srt.transl.x = sp4C->srt.transl.x + ((20.0f * sp48) - (-10.0f * var_fv0));
    cam->srt.transl.z = sp4C->srt.transl.z + ((20.0f * var_fv0) + (-10.0f * sp48));
    cam->srt.transl.y = (sp4C->srt.transl.y + 35.0f) - (sp38 * 15.0f);
    cam->srt.pitch = 0x11C6 - (s32) (sp38 * 182.0f * 35.0f);
    // FAKE
    if (1){}
    cam->srt.yaw = sp26 + 0x1FFE;
    cam->srt.roll = 0;
    cam->letterboxGoal = 0;
    cam->fov = 60.0f;
    bss_0->unk4 += 0.005f * gUpdateRateF;
    if (bss_0->unk4 > 1.0f) {
        bss_0->unk4 = 1.0f;
    }
}

// offset: 0x250 | func: 2 | export: 2
void dll_97_func_250(Cam* cam) {
    mmFree(bss_0);
}

// offset: 0x290 | func: 3 | export: 3
void dll_97_func_290(void* arg0, s32 arg1) {

}
