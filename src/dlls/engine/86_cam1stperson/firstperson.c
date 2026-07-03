#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/84_camnormal.h"
#include "sys/curves.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/segment_53F00.h"
#include "dll.h"

// official filename: firstperson.c

// size: 0x124
typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10[4];
    f32 unk20[4];
    f32 unk30[4];
    f32 unk40[4];
    f32 unk50[4];
    u8 _unk60[0x70 - 0x60];
    f32 unk70;
    f32 unk74;
    CurvesStruct unk78;
    f32 unk114;
    f32 unk118;
    f32 unk11C;
    u8 unk120;
} Cam1stPerson;

typedef struct {
    f32 unk0;
    f32 unk4;
    u16 unk8;
} Cam1stPerson_Params;

/*0x0*/ static Cam1stPerson* sState;

static s32 cam1stperson_func_A74(Cam* cam, Object* arg1);
static void cam1stperson_func_D90(Cam* cam);
static void cam1stperson_func_1008(Cam* cam);
static void cam1stperson_func_1524(Cam* cam, Object* arg1);

// offset: 0x0 | ctor
void cam1stperson_ctor(void* dll) { }

// offset: 0xC | dtor
void cam1stperson_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void cam1stperson_func_18(Cam* cam, s32 arg1, Cam1stPerson_Params* data) {
    f32 sp54;
    f32 sp50;
    f32 temp_fv0;
    f32 sp48;
    f32 temp_fv0_2;
    f32 sp40;
    Object* player;
    s16 temp_v1;
    s16 sp38;
    s16 var_v1;
    f32 temp_fv1;
    f32 temp_fv1_2;

    player = cam->player;
    sState = mmAlloc(sizeof(Cam1stPerson), ALLOC_TAG_CAM_COL, ALLOC_NAME("cam1stperson"));
    sState->unk0 = data->unk0;
    sState->unk114 = (f32) data->unk8;
    sState->unk4 = data->unk4;
    sState->unk11C = 0;
    temp_v1 = (-cam->srt.yaw - player->srt.yaw) + 0x8000;
    if (temp_v1 < 0) {
        sp38 = -temp_v1;
    } else {
        sp38 = temp_v1;
    }
    sp40 = (f32) temp_v1 / 50.0f;
    sState->unk78.unk84 = sState->unk10;
    sState->unk78.unk88 = sState->unk20;
    sState->unk78.unk8C = sState->unk30;
    sState->unk78.numControlPoints = 4;
    sState->unk78.unk80 = 0;
    sState->unk78.splineFunc = curves_hermite;
    sState->unk78.splineConverterFunc = curves_hermite_converter;
    sp54 = cam->srt.transl.x - player->srt.transl.x;
    sp50 = cam->srt.transl.z - player->srt.transl.z;
    temp_fv0 = sqrtf(SQ(sp54) + SQ(sp50));
    if (temp_fv0 != 0.0f) {
        sp54 /= temp_fv0;
        sp50 /= temp_fv0;
    }
    sp48 = -fsin16_precise(player->srt.yaw);
    temp_fv0_2 = fcos16_precise(player->srt.yaw);
    temp_fv1 = (f32) sp38 / 90.0f;
    sState->unk10[0] = cam->srt.transl.x;
    sState->unk10[1] = player->srt.transl.x;
    sState->unk10[2] = -sp50 * sp40;
    sState->unk10[3] = sp48 * temp_fv1;
    sState->unk20[0] = cam->srt.transl.y;
    sState->unk20[1] = player->srt.transl.y + 27.0f;
    sState->unk20[2] = 0.0f;
    sState->unk20[3] = 0.0f;
    sState->unk30[0] = cam->srt.transl.z;
    sState->unk30[1] = player->srt.transl.z;
    sState->unk30[2] = sp54 * sp40;
    sState->unk30[3] = -temp_fv0_2 * temp_fv1;
    sState->unk10[2] = 0.0f;
    sState->unk10[3] = 0.0f;
    sState->unk20[2] = 0.0f;
    sState->unk20[3] = 0.0f;
    sState->unk30[2] = 0.0f;
    sState->unk30[3] = 0.0f;
    sState->unk10[1] = sState->unk10[1];
    sState->unk30[1] = sState->unk30[1];
    curves_move(&sState->unk78);
    var_v1 = 0x8000 - arctan2_f(cam->srt.transl.x - sState->unk10[1], cam->srt.transl.z - sState->unk30[1]);\
    var_v1 = cam->srt.yaw - (u16)var_v1;\
    CIRCLE_WRAP(var_v1);
    sState->unk40[0] = (f32) var_v1;
    sState->unk40[1] = 0.0f;
    sState->unk40[2] = 0.0f;
    sState->unk40[3] = 0.0f;
    temp_fv1_2 = sState->unk40[0] - sState->unk40[1];
    if ((temp_fv1_2 > 32768.0f) || (temp_fv1_2 < -32768.0f)) {
        if (sState->unk40[0] < 0.0f) {
            sState->unk40[0] += 65535.0f;
        } else if (sState->unk40[1] < 0.0f) {
            sState->unk40[1] += 65535.0f;
        }
    }
    sState->unk50[0] = (f32) cam->srt.pitch;
    sState->unk50[1] = 0.0f;
    sState->unk50[2] = 0.0f;
    sState->unk50[3] = 0.0f;
    if (arg1 == 1) {
        sState->unk120 = 5;
    } else {
        sState->unk120 = 0;
        gDLL_6_AMSFX->vtbl->play(player, SOUND_1F, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    cam->unk118 = 1;
}

// offset: 0x4F8 | func: 1 | export: 1
void cam1stperson_func_4F8(Cam* cam) {
    u16 sp56;
    s32 temp_v0;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fv0;
    CamControl_Module* sp3C;
    Object* sp38;

    sp38 = cam->player;
    cam->highlightFlags |= 2;
    sp56 = joy_get_pressed(0);
    switch (sState->unk120) {
    case 0:
        sState->unk120 = cam1stperson_func_A74(cam, cam->player);
        cam->unk118 = 1;
        break;
    case 1:
        if (curves_func_800053B0(&sState->unk78, 1000.0f) != 0) {
            sState->unk8 = cam->srt.transl.x - cam->player->srt.transl.x;
            sState->unkC = cam->srt.transl.z - cam->player->srt.transl.z;
            sState->unk120 = 2;
        }
        cam->srt.yaw = (s16) sState->unk78.unk68.x;
        cam->unk118 = 1;
        break;
    case 2:
        cam1stperson_func_D90(cam);
        if (sp56 != 0) {
            cam1stperson_func_1008(cam);
            sState->unk120 = 3;
        }
        cam->unk118 = 0;
        break;
    case 5:
        cam->srt.yaw = 0x8000 - sp38->srt.yaw;
        cam->srt.pitch = (s16) ((f32) sp38->srt.pitch + (sState->unk114 * 182.0f));
        cam->srt.roll = sp38->srt.roll;
        cam->srt.transl.x = sp38->globalPosition.x;
        cam->srt.transl.y = sp38->globalPosition.y + sState->unk4;
        cam->srt.transl.z = sp38->globalPosition.z;
        cam->fov = 80.0f;
        break;
    case 3:
        temp_v0 = curves_func_800053B0(&sState->unk78, 1000.0f);
        cam->srt.yaw = (s16) sState->unk78.unk68.x;
        cam->srt.pitch = (s16) sState->unk78.unk68.y;
        if (temp_v0 != 0) {
            cam1stperson_func_1524(cam, cam->player);
            sState->unk120 = 4;
        }
        cam->unk118 = 1;
        break;
    case 4:
        var_fv0 = sState->unk78.unk0 - 0.2f;
        if (var_fv0 < 0.0f) {
            var_fv0 = 0.0f;
        }
        var_fv0 *= 1.25f;
        cam->player->opacity = (u8) (s32) (var_fv0 * 255.0f);
        if (curves_func_800053B0(&sState->unk78, 5.0f) != 0) {
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
            cam->player->opacity = 0xFF;
        }
        cam->srt.transl.x = sState->unk78.unk68.x;
        cam->srt.transl.z = sState->unk78.unk68.z;
        sp3C = gDLL_2_Camera->vtbl->get_camnormal_module();
        ((DLL_84_camnormal*)sp3C->dll)->vtbl->func8(cam, 2, 3, &sState->unk70, &sState->unk74);
        ((DLL_84_camnormal*)sp3C->dll)->vtbl->func6(cam, cam->player, sState->unk70, sState->unk74);
        temp_fv0 = cam->srt.transl.x - cam->player->srt.transl.x;
        temp_fv1 = cam->srt.transl.z - cam->player->srt.transl.z;
        temp_fa0 = SQ(temp_fv0) + SQ(temp_fv1);
        if (temp_fa0 < 100.0f) {
            cam->srt.pitch = 0;
        } else {
            ((DLL_84_camnormal*)sp3C->dll)->vtbl->func5(cam, cam->player->srt.transl.y, sqrtf(temp_fa0));
        }
        cam->unk118 = 1;
        break;
    }
    if (func_80025F40(cam->player, NULL, NULL, NULL) != 0) {
        cam1stperson_func_1008(cam);
        cam->srt.transl.x = sState->unk10[1];
        cam->srt.transl.y = sState->unk20[1];
        cam->srt.transl.z = sState->unk30[1];
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_None);
    }
}

// offset: 0xA04 | func: 2 | export: 2
void cam1stperson_func_A04(Cam* cam) {
    cam->player->srt.flags &= ~OBJFLAG_SKIP_MODEL_DL;
    cam->player->opacity = 0xFF;
    mmFree(sState);
}

// offset: 0xA64 | func: 3 | export: 3
void cam1stperson_func_A64(void* arg0, s32 arg1) {

}

// offset: 0xA74 | func: 4
s32 cam1stperson_func_A74(Cam* cam, Object* arg1) {
    s32 sp2C;
    s16 sp2A;
    f32 temp_fv0;
    f32 var_fv1;

    sp2C = curves_func_800053B0(&sState->unk78, 5.0f);
    cam->srt.transl.x = sState->unk78.unk68.x;
    cam->srt.transl.y = sState->unk78.unk68.y;
    cam->srt.transl.z = sState->unk78.unk68.z;
    cam->srt.pitch = (s16) curves_hermite(sState->unk50, sState->unk78.unk0, NULL);
    sp2A = (s16) curves_hermite(sState->unk40, sState->unk78.unk0, NULL);
    var_fv1 = sState->unk78.unk0 - 0.2f;
    if (var_fv1 < 0.0f) {
        var_fv1 = 0.0f;
    }
    var_fv1 *= 1.25f;
    cam->player->opacity = 255 - (s32) (var_fv1 * 255.0f);
    if (sp2C != 0) {
        sState->unk78.unk84 = sState->unk40;
        sState->unk78.unk88 = NULL;
        sState->unk78.unk8C = NULL;
        sState->unk78.numControlPoints = 4;
        sState->unk78.splineFunc = curves_hermite;
        sState->unk78.splineConverterFunc = curves_hermite_converter;
        sState->unk78.unk80 = 0;
        sState->unk40[0] = (f32) cam->srt.yaw;
        sState->unk40[1] = (f32) (s16) (0x8000 - arg1->srt.yaw);
        temp_fv0 = sState->unk40[0] - sState->unk40[1];
        if ((temp_fv0 < 1820.0f) && (temp_fv0 > -1820.0f)) {
            sState->unk40[1] = sState->unk40[0];
        } else {
            if (((sState->unk40[0] - sState->unk40[1]) > 32768.0f) || ((sState->unk40[0] - sState->unk40[1]) < -32768.0f)) {
                if (sState->unk40[0] < 0.0f) {
                    sState->unk40[0] += 65535.0f;
                } else if (sState->unk40[1] < 0.0f) {
                    sState->unk40[1] += 65535.0f;
                }
            }
        }
        sState->unk40[2] = 0.0f;
        sState->unk40[3] = 0.0f;
        curves_move(&sState->unk78);
        cam->player->srt.flags |= OBJFLAG_SKIP_MODEL_DL;
        return 1;
    }
    cam->srt.yaw = (sp2A - arctan2_f(cam->srt.transl.x - sState->unk10[1], cam->srt.transl.z - sState->unk30[1])) + 0x8000;
    return 0;
}

// offset: 0xD90 | func: 5
void cam1stperson_func_D90(Cam* cam) {
    s8 sp27;
    s8 var_a0;
    s16 var_v0;
    Object* sp20;

    sp20 = cam->player;
    sp27 = joy_get_stick_x(0);
    var_a0 = joy_get_stick_y(0);
    if ((sp27 < 8) && (sp27 > -8)) {
        sp27 = 0;
    }
    if ((var_a0 < 8) && (var_a0 > -8)) {
        var_a0 = 0;
    }
    sState->unk11C += (((f32) (sp27 * 6) - sState->unk11C) * 0.12f * gUpdateRateF);
    if ((sState->unk11C > -5.0f) && (sState->unk11C < 5.0f)) {
        sState->unk11C = 0.0f;
    }
    cam->srt.yaw += (sState->unk11C * gUpdateRateF);
    var_v0 = ((s16)(((f32) var_a0 / 120.0f) * 15360.0f) - (u16)(cam->srt.pitch));
    CIRCLE_WRAP(var_v0);
    cam->srt.pitch += ((var_v0 * gUpdateRate) >> 4);
    if (cam->srt.pitch > 0x3C00) {
        cam->srt.pitch = 0x3C00;
    }
    if (cam->srt.pitch < -0x3C00) {
        cam->srt.pitch = -0x3C00;
    }
    sp20->srt.yaw = 0x8000 - cam->srt.yaw;
    cam->srt.transl.x = sp20->srt.transl.x + sState->unk8;
    cam->srt.transl.y = sp20->srt.transl.y + 27.0f;
    cam->srt.transl.z = sp20->srt.transl.z + sState->unkC;
}

// offset: 0x1008 | func: 6
void cam1stperson_func_1008(Cam* cam) {
    f32 temp_fv0;
    f32 spF8;
    f32 temp_ft4;
    f32 spF0;
    f32 spEC;
    f32 spE8;
    Object* player;
    Vec3f spD8;
    Vec3f spCC;
    AABBs32 spB4;
    Unk80027934 sp48;

    player = cam->player;
    sState->unk10[0] = cam->srt.transl.x;
    sState->unk10[2] = 0.0f;
    sState->unk10[3] = 0.0f;
    sState->unk20[0] = cam->srt.transl.y;
    sState->unk20[2] = 0.0f;
    sState->unk20[3] = 0.0f;
    sState->unk30[0] = cam->srt.transl.z;
    sState->unk30[2] = 0.0f;
    sState->unk30[3] = 0.0f;
    spEC = fsin16_precise(player->srt.yaw);
    spE8 = fcos16_precise(player->srt.yaw);
    temp_fv0 = sqrtf(SQ(sState->unk0) - SQ(sState->unk4));
    sState->unk10[1] = player->globalPosition.x + (spEC * temp_fv0);
    sState->unk20[1] = player->globalPosition.y + sState->unk114 + sState->unk4;
    sState->unk30[1] = player->globalPosition.z + (spE8 * temp_fv0);
    spF8 = sState->unk10[1] - sState->unk10[0];
    temp_ft4 = sState->unk20[1] - sState->unk20[0];
    spF0 = sState->unk30[1] - sState->unk30[0];
    temp_fv0 = sqrtf(SQ(spF8) + SQ(temp_ft4) + SQ(spF0));
    if (temp_fv0 != 0.0f) {
        spF8 *= (1.0f / temp_fv0);
        spF0 *= (1.0f / temp_fv0);
    }
    spCC.x = sState->unk10[1];
    spCC.y = sState->unk20[1];
    spCC.z = sState->unk30[1];
    spD8.x = sState->unk10[0] - (spF8 * 20.0f);
    spD8.y = sState->unk20[1];
    spD8.z = sState->unk30[0] - (spF0 * 20.0f);
    sp48.unk40[0] = 4.5f;
    sp48.unk50[0] = -1;
    sp48.unk54[0] = 4;
    fit_aabb_around_cubes(&spB4, &spD8, &spCC, &sp48.unk40[0], 1);
    func_80053750(player, &spB4, 1);
    func_8005509C(player, spD8.f, spCC.f, 1, &sp48, 0);
    sState->unk10[1] = spCC.x;
    sState->unk20[1] = spCC.y;
    sState->unk30[1] = spCC.z;
    spF8 = sState->unk10[1] - sState->unk10[0];
    spF0 = sState->unk30[1] - sState->unk30[0];
    sState->unk118 = sqrtf(SQ(spF8) + SQ(spF0));
    sState->unk78.unk84 = sState->unk40;
    sState->unk78.unk88 = sState->unk50;
    sState->unk78.unk8C = NULL;
    sState->unk78.numControlPoints = 4;
    sState->unk78.unk80 = 0;
    sState->unk78.splineFunc = curves_hermite;
    sState->unk78.splineConverterFunc = curves_hermite_converter;
    sState->unk40[0] = (f32) cam->srt.yaw;
    sState->unk40[1] = (f32) (s16) (0x8000 - arctan2_f(
        sState->unk10[1] - player->globalPosition.x, 
        sState->unk30[1] - player->globalPosition.z));
    sState->unk40[2] = 0.0f;
    sState->unk40[3] = 0.0f;
    if (((sState->unk40[0] - sState->unk40[1]) > 32768.0f) || ((sState->unk40[0] - sState->unk40[1]) < -32768.0f)) {
        if (sState->unk40[0] < 0.0f) {
            sState->unk40[0] += 65535.0f;
        } else if (sState->unk40[1] < 0.0f) {
            sState->unk40[1] += 65535.0f;
        }
    }
    sState->unk50[0] = (f32) cam->srt.pitch;
    sState->unk50[1] = 0.0f;
    sState->unk50[2] = 0.0f;
    sState->unk50[3] = 0.0f;
    if (((sState->unk50[0] - sState->unk50[1]) > 32768.0f) || ((sState->unk50[0] - sState->unk50[1]) < -32768.0f)) {
        if (sState->unk50[0] < 0.0f) {
            sState->unk50[0] += 65535.0f;
        } else if (sState->unk50[1] < 0.0f) {
            sState->unk50[1] += 65535.0f;
        }
    }
    curves_move(&sState->unk78);
}

// offset: 0x1524 | func: 7
void cam1stperson_func_1524(Cam* cam, Object* arg1) {
    sState->unk78.unk84 = sState->unk10;
    sState->unk78.unk88 = sState->unk20;
    sState->unk78.unk8C = sState->unk30;
    sState->unk78.numControlPoints = 4;
    sState->unk78.unk80 = 0;
    sState->unk78.splineFunc = curves_hermite;
    sState->unk78.splineConverterFunc = curves_hermite_converter;
    curves_move(&sState->unk78);
    cam->player->srt.flags &= ~OBJFLAG_SKIP_MODEL_DL;
}
