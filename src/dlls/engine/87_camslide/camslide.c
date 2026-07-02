#include "dlls/engine/84_camnormal.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "dll.h"

// Active camera while riding a snowbike

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u16 unk1C;
} BSS0;

/*0x0*/ static BSS0* bss_0;

static void dll_87_func_41C(Cam* cam, Object* arg1);
static void dll_87_func_4E4(Cam* cam, Object* arg1);
static void dll_87_func_558(Cam* cam, f32 arg1, f32 arg2);
static void dll_87_func_5E8(Cam* cam, f32 arg1, f32 arg2);

// offset: 0x0 | ctor
void dll_87_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_87_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_87_func_18(Cam* cam, s32 arg1, void* action) {
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, ALLOC_NAME("camslide"));
    bss_0->unk18 = cam->fov;
    bss_0->unk0 = 80.0f;
    bss_0->unk14 = 1.0f;
}

// offset: 0x98 | func: 1 | export: 1
void dll_87_func_98(Cam* cam) {
    Object* sp64;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    CamControl_Module* sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;

    sp40 = gDLL_2_Camera->vtbl->get_camnormal_module();
    sp64 = cam->player;
    if (sp64 != NULL) {
        if (bss_0->unk14 < 1.0f) {
            ((DLL_84_camnormal*)sp40->dll)->vtbl->func7(&sp50, &sp4C, &sp48, &sp44, &sp3C);
            bss_0->unk4 = ((68.0f - sp50) * bss_0->unk14) + sp50;
            bss_0->unk8 = ((68.0f - sp4C) * bss_0->unk14) + sp4C;
            bss_0->unkC = ((48.0f - sp48) * bss_0->unk14) + sp48;
            bss_0->unk10 = ((48.0f - sp44) * bss_0->unk14) + sp44;
            bss_0->unk1C = (s32) (((1456.0f - sp3C) * bss_0->unk14) + (0, sp3C)); // partial @fake
            cam->fov = bss_0->unk18 + ((bss_0->unk0 - bss_0->unk18) * bss_0->unk14);
            bss_0->unk14 += gUpdateRateF * 0.01f;
            if (bss_0->unk14 > 1.0f) {
                bss_0->unk14 = 1.0f;
            }
        } else {
            bss_0->unk4 = 68.0f;
            bss_0->unk8 = 68.0f;
            bss_0->unkC = 48.0f;
            bss_0->unk10 = 48.0f;
            bss_0->unk1C = 0x5B0;
            cam->fov = bss_0->unk0;
        }
        dll_87_func_41C(cam, sp64);
        dll_87_func_4E4(cam, sp64);
        gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp60, &sp5C, &sp58, &sp54, 0.0f);
        dll_87_func_558(cam, sp60, sp58);
        dll_87_func_5E8(cam, sp5C, sp54);
        ((DLL_84_camnormal*)sp40->dll)->vtbl->func8(cam, 2, 0, &sp38, &sp34);
        if (cam->srt.transl.y < sp38 + 5.0f) {
            cam->srt.transl.y = sp38 + 5.0f;
        }
    }
}

// offset: 0x3CC | func: 2 | export: 2
void dll_87_func_3CC(Cam* cam) {
    mmFree(bss_0);
}

// offset: 0x40C | func: 3 | export: 3
void dll_87_func_40C(void* params, s32 arg1) {

}

// offset: 0x41C | func: 4
static void dll_87_func_41C(Cam* cam, Object* arg1) {
    s32 _pad;
    f32 temp_fv1;
    f32 temp;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;

    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp40, &sp3C, &sp38, &sp34, 0.0f);
    if (sp34 > bss_0->unk8) {

    } else if (sp34 < bss_0->unk4) {
        if (1){} // @fake
    }
    temp = bss_0->unk8;
    temp = sp34 - temp;
    temp_fv1 = (sp34 + temp) / sp34;
    if (temp_fv1 != 0.0f) {
        cam->srt.transl.x = (sp40 / temp_fv1) + arg1->srt.transl.x;
        cam->srt.transl.z = (sp38 / temp_fv1) + arg1->srt.transl.z;
    }
}

// offset: 0x4E4 | func: 5
static void dll_87_func_4E4(Cam* cam, Object* arg1) {
    f32 temp_fa0;
    f32 temp_fv0;
    f32 var_fv1;

    temp_fv0 = arg1->srt.transl.y + bss_0->unk10;
    temp_fa0 = arg1->srt.transl.y + bss_0->unkC;
    if (cam->srt.transl.y < temp_fa0) {
        var_fv1 = temp_fa0 - cam->srt.transl.y;
    } else if (temp_fv0 < cam->srt.transl.y) {
        var_fv1 = temp_fv0 - cam->srt.transl.y;
    } else {
        var_fv1 = 0.0f;
    }
    cam->srt.transl.y += var_fv1;
}

// offset: 0x558 | func: 6
static void dll_87_func_558(Cam* cam, f32 arg1, f32 arg2) {
    s32 var_v1 = (-arctan2_f(arg1, arg2) - (cam->srt.yaw & 0xFFFF)) + 0x8000;
    CIRCLE_WRAP(var_v1);
    cam->srt.yaw += var_v1;
}

// offset: 0x5E8 | func: 7
static void dll_87_func_5E8(Cam* cam, f32 arg1, f32 arg2) {
    s32 var_v1 = (arctan2_f(arg1, 100.0f) - bss_0->unk1C) - (cam->srt.pitch & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    cam->srt.pitch += ((s32) (var_v1 * gUpdateRate) >> 3);
}
