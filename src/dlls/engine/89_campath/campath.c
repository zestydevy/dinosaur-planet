#include "PR/ultratypes.h"
#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/89_campath.h"
#include "sys/curves.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "dll.h"
#include "macros.h"

// Camera that follows a set curve as the player moves along it

// size: 0x68
typedef struct {
    SRT* unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    Vec4f unk48;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    u8 unk64;
    u8 unk65;
} CamPath;

/*0x0*/ static CamPath* sState;

static void campath_func_E40(Cam* cam, Vec3f* arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5);
static s32 campath_func_1004(Cam* cam, u8 arg1);
static f32 campath_func_14D8(f32, f32, f32, s32*);
static void campath_func_17A0(CurveSetup* arg0, s32* arg1, s32 arg2);
static void campath_func_19AC(s32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7);
static void campath_func_1F9C(s32* arg0, s32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);

// offset: 0x0 | ctor
void campath_ctor(void* dll) { }

// offset: 0xC | dtor
void campath_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void campath_func_18(Cam* cam, s32 arg1, CamPath_Params* data) {
    Object* temp_s0;
    f32 sp140;
    f32 sp13C;
    f32 sp138;
    f32 sp134;
    f32 sp130;
    f32 sp12C;
    s32 _pad;
    s16 sp126;
    s16 var_s0;
    s16 sp122;
    f32 var_fs0;
    CurveSetup* sp118;
    CurveSetup* sp114;
    Vec3f sp108;
    s32 spF8[4];
    s32 spE8[4];
    s32 spE0[2];
    f32 spD0[4];
    f32 spC0[4];
    f32 spB0[4];
    f32 spA0[4];
    f32 sp90[4];
    f32 sp80[4];
    f32 sp70[4];
    f32 temp_fv0_3;
    s32 _pad2[8];

    temp_s0 = cam->player;
    sState = mmAlloc(sizeof(CamPath), ALLOC_TAG_CAM_COL, ALLOC_NAME("campath"));
    bzero(sState, sizeof(CamPath));
    sState->unk4 = data->unk0;
    sState->unk64 = 1;
    spE0[0] = 9;
    spE0[1] = 0x1B;
    sState->unkC = gDLL_26_Curves->vtbl->func_1E4(temp_s0->srt.transl.x, temp_s0->srt.transl.y, temp_s0->srt.transl.z, spE0, 2, sState->unk4);
    spE0[0] = 8;
    spE0[1] = 0x1A;
    sState->unk8 = gDLL_26_Curves->vtbl->func_1E4(temp_s0->srt.transl.x, temp_s0->srt.transl.y, temp_s0->srt.transl.z, spE0, 2, sState->unk4);
    /* default.dol
    if (sState->unk8 < 0 || sState->unkC < 0) {
        if (sState->unk8 < 0) {
            STUBBED_PRINTF("Warning: No camera move path found to associate path camera with!!\n");
        } else {
            STUBBED_PRINTF("Warning: No camera control path found to associate path camera with!!\n");
        }
        sState->unk65 = 1;
        return;
    }
    */
    campath_func_1F9C(&sState->unkC, &sState->unk8, temp_s0->srt.transl.x, temp_s0->srt.transl.y, temp_s0->srt.transl.z, sState->unk4);
    sp118 = gDLL_26_Curves->vtbl->func_39C(sState->unk8);
    sp114 = gDLL_26_Curves->vtbl->func_39C(sState->unkC);
    campath_func_17A0(sp118, spE8, sState->unk4);
    campath_func_17A0(sp114, spF8, sState->unk4);
    campath_func_19AC(spE8, sp90, sp80, sp70, spD0, spC0, spB0, spA0);
    var_fs0 = campath_func_14D8(temp_s0->srt.transl.x, temp_s0->srt.transl.y, temp_s0->srt.transl.z, spF8);
    if (var_fs0 < 0.0f) {
        var_fs0 = 0.0f;
    } else if (var_fs0 > 1.0f) {
        var_fs0 = 1.0f;
    }
    sp140 = curvesBSpline(sp90, var_fs0, NULL);
    sp13C = curvesBSpline(sp80, var_fs0, NULL);
    sp138 = curvesBSpline(sp70, var_fs0, NULL);
    sp134 = sp140 - temp_s0->srt.transl.x;
    sp130 = sp13C - temp_s0->srt.transl.y;
    sp12C = sp138 - temp_s0->srt.transl.z;
    if (sp118->campath.unk3B & 1) {
        sp126 = 0x8000 - arctan2_f(sp134, sp12C);
    } else {
        sp126 = (s16) curvesCatmullRom(spD0, var_fs0, NULL) + 0x8000;
    }
    if (sp118->campath.unk3B & 4) {
        sp122 = temp_s0->srt.roll;
    } else {
        sp122 = (s16) curvesCatmullRom(spB0, var_fs0, NULL);
    }
    if (sp118->campath.unk3B & 2) {
        var_s0 = arctan2_f(sp130, sqrtf(SQ(sp134) + SQ(sp12C)));
        var_s0 = (s16) ((f32) var_s0 - curvesCatmullRom(spC0, var_fs0, NULL));
    } else {
        var_s0 = (s16) curvesCatmullRom(spC0, var_fs0, NULL);
    }
    temp_fv0_3 = curvesBSpline(spA0, var_fs0, NULL);
    sp108.x = sp140;
    sp108.y = sp13C;
    sp108.z = sp138;
    if ((data->unk4 == 0) && (arg1 != 3)) {
        campath_func_E40(cam, &sp108, sp126, var_s0, sp122, temp_fv0_3);
    } else {
        cam->srt.transl.x = sp140;
        cam->srt.transl.y = sp13C;
        cam->srt.transl.z = sp138;
        cam->srt.yaw = sp126;
        cam->srt.pitch = var_s0;
        cam->srt.roll = sp122;
        cam->fov = temp_fv0_3;
    }
    sState->unk58 = var_fs0;
}

// offset: 0x588 | func: 1 | export: 1
void campath_func_588(Cam* cam) {
    f32 sp104;
    f32 sp100;
    f32 spFC;
    f32 spF8;
    s32 spE8[4];
    s32 spD8[4];
    CurveSetup* spD4;
    f32 temp;
    s32 pad_spCC;
    u8 spCB;
    f32 spB8[4];
    f32 spA8[4];
    f32 sp98[4];
    f32 sp88[4];
    f32 sp78[4];
    f32 sp68[4];
    f32 sp58[4];
    s32 var_v1; // sp54
    s32 btns;
    Object* player;

    if (sState->unk65 != 0) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
        return;
    }
    player = cam->player;
    btns = joyGetPressed(0);
    spD4 = gDLL_26_Curves->vtbl->func_39C(sState->unkC);
    campath_func_17A0(gDLL_26_Curves->vtbl->func_39C(sState->unk8), spD8, sState->unk4);
    campath_func_17A0(spD4, spE8, sState->unk4);
    campath_func_19AC(spD8, spB8, spA8, sp98, sp88, sp78, sp68, sp58);
    sp104 = campath_func_14D8(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, spE8);
    if (sp104 < 0.0f) {
        if (spE8[0] >= 0) {
            sState->unkC = spE8[0];
            campath_func_17A0(gDLL_26_Curves->vtbl->func_39C(sState->unkC), spE8, sState->unk4);
            if (spD8[0] >= 0) {
                sState->unk8 = spD8[0];
                campath_func_17A0(gDLL_26_Curves->vtbl->func_39C(sState->unk8), spD8, sState->unk4);
                campath_func_19AC(spD8, spB8, spA8, sp98, sp88, sp78, sp68, sp58);
                sp104 = campath_func_14D8(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, spE8);
                sState->unk58 = (f32) (sState->unk58 + 1.0f);
            } else {
                sp104 = 0.0f;
            }
        } else {
            sp104 = 0.0f;
        }
    } else if (sp104 > 1.0f) {
        if ((spE8[2] >= 0) && (spE8[3] >= 0)) {
            sState->unkC = spE8[2];
            campath_func_17A0(gDLL_26_Curves->vtbl->func_39C(sState->unkC), spE8, sState->unk4);
            if ((spD8[2] >= 0) && (spD8[3] >= 0)) {
                sState->unk8 = spD8[2];
                campath_func_17A0(gDLL_26_Curves->vtbl->func_39C(sState->unk8), spD8, sState->unk4);
                campath_func_19AC(spD8, spB8, spA8, sp98, sp88, sp78, sp68, sp58);
                sp104 = campath_func_14D8(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, spE8);
                sState->unk58 = (f32) (sState->unk58 - 1.0f);
            } else {
                sp104 = 1.0f;
            }
        } else {
            sp104 = 1.0f;
        }
    }
    sp104 = sState->unk58 + ((sp104 - sState->unk58) * 0.3f);
    sState->unk58 = sp104;
    cam->srt.transl.x = curvesBSpline(spB8, sp104, NULL);
    cam->srt.transl.y = curvesBSpline(spA8, sp104, NULL);
    cam->srt.transl.z = curvesBSpline(sp98, sp104, NULL);
    spCB = gDLL_26_Curves->vtbl->func_39C(sState->unk8)->campath.unk3B;
    if ((spCB & 1) == 0) {
        cam->srt.yaw = (s16) curvesCatmullRom(sp88, sp104, NULL) + 0x8000;
    }
    if ((spCB & 2) == 0) {
        cam->srt.pitch = (s16) curvesCatmullRom(sp78, sp104, NULL);
    }
    if ((spCB & 4) == 0) {
        cam->srt.roll = (s16) curvesCatmullRom(sp68, sp104, NULL);
    }
    cam->fov = curvesBSpline(sp58, sp104, NULL);
    if ((sState->unk64 == 0) && (campath_func_1004(cam, spCB) != 0)) {
        sState->unk64 = 1;
    }
    sp100 = cam->srt.transl.x - player->srt.transl.x;
    spFC = cam->srt.transl.y - player->srt.transl.y;
    spF8 = cam->srt.transl.z - player->srt.transl.z;
    if ((spCB & 1) != 0) {
        cam->srt.yaw = 0x8000 - arctan2_f(sp100, spF8);
    }
    if ((spCB & 2) != 0) {
        temp = sqrtf(SQ(sp100) + SQ(spF8));
        var_v1 = arctan2_f(spFC, temp);
        var_v1 = ((var_v1 - curvesCatmullRom(sp78, sp104, NULL)) - (cam->srt.pitch & 0xFFFF));
        CIRCLE_WRAP(var_v1);
        cam->srt.pitch += (var_v1 * gUpdateRate) >> 3;
    }
    if ((spCB & 4) != 0) {
        var_v1 = cam->srt.roll - (player->srt.roll & 0xFFFF);
        CIRCLE_WRAP(var_v1);
        cam->srt.roll += (var_v1 * gUpdateRate) >> 3;
    }
    if (btns & Z_TRIG) {
        if (cam->highlight != NULL) {
            if (cam->target == NULL) {
                cam->target = cam->highlight;
            } else {
                cam->target = NULL;
            }
        }
    }
    if (sState->unk0 != NULL) {
        sState->unk0->transl.x = cam->srt.transl.x;
        sState->unk0->transl.y = cam->srt.transl.y;
        sState->unk0->transl.z = cam->srt.transl.z;
    }
}

// offset: 0xDF0 | func: 2 | export: 2
void campath_func_DF0(Cam* cam) {
    mmFree(sState);
}

// offset: 0xE30 | func: 3 | export: 3
void campath_func_E30(void* arg0, s32 arg1) {

}

// offset: 0xE40 | func: 4
static void campath_func_E40(Cam* cam, Vec3f* arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;

    sState->unk64 = 0;
    sState->unk10 = cam->srt.transl.x;
    sState->unk18 = cam->srt.transl.y;
    sState->unk20 = cam->srt.transl.z;
    sState->unk28 = cam->srt.yaw;
    sState->unk30 = cam->srt.pitch;
    sState->unk38 = cam->srt.roll;
    sState->unk40 = cam->fov;
    sState->unk14 = arg1->x;
    sState->unk1C = arg1->y;
    sState->unk24 = arg1->z;
    sState->unk2C = arg2;
    sState->unk34 = arg3;
    sState->unk3C = arg4;
    sState->unk44 = arg5;
    sState->unk5C = 0.0f;
    temp_fv0 = sState->unk14 - sState->unk10;
    temp_fv1 = sState->unk1C - sState->unk18;
    temp_fa1 = sState->unk24 - sState->unk20;
    sState->unk60 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));
    gDLL_2_Camera->vtbl->func12(sState->unk60, &sState->unk48, 100.0f, 0.1f, 0.1f, -5.0f);
}

// offset: 0x1004 | func: 5
static s32 campath_func_1004(Cam* cam, u8 arg1) {
    f32 var_fs0;
    f32 var_fv1;

    sState->unk14 = cam->srt.transl.x;
    sState->unk1C = cam->srt.transl.y;
    sState->unk24 = cam->srt.transl.z;
    sState->unk2C = cam->srt.yaw;
    sState->unk34 = cam->srt.pitch;
    sState->unk3C = cam->srt.roll;
    sState->unk44 = cam->fov;
    if (sState->unk60 != 0.0f) {
        var_fs0 = sState->unk5C / sState->unk60;
    } else {
        var_fs0 = 0.0f;
    }
    if (var_fs0 > 1.0f) {
        var_fs0 = 1.0f;
    }
    var_fv1 = curvesHermite(&sState->unk48.x, var_fs0, NULL);
    if (var_fv1 < 0.2f) {
        var_fv1 = 0.2f;
    }
    sState->unk5C += var_fv1 * gUpdateRateF;
    if (sState->unk60 != 0.0f) {
        var_fs0 = sState->unk5C / sState->unk60;
    } else {
        var_fs0 = 0.0f;
    }
    if (var_fs0 > 1.0f) {
        var_fs0 = 1.0f;
    }
    cam->srt.transl.x = curvesLinear(&sState->unk10, var_fs0, NULL);
    cam->srt.transl.y = curvesLinear(&sState->unk18, var_fs0, NULL);
    cam->srt.transl.z = curvesLinear(&sState->unk20, var_fs0, NULL);
    cam->fov = curvesLinear(&sState->unk40, var_fs0, NULL);
    if (((sState->unk28 - sState->unk2C) > 32768.0f) || ((sState->unk28 - sState->unk2C) < -32768.0f)) {
        if (sState->unk28 < 0.0f) {
            sState->unk28 += 65535.0f;
        } else if (sState->unk2C < 0.0f) {
            sState->unk2C += 65535.0f;
        }
    }
    if (((sState->unk30 - sState->unk34) > 32768.0f) || ((sState->unk30 - sState->unk34) < -32768.0f)) {
        if (sState->unk30 < 0.0f) {
            sState->unk30 += 65535.0f;
        } else if (sState->unk34 < 0.0f) {
            sState->unk34 += 65535.0f;
        }
    }
    if (((sState->unk38 - sState->unk3C) > 32768.0f) || ((sState->unk38 - sState->unk3C) < -32768.0f)) {
        if (sState->unk38 < 0.0f) {
            sState->unk38 += 65535.0f;
        } else if (sState->unk3C < 0.0f) {
            sState->unk3C += 65535.0f;
        }
    }
    if (!(arg1 & 1)) {
        cam->srt.yaw = (s16) curvesLinear(&sState->unk28, var_fs0, NULL);
    }
    if (!(arg1 & 2)) {
        cam->srt.pitch = (s16) curvesLinear(&sState->unk30, var_fs0, NULL);
    }
    if (!(arg1 & 4)) {
        cam->srt.roll = (s16) curvesLinear(&sState->unk38, var_fs0, NULL);
    }
 
    return var_fs0 >= 1.0f;
}

// offset: 0x14D8 | func: 6
static f32 campath_func_14D8(f32 arg0, f32 arg1, f32 arg2, s32* arg3) {
    f32 sp8C;
    f32 sp88;
    f32 var_f18;
    f32 var_f16;
    f32 sp7C;
    f32 sp78;
    f32 pad_sp74;
    f32 pad_sp70;
    f32 temp_fv0;
    f32 sp68;
    f32 var_fa0;
    f32 var_fs0;
    s32 i;
    CurveSetup* sp4C[4];
    f32 temp;

    for (i = 0; i < 4; i++) sp4C[i] = gDLL_26_Curves->vtbl->func_39C(arg3[i]);

    sp7C = var_f18 = sp4C[2]->pos.x - sp4C[1]->pos.x;
    sp78 = var_f16 = sp4C[2]->pos.z - sp4C[1]->pos.z;
    if (sp4C[0] != NULL) {
        sp8C = sp4C[1]->pos.x - sp4C[0]->pos.x;
        sp88 = sp4C[1]->pos.z - sp4C[0]->pos.z;
    } else {
        sp8C = var_f18;
        sp88 = var_f16;
    }
    sp8C = (sp8C + var_f18) * 0.5f;
    sp88 = (sp88 + var_f16) * 0.5f;
    temp_fv0 = sqrtf(SQ(sp8C) + SQ(sp88));
    if (temp_fv0 != 0.0f) {
        sp8C /= temp_fv0;
        sp88 /= temp_fv0;
    }
    sp68 = -((sp8C * sp4C[1]->pos.x) + (sp4C[1]->pos.z * sp88));
    var_fs0 = (sp8C * var_f18) + (sp88 * var_f16);
    if (var_fs0 != 0.0f) {
        var_fs0 = -((sp8C * arg0) + (sp88 * arg2) + sp68) / var_fs0;
    }

    var_f18 = sp4C[2]->pos.x - sp4C[1]->pos.x;
    var_f16 = sp4C[2]->pos.z - sp4C[1]->pos.z;
    if (sp4C[3] != NULL) {
        sp8C = sp4C[3]->pos.x - sp4C[2]->pos.x;
        sp88 = sp4C[3]->pos.z - sp4C[2]->pos.z;
    } else {
        sp8C = var_f18;
        sp88 = var_f16;
    }
    sp8C = (sp8C + var_f18) * 0.5f;
    sp88 = (sp88 + var_f16) * 0.5f;
    temp_fv0 = sqrtf(SQ(sp8C) + SQ(sp88));
    if (temp_fv0 != 0.0f) {
        sp8C /= temp_fv0;
        sp88 /= temp_fv0;
    }
    temp = -((sp8C * sp4C[2]->pos.x) + (sp4C[2]->pos.z * sp88));
    var_fa0 = (sp8C * sp7C) + (sp88 * sp78);
    if (var_fa0 != 0.0f) {
        var_fa0 = -((sp8C * arg0) + (sp88 * arg2) + temp) / var_fa0;
    }
    return var_fs0 = -var_fs0 / (var_fa0 - var_fs0);
}

// offset: 0x17A0 | func: 7
static void campath_func_17A0(CurveSetup* arg0, s32* arg1, s32 arg2) {
    s32 i;
    CurveSetup* sp38;
    CurveSetup* temp_v0;

    arg1[0] = -1;
    arg1[1] = -1;
    arg1[2] = -1;
    arg1[3] = -1;
    if (arg0 != NULL) {
        arg1[1] = arg0->uID;
        i = 0;
        while (i < 5) {
            if (arg0->links[i] >= 0) {
                temp_v0 = gDLL_26_Curves->vtbl->func_39C(arg0->links[i]);
                if ((temp_v0 != NULL) && ((arg2 == temp_v0->campath.unk31) || (arg2 == temp_v0->campath.unk32) || (arg2 == temp_v0->campath.unk33))) {
                    if ((arg0->unk1B & (1 << i)) != 0) {
                        arg1[0] = arg0->links[i];
                    } else if ((arg0->unk1B & (1 << i)) == 0) {
                        arg1[2] = arg0->links[i];
                    }
                }
            }
            i += 1;
        }
        if (arg1[2] > -1) {
            sp38 = gDLL_26_Curves->vtbl->func_39C(arg1[2]);
            if (sp38 != NULL) {
                if ((arg2 == sp38->campath.unk31) || (arg2 == sp38->campath.unk32) || (arg2 == sp38->campath.unk33)) {
                    i = 0;
                    while (i < 5) {
                        if ((sp38->links[i] >= 0) && !(sp38->unk1B & (1 << i))) {
                            temp_v0 = gDLL_26_Curves->vtbl->func_39C(sp38->links[i]);
                            if ((temp_v0 != NULL) && ((arg2 == temp_v0->campath.unk31) || (arg2 == temp_v0->campath.unk32) || (arg2 == temp_v0->campath.unk33))) {
                                arg1[3] = sp38->links[i];
                            }
                        }
                        i += 1;
                    }
                }
            }
        }
        if (arg1[1] < 0 || arg1[2] < 0) {
            if (1) {} // @fake
            STUBBED_PRINTF("PATHCAM error: need at least two control points\n");
        }
    }
}

// offset: 0x19AC | func: 8
static void campath_func_19AC(s32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    f32* var_v1;
    s32 var_a1;
    s32 var_s0;
    CurveSetup* sp5C[4];

    var_s0 = 0;
    while (var_s0 < 4) {
        sp5C[var_s0] = gDLL_26_Curves->vtbl->func_39C(arg0[var_s0]);
        if (sp5C[var_s0] != NULL) {
            arg1[var_s0] = sp5C[var_s0]->pos.x;
            arg2[var_s0] = sp5C[var_s0]->pos.y;
            arg3[var_s0] = sp5C[var_s0]->pos.z;
            arg4[var_s0] = (f32) sp5C[var_s0]->campath.unk34;
            arg5[var_s0] = (f32) sp5C[var_s0]->campath.unk36;
            arg6[var_s0] = (f32) sp5C[var_s0]->campath.unk38;
            arg7[var_s0] = (f32) sp5C[var_s0]->campath.unk3A;
        }
        var_s0 += 1;
    }
    if (sp5C[1] != NULL && sp5C[2] != NULL) {
        var_s0 = 0;
        while (var_s0 < 4) {
            if (sp5C[var_s0] == NULL) {
                if (var_s0 == 0) {
                    arg1[var_s0] = sp5C[1]->pos.x + (sp5C[1]->pos.x - sp5C[2]->pos.x);
                    arg2[var_s0] = sp5C[1]->pos.y + (sp5C[1]->pos.y - sp5C[2]->pos.y);
                    arg3[var_s0] = sp5C[1]->pos.z + (sp5C[1]->pos.z - sp5C[2]->pos.z);
                    arg4[var_s0] = (f32) (sp5C[1]->campath.unk34 + (sp5C[1]->campath.unk34 - sp5C[2]->campath.unk34));
                    arg5[var_s0] = (f32) (sp5C[1]->campath.unk36 + (sp5C[1]->campath.unk36 - sp5C[2]->campath.unk36));
                    arg6[var_s0] = (f32) (sp5C[1]->campath.unk38 + (sp5C[1]->campath.unk38 - sp5C[2]->campath.unk38));
                    arg7[var_s0] = (f32) sp5C[1]->campath.unk3A + ((f32) sp5C[1]->campath.unk3A - (f32) sp5C[2]->campath.unk3A);
                } else if (var_s0 == 3) {
                    arg1[var_s0] = sp5C[2]->pos.x + (sp5C[2]->pos.x - sp5C[1]->pos.x);
                    arg2[var_s0] = sp5C[2]->pos.y + (sp5C[2]->pos.y - sp5C[1]->pos.y);
                    arg3[var_s0] = sp5C[2]->pos.z + (sp5C[2]->pos.z - sp5C[1]->pos.z);
                    arg4[var_s0] = (f32) (sp5C[2]->campath.unk34 + (sp5C[2]->campath.unk34 - sp5C[1]->campath.unk34));
                    arg5[var_s0] = (f32) (sp5C[2]->campath.unk36 + (sp5C[2]->campath.unk36 - sp5C[1]->campath.unk36));
                    arg6[var_s0] = (f32) (sp5C[2]->campath.unk38 + (sp5C[2]->campath.unk38 - sp5C[1]->campath.unk38));
                    arg7[var_s0] = (f32) sp5C[2]->campath.unk3A + ((f32) sp5C[2]->campath.unk3A - (f32) sp5C[1]->campath.unk3A);
                }
            }
            var_s0 += 1;
        }
        var_a1 = 0;
        while (var_a1 < 3) {
            if (var_a1 == 0) {
                var_v1 = arg4;
            } else if (var_a1 == 1) {
                var_v1 = arg5;
            } else {
                var_v1 = arg6;
            }
            if (var_v1 != NULL) {
                var_s0 = 0;
                while (var_s0 < 3) {
                    if (((var_v1[var_s0] - var_v1[var_s0 + 1]) > 32768.0f) || ((var_v1[var_s0] - var_v1[var_s0 + 1]) < -32768.0f)) {
                        if (var_v1[var_s0] < 0.0f) {
                            var_v1[var_s0] += 65535.0f;
                        } else if (var_v1[var_s0 + 1] < 0.0f) {
                            var_v1[var_s0 + 1] += 65535.0f;
                        }
                    }
                    var_s0++;
                }
            }
            var_a1 += 1;
        }
    }
}

// offset: 0x1E5C | func: 9
static CurveSetup* campath_func_1E5C(CurveSetup* arg0, s32* arg1, s32 arg2) {
    CurveSetup* temp_v0_2;
    s32 temp_a0;
    s32 var_s0;
    s32 var_s3;

    *arg1 = 0;
    do {
        var_s3 = 1;
        if (arg0->curveType != 0x1B && arg0->curveType != 0x1A) {
            var_s0 = 0;
            while (var_s0 < 5) {
                temp_a0 = arg0->links[var_s0];
                if ((temp_a0 >= 0) && (arg0->unk1B & (1 << var_s0))) {
                    temp_v0_2 = gDLL_26_Curves->vtbl->func_39C(temp_a0);
                    if ((temp_v0_2 != NULL) && ((arg2 == temp_v0_2->campath.unk31) || (arg2 == temp_v0_2->campath.unk32) || (arg2 == temp_v0_2->campath.unk33))) {
                        arg0 = temp_v0_2;
                        var_s3 = 0;
                        var_s0 = 5; // break
                    }
                }
                var_s0 += 1;
            }
        }
        if (var_s3 == 0) {
            *arg1 += 1;
        }
    } while (var_s3 == 0);
    return arg0;
}

// offset: 0x1F9C | func: 10
static void campath_func_1F9C(s32* arg0, s32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    CurveSetup* temp_s1;
    CurveSetup* temp_s2;
    CurveSetup* temp_v0;
    s32 sp68;
    s32 sp64;
    s32 temp_a0;
    s32 sp50[4];
    f32 temp_fv0;
    s32 var_s0;
    s32 var_s0_3;
    s32 var_s4;

    temp_s1 = gDLL_26_Curves->vtbl->func_39C(*arg0);
    var_s0 = 1;
    var_s4 = 0;
    while (var_s4 < 5) {
        temp_a0 = temp_s1->links[var_s4];
        if ((temp_a0 >= 0) && !(temp_s1->unk1B & (1 << var_s4))) {
            temp_v0 = gDLL_26_Curves->vtbl->func_39C(temp_a0);
            if ((temp_v0 != NULL) && ((arg5 == temp_v0->campath.unk31) || (arg5 == temp_v0->campath.unk32) || (arg5 == temp_v0->campath.unk33))) {
                var_s0 = 0;
                var_s4 = 5; // break
            }
        }
        var_s4 += 1;
    }
    if (var_s0 != 0) {
        var_s4 = 0;
        while (var_s4 < 5) {
            temp_a0 = temp_s1->links[var_s4];
            if ((temp_a0 >= 0) && (temp_s1->unk1B & (1 << var_s4))) {
                temp_v0 = gDLL_26_Curves->vtbl->func_39C(temp_a0);
                if ((temp_v0 != NULL) && ((arg5 == temp_v0->campath.unk31) || (arg5 == temp_v0->campath.unk32) || (arg5 == temp_v0->campath.unk33))) {
                    *arg0 = temp_s1->links[var_s4];
                    var_s4 = 5; // break
                }
            }
            var_s4 += 1;
        }
    }
    do {
        var_s0 = 1;
        campath_func_17A0(gDLL_26_Curves->vtbl->func_39C(*arg0), sp50, arg5);
        temp_fv0 = campath_func_14D8(arg2, arg3, arg4, sp50);
        if (temp_fv0 < 0.0f) {
            if (sp50[0] >= 0) {
                *arg0 = sp50[0];
                var_s0 = 0;
            }
        } else if ((temp_fv0 > 1.0f)) {
            if ((sp50[2] >= 0) && (sp50[3] >= 0)) {
                *arg0 = sp50[2];
                var_s0 = 0;
            }
        }
    } while (var_s0 == 0);
    campath_func_1E5C(gDLL_26_Curves->vtbl->func_39C(*arg0), &sp68, arg5);
    *arg1 = campath_func_1E5C(gDLL_26_Curves->vtbl->func_39C(*arg1), &sp64, arg5)->uID;
    var_s4 = 0;
    while (var_s4 < sp68) {
        temp_s2 = gDLL_26_Curves->vtbl->func_39C(*arg1);
        var_s0_3 = 0;
        while (var_s0_3 < 5) {
            temp_a0 = temp_s2->links[var_s0_3];
            if ((temp_a0 >= 0) && !(temp_s2->unk1B & (1 << var_s0_3))) {
                temp_v0 = gDLL_26_Curves->vtbl->func_39C(temp_a0);
                if ((temp_v0 != NULL) && ((arg5 == temp_v0->campath.unk31) || (arg5 == temp_v0->campath.unk32) || (arg5 == temp_v0->campath.unk33))) {
                    *arg1 = temp_s2->links[var_s0_3];
                    var_s0_3 = 5; // break
                }
            }
            var_s0_3 += 1;
        }
        var_s4 += 1;
    }
}
