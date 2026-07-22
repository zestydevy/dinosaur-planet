#include "dlls/engine/84_camnormal.h"
#include "dlls/objects/210_player.h"
#include "sys/joypad.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "dll.h"

// This appears to be an alternative (older?) firstperson camera

typedef struct {
    s8 unk0;
} Cam1stPerson2_Params;

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
    u8 unk11C;
} Cam1stPerson2;

/*0x0*/ static Cam1stPerson2* sState;
/*0x4*/ static s8 bss_4;

static s32 cam1stperson2_func_9C4(Cam* cam, Object* arg1);
static void cam1stperson2_func_CE0(Cam* cam);
static void cam1stperson2_func_E60(Cam* cam);
static void cam1stperson2_func_137C(Cam* cam, Object* arg1);

// offset: 0x0 | ctor
void cam1stperson2_ctor(void* dll) { }

// offset: 0xC | dtor
void cam1stperson2_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void cam1stperson2_func_18(Cam* cam, s32 arg1, void* data) {
    f32 sp6C;
    f32 sp68;
    f32 temp;
    f32 sp60;
    CamControl_Module* camnormal;
    f32 sp58;
    Object* temp_s1;
    f32 sp50;
    s16 var_v1;
    s16 sp4C;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_s1 = cam->player;
    sState = mmAlloc(sizeof(Cam1stPerson2), ALLOC_TAG_CAM_COL, NULL);
    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func7(&sp50, &sState->unk0, &sState->unk4, 0, &sState->unk114);
    var_v1 = (-cam->srt.yaw - temp_s1->srt.yaw) + 0x8000;
    if (var_v1 < 0) {
        sp4C = -var_v1;
    } else {
        sp4C = var_v1;
    }
    sp58 = var_v1 / 50.0f;
    temp_fv1 = sp4C / 90.0f;
    sState->unk78.unk84 = sState->unk10;
    sState->unk78.unk88 = sState->unk20;
    sState->unk78.unk8C = sState->unk30;
    sState->unk78.numControlPoints = 4;
    sState->unk78.unk80 = 0;
    sState->unk78.splineFunc = curvesHermite;
    sState->unk78.splineConverterFunc = curvesHermiteConverter;
    sp6C = cam->srt.transl.x - temp_s1->srt.transl.x;
    sp68 = cam->srt.transl.z - temp_s1->srt.transl.z;
    temp_fv0 = sqrtf(SQ(sp6C) + SQ(sp68));
    sp6C /= temp_fv0;
    sp68 /= temp_fv0;
    sp60 = -mathSinfInterp(temp_s1->srt.yaw);
    temp = -mathCosfInterp(temp_s1->srt.yaw);
    sState->unk10[0] = cam->srt.transl.x;
    sState->unk10[1] = temp_s1->srt.transl.x;
    sState->unk10[2] = -sp68 * sp58;
    sState->unk10[3] = sp60 * temp_fv1;
    sState->unk20[0] = cam->srt.transl.y;
    sState->unk20[1] = temp_s1->srt.transl.y + 27.0f;
    sState->unk20[2] = 0.0f;
    sState->unk20[3] = 0.0f;
    sState->unk30[0] = cam->srt.transl.z;
    sState->unk30[1] = temp_s1->srt.transl.z;
    sState->unk30[2] = sp6C * sp58;
    sState->unk30[3] = temp * temp_fv1;
    if (1){} // @fake
    sState->unk10[2] = 0.0f;
    sState->unk10[3] = 0.0f;
    sState->unk20[2] = 0.0f;
    sState->unk20[3] = 0.0f;
    sState->unk30[2] = 0.0f;
    sState->unk30[3] = 0.0f;
    sState->unk10[1] = sState->unk10[1] + (sp60 * 5.0f);
    sState->unk30[1] = sState->unk30[1] + (temp * 5.0f);
    if (1){} // @fake
    curvesMove(&sState->unk78);
    var_v1 = 0x8000 - mathAtan2f(cam->srt.transl.x - sState->unk10[1], cam->srt.transl.z - sState->unk30[1]);\
    var_v1 = cam->srt.yaw - (u16)var_v1;\
    CIRCLE_WRAP(var_v1);
    sState->unk40[0] = var_v1;
    sState->unk40[1] = 0.0f;
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
    sState->unk11C = 0;
    cam->unk118 = 1;
    bss_4 = 0;
}

// offset: 0x4BC | func: 1 | export: 1
void cam1stperson2_func_4BC(Cam* cam) {
    Object* temp_v0_4;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fv0;
    s32 var_a0;
    s16 var_v1;
    Object* temp_v0_5;
    CamControl_Module* sp3C;
    s32 temp_v0;

    switch (sState->unk11C) {
    case 0:
        sState->unk11C = cam1stperson2_func_9C4(cam, cam->player);
        cam->unk118 = 1;
        break;
    case 1:
        if (curves_func_800053B0(&sState->unk78, 1000.0f) != 0) {
            sState->unk8 = cam->srt.transl.x - cam->player->srt.transl.x;
            sState->unkC = cam->srt.transl.z - cam->player->srt.transl.z;
            sState->unk11C = 2;
        }
        cam->srt.yaw = (s16) sState->unk78.unk68.x;
        cam->unk118 = 1;
        break;
    case 2:
        cam1stperson2_func_CE0(cam);
        if (bss_4 != 0) {
            cam1stperson2_func_E60(cam);
            sState->unk11C = 3;
        }
        cam->unk118 = 0;
        break;
    case 3:
        temp_v0 = curves_func_800053B0(&sState->unk78, 1000.0f);
        cam->srt.yaw = (s16) sState->unk78.unk68.x;
        cam->srt.pitch = (s16) sState->unk78.unk68.y;
        if (temp_v0 != 0) {
            cam1stperson2_func_137C(cam, cam->player);
            sState->unk11C = 4;
        }
        cam->unk118 = 0;
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
        ((DLL_84_camnormal*)sp3C->dll)->vtbl->func8(cam, 2, 0, &sState->unk70, &sState->unk74);
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
    temp_v0_4 = objGetPlayer();
    temp_v0_5 = ((DLL_210_Player*)temp_v0_4->dll)->vtbl->get_vehicle(temp_v0_4);
    if (temp_v0_5 != NULL) {
        var_v1 = (cam->srt.yaw & 0xFFFF) - ((-temp_v0_5->srt.yaw) & 0xFFFF);
        CIRCLE_WRAP(var_v1);
        if (var_v1 >= 0) {
            var_a0 = var_v1;
        } else {
            var_a0 = -var_v1;
        }
        if (var_a0 < 0x3FFF) {
            if (var_v1 > 0) {
                var_a0 = 0x3FFF;
            } else {
                var_a0 = -0x3FFF;
            }
            // FAKE ^ 0
            cam->srt.yaw = (var_a0^0) + -temp_v0_5->srt.yaw;
        }
    }
}

// offset: 0x93C | func: 2 | export: 2
void cam1stperson2_func_93C(Cam* cam) {
    cam->player->srt.flags &= ~OBJFLAG_INVISIBLE;
    mmFree(sState);
}

// offset: 0x990 | func: 3 | export: 3
void cam1stperson2_func_990(Cam1stPerson2_Params* arg0, s32 arg1) {
    if ((arg1 == 1) && (arg0->unk0 == 1)) {
        bss_4 = 1;
    }
}

// offset: 0x9C4 | func: 4
static s32 cam1stperson2_func_9C4(Cam* cam, Object* arg1) {
    s32 sp2C;
    s16 temp_a0;
    f32 temp_fv0;
    f32 var_fv1;

    sp2C = curves_func_800053B0(&sState->unk78, 5.0f);
    cam->srt.transl.x = sState->unk78.unk68.x;
    cam->srt.transl.y = sState->unk78.unk68.y;
    cam->srt.transl.z = sState->unk78.unk68.z;
    cam->srt.pitch = (s16) curvesHermite(sState->unk50, sState->unk78.unk0, NULL);
    temp_a0 = (s16) curvesHermite(sState->unk40, sState->unk78.unk0, NULL);
    var_fv1 = sState->unk78.unk0 - 0.2f;
    if (var_fv1 < 0.0f) {
        var_fv1 = 0.0f;
    }
    var_fv1 *= 1.25f;
    cam->player->opacity = 0xFF - (s32) (var_fv1 * 255.0f);
    if (sp2C != 0) {
        sState->unk78.unk84 = sState->unk40;
        sState->unk78.unk88 = NULL;
        sState->unk78.unk8C = NULL;
        sState->unk78.numControlPoints = 4;
        sState->unk78.splineFunc = curvesHermite;
        sState->unk78.splineConverterFunc = curvesHermiteConverter;
        sState->unk78.unk80 = 0;
        sState->unk40[0] = (f32) cam->srt.yaw;
        sState->unk40[1] = (f32) (s16) (0x8000 - arg1->srt.yaw);
        temp_fv0 = (sState->unk40[0] - sState->unk40[1]);
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
        curvesMove(&sState->unk78);
        cam->player->srt.flags |= OBJFLAG_INVISIBLE;
        return 1;
    }
    cam->srt.yaw = (temp_a0 - mathAtan2f(cam->srt.transl.x - sState->unk10[1], cam->srt.transl.z - sState->unk30[1])) + 0x8000;
    return 0;
}

// offset: 0xCE0 | func: 5
static void cam1stperson2_func_CE0(Cam* cam) {
    s8 sp27;
    s8 var_a0;
    Object* temp_v0_2;

    sp27 = joyGetStickX(0);
    var_a0 = joyGetStickY(0);
    if ((sp27 < 8) && (sp27 > -8)) {
        sp27 = 0;
    }
    if ((var_a0 < 8) && (var_a0 > -8)) {
        var_a0 = 0;
    }
    cam->srt.yaw += gUpdateRate * sp27 * 5;
    cam->srt.pitch += gUpdateRate * var_a0 * 5;
    if (cam->srt.pitch > 0x2000) {
        cam->srt.pitch = 0x2000;
    }
    if (cam->srt.pitch < -0x2800) {
        cam->srt.pitch = -0x2800;
    }
    cam->player->srt.yaw = 0x8000 - cam->srt.yaw;
    temp_v0_2 = cam->player;
    cam->srt.transl.x = temp_v0_2->srt.transl.x + sState->unk8;
    cam->srt.transl.y = temp_v0_2->srt.transl.y + 27.0f;
    cam->srt.transl.z = temp_v0_2->srt.transl.z + sState->unkC;
}

// offset: 0xE60 | func: 6
static void cam1stperson2_func_E60(Cam* cam) {
    f32 temp_ft4;
    f32 spF8;
    f32 temp_fv0;
    f32 spF0;
    f32 spEC;
    f32 spE8;
    Object* player;
    Vec3f spD8;
    Vec3f spCC;
    TrackIntersectResult sp60;
    AABBs32 sp48;

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
    spEC = mathSinfInterp(player->srt.yaw);
    spE8 = mathCosfInterp(player->srt.yaw);
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
    sp60.unk50[0] = -1;
    sp60.unk54[0] = 4;
    sp60.unk40[0] = 4.5f;
    trackIntersectBuildAABB(&sp48, (Vec3f* ) sState->unk10, &spCC, &sp60.unk40[0], 1);
    trackIntersectBroadphase(player, &sp48, 0);
    trackGetIntersect(player, spD8.f, spCC.f, 1, &sp60, 0);
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
    sState->unk78.splineFunc = curvesHermite;
    sState->unk78.splineConverterFunc = curvesHermiteConverter;
    sState->unk40[0] = (f32) cam->srt.yaw;
    sState->unk40[1] = (f32) (s16) (0x8000 - mathAtan2f(
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
    curvesMove(&sState->unk78);
}

// offset: 0x137C | func: 7
static void cam1stperson2_func_137C(Cam* cam, Object* arg1) {
    sState->unk78.unk84 = sState->unk10;
    sState->unk78.unk88 = sState->unk20;
    sState->unk78.unk8C = sState->unk30;
    sState->unk78.numControlPoints = 4;
    sState->unk78.unk80 = 0;
    sState->unk78.splineFunc = curvesHermite;
    sState->unk78.splineConverterFunc = curvesHermiteConverter;
    curvesMove(&sState->unk78);
    cam->player->srt.flags &= ~OBJFLAG_INVISIBLE;
}

/*0x0*/ static const char str_0[] = " %x %i \n";
