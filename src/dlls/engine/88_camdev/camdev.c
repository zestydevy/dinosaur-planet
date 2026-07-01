#include "common.h"

typedef struct {
    f32 zoom;
} BSS0;

/*0x0*/ static BSS0* bss_0;

// offset: 0x0 | ctor
void dll_88_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_88_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_88_func_18(CamControl_Data* camData, s32 arg1, void* action) {
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, NULL);
    bss_0->zoom = 50.0f;
}

// offset: 0x78 | func: 1 | export: 1
void dll_88_func_78(CamControl_Data* camData) {
    u16 sp56;
    f32 temp_fa0;
    f32 temp_ft4;
    f32 temp_fv1;
    f32 temp_fa1;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    u16 var_v0;
    u16 var_v1;
    Object* sp30;

    sp30 = camData->player;
    sp56 = joy_get_buttons(0);
    if (joy_get_pressed(0) & L_TRIG) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, 0, 1, 0, NULL, 0, 0xFF);
        return;
    }
    if (sp56 & U_JPAD) {
        bss_0->zoom -= 8.0f;
    }
    if (sp56 & D_JPAD) {
        bss_0->zoom += 8.0f;
    }
    if (bss_0->zoom < 20.0f) {
        bss_0->zoom = 20.0f;
    }
    if (bss_0->zoom > 2000.0f) {
        bss_0->zoom = 2000.0f;
    }
    temp_fa1 = 0; // @fake
    var_v0 = 0;
    var_v1 = 0;
    if (sp56 & R_CBUTTONS) {
        var_v0 -= gUpdateRate * 0x5A;
    }
    if (sp56 & L_CBUTTONS) {
        var_v0 += gUpdateRate * 0x5A;
    }
    if (sp56 & U_CBUTTONS) {
        var_v1 += gUpdateRate * 0x5A;
    }
    if (sp56 & D_CBUTTONS) {
        var_v1 -= gUpdateRate * 0x5A;
    }
    camData->srt.yaw += var_v0;
    camData->srt.pitch += var_v1;
    sp40 = fsin16_precise((s16) (camData->srt.yaw - 0x4000));
    sp3C = fcos16_precise((s16) (camData->srt.yaw - 0x4000));
    sp38 = fcos16_precise(camData->srt.pitch);
    temp_ft4 = fsin16_precise(camData->srt.pitch);
    temp_ft4 = temp_ft4 * bss_0->zoom;
    temp_fv1 = bss_0->zoom * sp38;
    temp_fa1 = temp_fv1 * sp3C;
    temp_fv1 *= sp40;
    camData->srt.transl.x = sp30->globalPosition.x + (temp_fa1);
    camData->srt.transl.y = sp30->globalPosition.y + 20/*.0f*/ + temp_ft4;
    camData->srt.transl.z = sp30->globalPosition.z + (temp_fv1);
}

// offset: 0x340 | func: 2 | export: 2
void dll_88_func_340(CamControl_Data* camData) {
    mmFree(bss_0);
}

// offset: 0x380 | func: 3 | export: 3
void dll_88_func_380(void* arg0, s32 arg1) {

}
