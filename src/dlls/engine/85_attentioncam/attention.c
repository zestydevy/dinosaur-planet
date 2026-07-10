#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/84_camnormal.h"
#include "dlls/engine/85_attentioncam.h"
#include "dlls/engine/86_cam1stperson.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "sys/joypad.h"
#include "sys/memory.h"
#include "sys/voxmap.h"
#include "dll.h"
#include "macros.h"

// official filename: attention.c
// This camera is active when the player presses Z to re-align the camera with the player

// size: 0x1BC
typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18[20];
    f32 unk68[20];
    f32 unkB8[20];
    Vec4f unk108;
    f32 unk118;
    CurvesStruct unk11C;
    u8 unk1B8;
} CamAttention;

/*0x0*/ static CamAttention* sState;

static void attentioncam_func_B14(Cam* cam, Object* arg1);
static void attentioncam_func_D70(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s16 arg6, s16 arg7, s32* arg8);
static void attentioncam_func_F84(s16* arg0, u16* arg1, s16 arg2, s16 arg3, s16 arg4);
static void attentioncam_func_109C(Cam* cam, f32 arg1, f32 arg2);
static s32 attentioncam_func_112C(f32* arg0, f32* arg1, f32* arg2, Object* arg3);
static void attentioncam_func_1374(Cam* cam, Object* arg1);

// offset: 0x0 | ctor
void attentioncam_ctor(void* dll) { }

// offset: 0xC | dtor
void attentioncam_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void attentioncam_func_18(Cam* cam, s32 arg1, AttentionCam_Params* data) {
    s16 var_a0;
    s16 sp14C;
    f32 sp148;
    f32 sp144;
    f32 sp140;
    f32 sp13C;
    Object* player;
    f32 temp_fv0;
    f32 sp130;
    f32 sp12C;
    Vec3f sp120;
    Vec3f sp114;
    Vec3f sp108;
    Vec3s16 sp100;
    Vec3s16 spF8;
    Vec3s16 spF0;
    s32 spEC;
    f32 var_fa0;
    f32 var_fa1;
    s32 temp_s0;
    s32 var_v1;
    Unk80027934 sp70;
    AABBs32 sp58;
    CamControl_Module* camnormal;
    s32 _pad;

    if (data == NULL) {
        STUBBED_PRINTF(" ERROR: Attention cam given NULL data \n");
    }

    data->unk3 = 1;
    player = cam->player;
    sState = mmAlloc(sizeof(CamAttention), ALLOC_TAG_CAM_COL, ALLOC_NAME("attentioncam"));
    bzero(sState, sizeof(CamAttention));
    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func7(
        &sState->unk0, &sState->unk4, &sState->unk8, 0, &sState->unkC);
    sState->unk1B8 = 0;
    sp130 = fsin16_precise(player->srt.yaw);
    sp12C = fcos16_precise(player->srt.yaw);
    var_fa0 = cam->srt.transl.x - player->srt.transl.x;
    var_fa1 = cam->srt.transl.z - player->srt.transl.z;
    var_a0 = player->srt.yaw - (u16)arctan2_f(var_fa0, var_fa1);
    CIRCLE_WRAP(var_a0);
    if (var_a0 < 0) {
        var_a0 = -var_a0;
    }
    if (var_a0 < (s16) (data->unk0 * 182.04f)) {
        sState->unk1B8 = 1;
        return;
    }
    var_fa0 = SQ(sState->unk0) - SQ(sState->unk8);
    if (var_fa0 < 5.0f) {
        var_fa0 = 5.0f;
    }
    temp_fv0 = sqrtf(var_fa0);
    sp120.x = player->srt.transl.x + (sp130 * temp_fv0);
    sp120.y = player->srt.transl.y + sState->unkC + sState->unk8;
    sp120.z = player->srt.transl.z + (sp12C * temp_fv0);
    if (data->unk3 != 0) {
        player->srt.transl.y += sState->unkC;
        func_80007EE0(&player->srt.transl, &sp100);
        func_80007EE0(&sp120, &spF8);
        func_80007E2C(&sp108, &spF0); // @bug: spF0 is uninitialized, result is unused
        sp148 = sp120.x - player->srt.transl.x;
        sp144 = sp120.y - player->srt.transl.y;
        sp140 = sp120.z - player->srt.transl.z;
        sp13C = sqrtf(SQ(sp148) + SQ(sp144) + SQ(sp140));
        if (sp13C != 0.0f) {
            sp13C = 1.0f / sp13C;
            sp148 *= sp13C;
            sp144 *= sp13C;
            sp140 *= sp13C;
        }
        sp114.x = player->srt.transl.x - (sp148 * 20.0f);
        sp114.y = player->srt.transl.y - (sp144 * 20.0f);
        sp114.z = player->srt.transl.z - (sp140 * 20.0f);
        sp114.y = sp120.y;
        sp70.unk40[0] = 4.5f;
        sp70.unk50[0] = -1;
        sp70.unk54[0] = 3;
        fit_aabb_around_cubes(&sp58, &player->srt.transl, &sp120, &sp70.unk40[0], 1);
        func_80053750(player, &sp58, 1);
        func_8005509C(player, sp114.f, sp120.f, 1, &sp70, 0);
        player->srt.transl.y -= sState->unkC;
    }
    spEC = 0;
    while (spEC < 3) {
        sState->unk18[spEC] = cam->srt.transl.x;
        sState->unk68[spEC] = cam->srt.transl.y;
        sState->unkB8[spEC] = cam->srt.transl.z;
        spEC++;
    }
    sp148 = cam->srt.transl.x - sp120.x;
    sp140 = cam->srt.transl.z - sp120.z;
    sp13C = 0.5f * sqrtf(SQ(sp148) + SQ(sp140));
    temp_s0 = arctan2_f(-sp130, -sp12C);
    var_a0 = temp_s0 - (u16)arctan2_f(sp148, sp140);\
    CIRCLE_WRAP(var_a0);
    sp14C = var_a0;
    if (var_a0 < 0) {
        var_a0 = -var_a0;
    }
    if (var_a0 > 0x4000) {
        var_a0 = 0;
    } else {
        var_a0 = 0x4000 - var_a0;
    }
    if (sp14C < 0) {
        sp14C = -(var_a0 * 2);
    } else {
        sp14C = var_a0 * 2;
    }
    if (var_a0 != 0) {
        sp13C = sp13C / fsin16_precise(var_a0);
    } else {
        sp13C = 0.0f;
    }
    var_fa0 = sp120.x - (sp130 * sp13C);
    var_fa1 = sp120.z - (sp12C * sp13C);
    sState->unk11C.unk84 = sState->unk18;
    sState->unk11C.unk88 = sState->unk68;
    sState->unk11C.unk8C = sState->unkB8;
    sState->unk11C.splineFunc = curvesBSpline;
    sState->unk11C.splineConverterFunc = curvesBSplineConverter;
    attentioncam_func_D70(
        var_fa0, 
        var_fa1, 
        cam->srt.transl.x, 
        cam->srt.transl.y, 
        cam->srt.transl.z, 
        sp120.y, 
        sp14C, 
        0x1555, 
        &spEC);
    var_v1 = spEC;
    while (var_v1 < (spEC + 3)) {
        sState->unk18[var_v1] = sp120.x;
        sState->unk68[var_v1] = sp120.y;
        sState->unkB8[var_v1] = sp120.z;
        var_v1 += 1;
    }
    sState->unk11C.numControlPoints = var_v1;
    sState->unk11C.unk80 = 0;
    curvesMove(&sState->unk11C);
    if (sp14C < 0) {
        var_a0 = sp14C * -1;
    } else {
        var_a0 = sp14C;
    }
    if ((var_a0 >= 0x2000) && (data->unk2 != 0)) {
        gDLL_6_AMSFX->vtbl->play(player, SOUND_1E, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    gDLL_2_Camera->vtbl->func12(sState->unk11C.unkC, &sState->unk108, 20.0f, 0.5f, 1.0f, -10.0f);
    sState->unk10 = -100000.0f;
    sState->unk14 = 100000.0f;
}

// offset: 0x848 | func: 1 | export: 1
void attentioncam_func_848(Cam* cam) {
    u8 sp5F;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    Vec3f sp40;
    Object* sp3C;
    CamControl_Module* camnormal;

    if (sState->unk1B8 != 0) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
        return;
    }
    sp3C = cam->player;
    sp40.y = cam->srt.transl.y;
    sp5F = attentioncam_func_112C(&sp40.x, &sp40.y, &sp40.z, sp3C);
    cam->srt.transl.x = sp40.x;
    cam->srt.transl.z = sp40.z;
    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func8(cam, 3, 3, &sState->unk10, &sState->unk14);
    if (cam->unk34.unk68 != 0) {
        sState->unk118 = (f32) (sState->unk118 + gUpdateRateF);
    }
    if (sState->unk118 > 10.0f) {
        attentioncam_func_1374(cam, sp3C);
        sp5F = 1;
    }
    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp58, &sp54, &sp50, &sp4C, 0.0f);
    attentioncam_func_109C(cam, sp58, sp50);
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func5(cam, sp3C->srt.transl.y, sp4C);
    if (sp5F != 0) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
    }
    attentioncam_func_B14(cam, sp3C);
}

// offset: 0xAC4 | func: 2 | export: 2
void attentioncam_func_AC4(Cam* cam) {
    mmFree(sState);
}

// offset: 0xB04 | func: 3 | export: 3
void attentioncam_func_B04(void* params, s32 arg1) {

}

// offset: 0xB14 | func: 4
static void attentioncam_func_B14(Cam* cam, Object* arg1) {
    u16 btns;
    Cam1stPerson_Params cam1stPerson;
    u8 temp_v1;

    if (arg1->animObj == NULL) {
        btns = joyGetPressed(0);
        if (((cam->highlight != NULL) || (cam->srt.flags & OBJFLAG_UNK_2)) && 
                !(cam->highlightFlags & 2) && 
                (((temp_v1 = mainGetBits(BIT_4AD), (temp_v1 == 0)) && (btns & Z_TRIG)) || ((temp_v1 != 0) && (btns & R_TRIG)) || (cam->targetFlags & 2)) && 
                (arg1->controlNo == OBJCONTROL_Player) && 
                ((((DLL_210_Player*)arg1->dll)->vtbl->func60(arg1) != 0))) {
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMLOCKON, TRUE, 0, sizeof(&cam->highlight), &cam->highlight, 0x3C, Cam_Ease_All);
        } else if ((btns & 8) && !(cam->targetFlags & 1)) {
            cam1stPerson.unk0 = sState->unk4;
            cam1stPerson.unk4 = sState->unk8;
            cam1stPerson.unk8 = (s16) (u32) sState->unkC;
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAM1STPERSON, TRUE, 0, sizeof(cam1stPerson), &cam1stPerson, 0, Cam_Ease_All);
        }
    }
}

// offset: 0xD70 | func: 5
static void attentioncam_func_D70(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s16 arg6, s16 arg7, s32* arg8) {
    s16 spC0[20];
    s16 spB8[3];
    s16 var_s2;
    u16 spB4;
    f32 temp1;
    f32 temp_fs0;
    f32 temp2;
    s32 var_s3;
    s32 var_s4;
    f32 sp94[3];

    if (arg6 < 0) {
        var_s2 = arg6 * -1;
    } else {
        var_s2 = arg6;
    }
    spB4 = 0;
    attentioncam_func_F84(spC0, &spB4, 0, var_s2, arg7);
    temp1 = arg2 - arg0;
    temp_fs0 = arg5 - arg3;
    temp2 = arg4 - arg1;
    var_s3 = 1;
    var_s4 = 3;
    while (var_s3 < spB4) {
        sp94[0] = temp1;
        sp94[1] = temp_fs0;
        sp94[2] = temp2;
        spB8[0] = arg6 < 0 ? spC0[var_s3] : -spC0[var_s3];
        spB8[1] = 0;
        spB8[2] = 0;
        rotate_vec3((SRT*)&spB8, sp94);
        sState->unk18[var_s4] = arg0 + sp94[0];
        sState->unk68[var_s4] = arg3 + (temp_fs0 * ((f32) spC0[var_s3] / (f32) var_s2));
        sState->unkB8[var_s4] = arg1 +  sp94[2];
        var_s3 += 1;
        var_s4 += 1;
    }
    *arg8 = var_s4;
}

// offset: 0xF84 | func: 6
static void attentioncam_func_F84(s16* arg0, u16* arg1, s16 arg2, s16 arg3, s16 arg4) {
    if (arg3 >= arg4) {
        attentioncam_func_F84(arg0, arg1, arg2, arg3 >> 1, arg4);
        attentioncam_func_F84(arg0, arg1, arg2 + (arg3 >> 1), arg3 >> 1, arg4);
        return;
    }
    arg0[(*arg1)++] = arg2;
}

// offset: 0x109C | func: 7
static void attentioncam_func_109C(Cam* cam, f32 arg1, f32 arg2) {
    s32 var_v1 = (-arctan2_f(arg1, arg2) - (cam->srt.yaw & 0xFFFF)) + 0x8000;
    CIRCLE_WRAP(var_v1);
    cam->srt.yaw += var_v1;
}

// offset: 0x112C | func: 8
static s32 attentioncam_func_112C(f32* arg0, f32* arg1, f32* arg2, Object* arg3) {
    s32 var_v1;
    s32 temp_v0;
    f32 t;
    CamControl_Module* camnormalMod;
    Cam camnorm;

    bzero(&camnorm, sizeof(camnorm));
    camnorm.srt.transl.x = sState->unk18[sState->unk11C.numControlPoints - 2];
    camnorm.srt.transl.y = *arg1;
    camnorm.srt.transl.z = sState->unkB8[sState->unk11C.numControlPoints - 2];
    camnorm.player = arg3;
    camnorm.positionMirror.x = camnorm.srt.transl.x;
    camnorm.positionMirror.y = camnorm.srt.transl.y;
    camnorm.positionMirror.z = camnorm.srt.transl.z;
    camnormalMod = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormalMod->dll)->vtbl->func4(&camnorm, arg3);
    ((DLL_84_camnormal*)camnormalMod->dll)->vtbl->func8(&camnorm, 1, 3, &sState->unk10, &sState->unk14);
    var_v1 = sState->unk11C.numControlPoints - 3;
    while (var_v1 < sState->unk11C.numControlPoints) {
        sState->unk18[var_v1] = camnorm.srt.transl.x;
        sState->unkB8[var_v1] = camnorm.srt.transl.z;
        var_v1 += 1;
    }
    if (sState->unk11C.unkC != 0.0f) {
        t = sState->unk11C.unk8 / sState->unk11C.unkC;
    } else {
        t = 0.0f;
    }
    if (t > 1.0f) {
        t = 1.0f;
    } else if (t < 0.0f) {
        t = 0.0f;
    }
    t = curvesHermite(&sState->unk108.x, t, NULL);
    if (t < 0.2f) {
        t = 0.2f;
    }
    temp_v0 = curves_func_800053B0(&sState->unk11C, t);
    *arg0 = sState->unk11C.unk68.x;
    *arg2 = sState->unk11C.unk68.z;
    return temp_v0;
}

// offset: 0x1374 | func: 9
static void attentioncam_func_1374(Cam* cam, Object* arg1) {
    f32 temp_fv0;
    f32 spD0;
    f32 spCC;
    Vec3f spC0;
    Vec3f spB4;
    AABBs32 sp9C;
    Unk80027934 sp30;

    spD0 = fsin16_precise(arg1->srt.yaw);
    spCC = fcos16_precise(arg1->srt.yaw);
    temp_fv0 = sqrtf(SQ(sState->unk4) - SQ(sState->unk8));
    spB4.x = arg1->globalPosition.x + (spD0 * temp_fv0);
    spB4.y = arg1->globalPosition.y + sState->unkC + sState->unk8;
    spB4.z = arg1->globalPosition.z + (spCC * temp_fv0);
    spC0.x = arg1->globalPosition.x;
    spC0.y = spB4.y;
    spC0.z = arg1->globalPosition.z;
    sp30.unk40[0] = 4.5f;
    sp30.unk50[0] = -1;
    sp30.unk54[0] = 3;
    fit_aabb_around_cubes(&sp9C, &spC0, &spB4, &sp30.unk40[0], 1);
    func_80053750(arg1, &sp9C, 0);
    func_8005509C(arg1, spC0.f, spB4.f, 1, &sp30, 0);
    cam->srt.transl.x = spB4.x;
    cam->srt.transl.y = spB4.y;
    cam->srt.transl.z = spB4.z;
}
