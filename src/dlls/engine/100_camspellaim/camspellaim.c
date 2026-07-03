#include "dlls/engine/2_camcontrol.h"
#include "dlls/objects/210_player.h"

/*0x0*/ static f32 data_0 = 37.0f;

/*0x0*/ static f32 bss_0;
/*0x4*/ static f32 bss_4;

static void dll_100_func_2E4(Cam* cam, Object* arg1, f32* arg2);

// offset: 0x0 | ctor
void dll_100_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_100_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_100_func_18(Cam* cam, s32 arg1, void* data) {
    dll_100_func_2E4(cam, cam->player, &bss_0);
}

// offset: 0x64 | func: 1 | export: 1
void dll_100_func_64(Cam* cam) {
    Object* sp4C;
    f32 temp_ft5;
    f32 temp_fv1;
    f32 var_f12;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 temp;
    s16 sp2E;
    s16 sp2C;
    f32 sp20[3];

    sp4C = cam->player;
    var_f12 = bss_0;
    cam->highlightFlags |= 2;
    bss_4 = var_f12;
    ((DLL_210_Player*)sp4C->dll)->vtbl->func62(sp4C, &sp2E, &sp2C);
    sp2E = ((sp2E >> 1) - sp4C->srt.yaw) + 0x8000;
    sp2C = (s16) (sp2C >> 1);
    sp20[2] = sp4C->srt.transl.x;
    sp20[1] = sp4C->srt.transl.y + data_0;
    sp20[0] = sp4C->srt.transl.z;
    sp2E -= (cam->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(sp2E);
    cam->srt.yaw += (s32) (sp2E * gUpdateRate) >> 3;
    sp2C -= (cam->srt.pitch & 0xFFFF);
    CIRCLE_WRAP(sp2C);
    cam->srt.pitch += (s32) (sp2C * gUpdateRate) >> 3;
    sp3C = fsin16_precise((s16) (cam->srt.yaw - 0x4000));
    sp38 = fcos16_precise((s16) (cam->srt.yaw - 0x4000));
    sp34 = fcos16_precise(cam->srt.pitch);
    temp = fsin16_precise(cam->srt.pitch);
    var_f12 = bss_4;
    temp_fv1 = var_f12 * sp34;
    cam->srt.transl.x = sp20[2] + (temp_fv1 * sp38);
    cam->srt.transl.y = sp20[1] + (var_f12 * temp);
    cam->srt.transl.z = sp20[0] + (temp_fv1 * sp3C);
}

// offset: 0x2C8 | func: 2 | export: 2
void dll_100_func_2C8(Cam* cam) {

}

// offset: 0x2D4 | func: 3 | export: 3
void dll_100_func_2D4(void* arg0, s32 arg1) {

}

// offset: 0x2E4 | func: 4
static void dll_100_func_2E4(Cam* cam, Object* arg1, f32* arg2) {
    f32 spD4;
    f32 temp_ft2;
    f32 temp_fv0;
    f32 temp_fv1;
    Vec3f spBC;
    Vec3f spB0;
    AABBs32 sp98;
    Unk80027934 sp2C;

    spD4 = fsin16_precise(arg1->srt.yaw);
    temp_ft2 = fcos16_precise(arg1->srt.yaw) * 60.0f;
    spB0.x = arg1->globalPosition.x + (spD4 * 60.0f);
    spB0.y = arg1->globalPosition.y + 37.0f;
    spB0.z = arg1->globalPosition.z + temp_ft2;
    spBC.x = arg1->globalPosition.x;
    spBC.y = spB0.y;
    spBC.z = arg1->globalPosition.z;
    sp2C.unk50[0] = -1;
    sp2C.unk54[0] = 4;
    sp2C.unk40[0] = 4.5f;
    fit_aabb_around_cubes(&sp98, &spBC, &spB0, &sp2C.unk40[0], 1);
    func_80053750(arg1, &sp98, 1);
    if (func_8005509C(arg1, spBC.f, spB0.f, 1, &sp2C, 0) != 0) {
        temp_fv0 = spB0.x - spBC.x;
        temp_fv1 = spB0.z - spBC.z;
        *arg2 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1));
        return;
    }
    *arg2 = 60.0f;
}
