#include "dlls/engine/2_camcontrol.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "dll.h"

typedef struct {
    f32 zoom;
} CamDev;

/*0x0*/ static CamDev* sState;

// offset: 0x0 | ctor
void dll_88_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_88_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_88_func_18(Cam* cam, s32 arg1, void* data) {
    sState = mmAlloc(sizeof(CamDev), ALLOC_TAG_CAM_COL, ALLOC_NAME("camdev"));
    sState->zoom = 50.0f;
}

// offset: 0x78 | func: 1 | export: 1
void dll_88_func_78(Cam* cam) {
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

    sp30 = cam->player;
    sp56 = joy_get_buttons(0);
    if (joy_get_pressed(0) & L_TRIG) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
        return;
    }
    if (sp56 & U_JPAD) {
        sState->zoom -= 8.0f;
    }
    if (sp56 & D_JPAD) {
        sState->zoom += 8.0f;
    }
    if (sState->zoom < 20.0f) {
        sState->zoom = 20.0f;
    }
    if (sState->zoom > 2000.0f) {
        sState->zoom = 2000.0f;
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
    cam->srt.yaw += var_v0;
    cam->srt.pitch += var_v1;
    sp40 = fsin16_precise((s16) (cam->srt.yaw - 0x4000));
    sp3C = fcos16_precise((s16) (cam->srt.yaw - 0x4000));
    sp38 = fcos16_precise(cam->srt.pitch);
    temp_ft4 = fsin16_precise(cam->srt.pitch);
    temp_ft4 = temp_ft4 * sState->zoom;
    temp_fv1 = sState->zoom * sp38;
    temp_fa1 = temp_fv1 * sp3C;
    temp_fv1 *= sp40;
    cam->srt.transl.x = sp30->globalPosition.x + (temp_fa1);
    cam->srt.transl.y = sp30->globalPosition.y + 20/*.0f*/ + temp_ft4;
    cam->srt.transl.z = sp30->globalPosition.z + (temp_fv1);
}

// offset: 0x340 | func: 2 | export: 2
void dll_88_func_340(Cam* cam) {
    mmFree(sState);
}

// offset: 0x380 | func: 3 | export: 3
void dll_88_func_380(void* arg0, s32 arg1) {

}
