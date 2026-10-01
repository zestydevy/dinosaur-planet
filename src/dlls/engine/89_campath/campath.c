#include "PR/ultratypes.h"
#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/89_campath.h"
#include "sys/curves.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "dll.h"
#include "macros.h"

// Camera that follows a set curve as the player moves along it

// size: 0x68
typedef struct {
    SRT* unk0;
    s32 pathID;
    s32 movePath;
    s32 controlPath;
    f32 x;
    f32 goalX;
    f32 y;
    f32 goalY;
    f32 z;
    f32 goalZ;
    f32 yaw;
    f32 goalYaw;
    f32 pitch;
    f32 goalPitch;
    f32 roll;
    f32 goalRoll;
    f32 fov;
    f32 goalFov;
    Vec4f easeSpline;
    f32 tValueEase;
    f32 easedDistance;
    f32 goalDistance;
    u8 easeInFinished;
    u8 pathCamFinished;
} CamPath;

/*0x0*/ static CamPath* sState;

static void campath_setupEase(Cam* cam, Vec3f* goalCoords, s32 goalYaw, s32 goalPitch, s32 goalRoll, f32 goalFov);
static s32 campath_ease(Cam* cam, u8 flags);
static f32 campath_func_14D8(f32 x, f32 y, f32 z, s32* linkIDs);
static void campath_findPathLinks(CurveSetup* curveSetup, s32* linkUIDs, s32 pathID);
static void campath_prepareSplines(s32* pathLinkIDs, f32* splineX, f32* splineY, f32* splineZ, f32* splineYaw, f32* splinePitch, f32* splineRoll, f32* splineFov);
static void campath_func_1F9C(s32* arg0, s32* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);

// offset: 0x0 | ctor
void campath_ctor(void* dll) { }

// offset: 0xC | dtor
void campath_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void campath_func_18(Cam* cam, s32 arg1, CamPath_Params* data) {
    Object* player;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 _pad;
    s16 yaw;
    s16 pitch;
    s16 roll;
    f32 tValue;
    CurveSetup* movePathSetup;
    CurveSetup* controlPathSetup;
    Vec3f goalCoords;
    s32 controlPathLinkIDs[4];
    s32 movePathLinkIDs[4];
    s32 curveTypes[2];
    f32 splineYaw[4];
    f32 splinePitch[4];
    f32 splineRoll[4];
    f32 splineFOV[4];
    f32 splineX[4];
    f32 splineY[4];
    f32 splineZ[4];
    f32 fov;
    s32 _pad2[8];

    player = cam->player;
    sState = mmAlloc(sizeof(CamPath), ALLOC_TAG_CAM_COL, ALLOC_NAME("campath"));
    bzero(sState, sizeof(CamPath));
    sState->pathID = data->pathID;
    sState->easeInFinished = TRUE;
    curveTypes[0] = 9;
    curveTypes[1] = 0x1B;
    sState->controlPath = gDLL_26_Curves->vtbl->func_1E4(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, curveTypes, ARRAYCOUNT(curveTypes), sState->pathID);
    curveTypes[0] = 8;
    curveTypes[1] = 0x1A;
    sState->movePath = gDLL_26_Curves->vtbl->func_1E4(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, curveTypes, ARRAYCOUNT(curveTypes), sState->pathID);
    /* default.dol
    if (sState->movePath < 0 || sState->controlPath < 0) {
        if (sState->movePath < 0) {
            STUBBED_PRINTF("Warning: No camera move path found to associate path camera with!!\n");
        } else {
            STUBBED_PRINTF("Warning: No camera control path found to associate path camera with!!\n");
        }
        sState->pathCamFinished = TRUE;
        return;
    }
    */
    campath_func_1F9C(&sState->controlPath, &sState->movePath, player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, sState->pathID);
    movePathSetup = gDLL_26_Curves->vtbl->func_39C(sState->movePath);
    controlPathSetup = gDLL_26_Curves->vtbl->func_39C(sState->controlPath);
    campath_findPathLinks(movePathSetup, movePathLinkIDs, sState->pathID);
    campath_findPathLinks(controlPathSetup, controlPathLinkIDs, sState->pathID);
    campath_prepareSplines(movePathLinkIDs, splineX, splineY, splineZ, splineYaw, splinePitch, splineRoll, splineFOV);
    
    tValue = campath_func_14D8(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, controlPathLinkIDs);
    if (tValue < 0.0f) {
        tValue = 0.0f;
    } else if (tValue > 1.0f) {
        tValue = 1.0f;
    }

    x = curvesBSpline(splineX, tValue, NULL);
    y = curvesBSpline(splineY, tValue, NULL);
    z = curvesBSpline(splineZ, tValue, NULL);

    dx = x - player->srt.transl.x;
    dy = y - player->srt.transl.y;
    dz = z - player->srt.transl.z;

    if (movePathSetup->campath.unk3B & CamPath_FLAG_Aim_Yaw_at_Player) {
        yaw = M_180_DEGREES - mathAtan2f(dx, dz);
    } else {
        yaw = (s16) curvesCatmullRom(splineYaw, tValue, NULL) + M_180_DEGREES;
    }

    if (movePathSetup->campath.unk3B & CamPath_FLAG_Use_Player_Roll) {
        roll = player->srt.roll;
    } else {
        roll = curvesCatmullRom(splineRoll, tValue, NULL);
    }

    if (movePathSetup->campath.unk3B & CamPath_FLAG_Aim_Pitch_at_Player) {
        pitch = mathAtan2f(dy, sqrtf(SQ(dx) + SQ(dz)));
        pitch -= curvesCatmullRom(splinePitch, tValue, NULL);
    } else {
        pitch = curvesCatmullRom(splinePitch, tValue, NULL);
    }

    fov = curvesBSpline(splineFOV, tValue, NULL);

    goalCoords.x = x;
    goalCoords.y = y;
    goalCoords.z = z;

    if ((data->previousCameraEasesIn == FALSE) && (arg1 != 3)) {
        //CamPath manages the initial ease in
        campath_setupEase(cam, &goalCoords, yaw, pitch, roll, fov);
    } else {
        cam->srt.transl.x = x;
        cam->srt.transl.y = y;
        cam->srt.transl.z = z;
        cam->srt.yaw = yaw;
        cam->srt.pitch = pitch;
        cam->srt.roll = roll;
        cam->fov = fov;
    }

    sState->tValueEase = tValue;
}

// offset: 0x588 | func: 1 | export: 1
void campath_func_588(Cam* cam) {
    f32 tValue;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 spE8[4];
    s32 movePathLinkIDs[4];
    CurveSetup* setup;
    f32 dLateral;
    s32 pad_spCC;
    u8 flags;
    f32 splineX[4];
    f32 splineY[4];
    f32 splineZ[4];
    f32 splineYaw[4];
    f32 splinePitch[4];
    f32 splineRoll[4];
    f32 splineFov[4];
    s32 angleDiff;
    s32 btns;
    Object* player;

    if (sState->pathCamFinished) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
        return;
    }

    player = cam->player;
    btns = joyGetPressed(0);
    setup = gDLL_26_Curves->vtbl->func_39C(sState->controlPath);
    campath_findPathLinks(gDLL_26_Curves->vtbl->func_39C(sState->movePath), movePathLinkIDs, sState->pathID);
    campath_findPathLinks(setup, spE8, sState->pathID);
    campath_prepareSplines(movePathLinkIDs, splineX, splineY, splineZ, splineYaw, splinePitch, splineRoll, splineFov);
    
    tValue = campath_func_14D8(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, spE8);
    if (tValue < 0.0f) {
        if (spE8[0] >= 0) {
            sState->controlPath = spE8[0];
            campath_findPathLinks(gDLL_26_Curves->vtbl->func_39C(sState->controlPath), spE8, sState->pathID);
            if (movePathLinkIDs[0] >= 0) {
                sState->movePath = movePathLinkIDs[0];
                campath_findPathLinks(gDLL_26_Curves->vtbl->func_39C(sState->movePath), movePathLinkIDs, sState->pathID);
                campath_prepareSplines(movePathLinkIDs, splineX, splineY, splineZ, splineYaw, splinePitch, splineRoll, splineFov);
                tValue = campath_func_14D8(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, spE8);
                sState->tValueEase += 1.0f;
            } else {
                tValue = 0.0f;
            }
        } else {
            tValue = 0.0f;
        }
    } else if (tValue > 1.0f) {
        if ((spE8[2] >= 0) && (spE8[3] >= 0)) {
            sState->controlPath = spE8[2];
            campath_findPathLinks(gDLL_26_Curves->vtbl->func_39C(sState->controlPath), spE8, sState->pathID);
            if ((movePathLinkIDs[2] >= 0) && (movePathLinkIDs[3] >= 0)) {
                sState->movePath = movePathLinkIDs[2];
                campath_findPathLinks(gDLL_26_Curves->vtbl->func_39C(sState->movePath), movePathLinkIDs, sState->pathID);
                campath_prepareSplines(movePathLinkIDs, splineX, splineY, splineZ, splineYaw, splinePitch, splineRoll, splineFov);
                tValue = campath_func_14D8(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, spE8);
                sState->tValueEase -= 1.0f;
            } else {
                tValue = 1.0f;
            }
        } else {
            tValue = 1.0f;
        }
    }
    tValue = sState->tValueEase + ((tValue - sState->tValueEase) * 0.3f);
    sState->tValueEase = tValue;

    cam->srt.transl.x = curvesBSpline(splineX, tValue, NULL);
    cam->srt.transl.y = curvesBSpline(splineY, tValue, NULL);
    cam->srt.transl.z = curvesBSpline(splineZ, tValue, NULL);

    flags = gDLL_26_Curves->vtbl->func_39C(sState->movePath)->campath.unk3B;
    if ((flags & CamPath_FLAG_Aim_Yaw_at_Player) == FALSE) {
        cam->srt.yaw = (s16) curvesCatmullRom(splineYaw, tValue, NULL) + M_180_DEGREES;
    }
    if ((flags & CamPath_FLAG_Aim_Pitch_at_Player) == FALSE) {
        cam->srt.pitch = curvesCatmullRom(splinePitch, tValue, NULL);
    }
    if ((flags & CamPath_FLAG_Use_Player_Roll) == FALSE) {
        cam->srt.roll = curvesCatmullRom(splineRoll, tValue, NULL);
    }
    
    cam->fov = curvesBSpline(splineFov, tValue, NULL);

    if (sState->easeInFinished == FALSE && campath_ease(cam, flags)) {
        sState->easeInFinished = TRUE;
    }

    dx = cam->srt.transl.x - player->srt.transl.x;
    dy = cam->srt.transl.y - player->srt.transl.y;
    dz = cam->srt.transl.z - player->srt.transl.z;

    if (flags & CamPath_FLAG_Aim_Yaw_at_Player) {
        cam->srt.yaw = M_180_DEGREES - mathAtan2f(dx, dz);
    }

    if (flags & CamPath_FLAG_Aim_Pitch_at_Player) {
        dLateral = sqrtf(SQ(dx) + SQ(dz));
        angleDiff = mathAtan2f(dy, dLateral);
        angleDiff = (angleDiff - curvesCatmullRom(splinePitch, tValue, NULL)) - (cam->srt.pitch & 0xFFFF);
        CIRCLE_WRAP(angleDiff);
        cam->srt.pitch += (angleDiff * gUpdateRate) >> 3;
    }

    if (flags & CamPath_FLAG_Use_Player_Roll) {
        angleDiff = cam->srt.roll - (player->srt.roll & 0xFFFF);
        CIRCLE_WRAP(angleDiff);
        cam->srt.roll += (angleDiff * gUpdateRate) >> 3;
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
static void campath_setupEase(Cam* cam, Vec3f* goalCoords, s32 goalYaw, s32 goalPitch, s32 goalRoll, f32 goalFov) {
    f32 dz;
    f32 dx;
    f32 dy;

    sState->easeInFinished = FALSE;
    sState->x = cam->srt.transl.x;
    sState->y = cam->srt.transl.y;
    sState->z = cam->srt.transl.z;
    sState->yaw = cam->srt.yaw;
    sState->pitch = cam->srt.pitch;
    sState->roll = cam->srt.roll;
    sState->fov = cam->fov;
    sState->goalX = goalCoords->x;
    sState->goalY = goalCoords->y;
    sState->goalZ = goalCoords->z;
    sState->goalYaw = goalYaw;
    sState->goalPitch = goalPitch;
    sState->goalRoll = goalRoll;
    sState->goalFov = goalFov;
    sState->easedDistance = 0.0f;

    dx = sState->goalX - sState->x;
    dy = sState->goalY - sState->y;
    dz = sState->goalZ - sState->z;
    sState->goalDistance = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));

    gDLL_2_Camera->vtbl->func12(sState->goalDistance, &sState->easeSpline, 100.0f, 0.1f, 0.1f, -5.0f);
}

// offset: 0x1004 | func: 5
static s32 campath_ease(Cam* cam, u8 flags) {
    f32 tValue;
    f32 speed;

    sState->goalX = cam->srt.transl.x;
    sState->goalY = cam->srt.transl.y;
    sState->goalZ = cam->srt.transl.z;
    sState->goalYaw = cam->srt.yaw;
    sState->goalPitch = cam->srt.pitch;
    sState->goalRoll = cam->srt.roll;
    sState->goalFov = cam->fov;
    if (sState->goalDistance != 0.0f) {
        tValue = sState->easedDistance / sState->goalDistance;
    } else {
        tValue = 0.0f;
    }
    if (tValue > 1.0f) {
        tValue = 1.0f;
    }

    speed = curvesHermite(sState->easeSpline.f, tValue, NULL);
    if (speed < 0.2f) {
        speed = 0.2f;
    }
    
    sState->easedDistance += speed * gUpdateRateF;
    if (sState->goalDistance != 0.0f) {
        tValue = sState->easedDistance / sState->goalDistance;
    } else {
        tValue = 0.0f;
    }
    if (tValue > 1.0f) {
        tValue = 1.0f;
    }

    //Ease position and FOV
    cam->srt.transl.x = curvesLinear(&sState->x, tValue, NULL);
    cam->srt.transl.y = curvesLinear(&sState->y, tValue, NULL);
    cam->srt.transl.z = curvesLinear(&sState->z, tValue, NULL);
    cam->fov = curvesLinear(&sState->fov, tValue, NULL);

    //Ensure yaw/pitch/roll take the shortest angular path to their goal
    {
        if (((sState->yaw - sState->goalYaw) > M_180_DEGREES) || ((sState->yaw - sState->goalYaw) < -M_180_DEGREES)) {
            if (sState->yaw < 0.0f) {
                sState->yaw += M_360_DEGREES - 1;
            } else if (sState->goalYaw < 0.0f) {
                sState->goalYaw += M_360_DEGREES - 1;
            }
        }
        if (((sState->pitch - sState->goalPitch) > M_180_DEGREES) || ((sState->pitch - sState->goalPitch) < -M_180_DEGREES)) {
            if (sState->pitch < 0.0f) {
                sState->pitch += M_360_DEGREES - 1;
            } else if (sState->goalPitch < 0.0f) {
                sState->goalPitch += M_360_DEGREES - 1;
            }
        }
        if (((sState->roll - sState->goalRoll) > M_180_DEGREES) || ((sState->roll - sState->goalRoll) < -M_180_DEGREES)) {
            if (sState->roll < 0.0f) {
                sState->roll += M_360_DEGREES - 1;
            } else if (sState->goalRoll < 0.0f) {
                sState->goalRoll += M_360_DEGREES - 1;
            }
        }
    }

    //Ease rotations
    if (!(flags & 1)) {
        cam->srt.yaw = curvesLinear(&sState->yaw, tValue, NULL);
    }
    if (!(flags & 2)) {
        cam->srt.pitch = curvesLinear(&sState->pitch, tValue, NULL);
    }
    if (!(flags & 4)) {
        cam->srt.roll = curvesLinear(&sState->roll, tValue, NULL);
    }
 
    return tValue >= 1.0f;
}

// offset: 0x14D8 | func: 6
static f32 campath_func_14D8(f32 x, f32 y, f32 z, s32* linkIDs) {
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
    CurveSetup* setups[4];
    f32 temp;

    for (i = 0; i < 4; i++) setups[i] = gDLL_26_Curves->vtbl->func_39C(linkIDs[i]);

    sp7C = var_f18 = setups[2]->pos.x - setups[1]->pos.x;
    sp78 = var_f16 = setups[2]->pos.z - setups[1]->pos.z;
    if (setups[0] != NULL) {
        sp8C = setups[1]->pos.x - setups[0]->pos.x;
        sp88 = setups[1]->pos.z - setups[0]->pos.z;
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
    sp68 = -((sp8C * setups[1]->pos.x) + (setups[1]->pos.z * sp88));
    var_fs0 = (sp8C * var_f18) + (sp88 * var_f16);
    if (var_fs0 != 0.0f) {
        var_fs0 = -((sp8C * x) + (sp88 * z) + sp68) / var_fs0;
    }

    var_f18 = setups[2]->pos.x - setups[1]->pos.x;
    var_f16 = setups[2]->pos.z - setups[1]->pos.z;
    if (setups[3] != NULL) {
        sp8C = setups[3]->pos.x - setups[2]->pos.x;
        sp88 = setups[3]->pos.z - setups[2]->pos.z;
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
    temp = -((sp8C * setups[2]->pos.x) + (setups[2]->pos.z * sp88));
    var_fa0 = (sp8C * sp7C) + (sp88 * sp78);
    if (var_fa0 != 0.0f) {
        var_fa0 = -((sp8C * x) + (sp88 * z) + temp) / var_fa0;
    }
    return var_fs0 = -var_fs0 / (var_fa0 - var_fs0);
}

// offset: 0x17A0 | func: 7
static void campath_findPathLinks(CurveSetup* curveSetup, s32* linkUIDs, s32 pathID) {
    s32 i;
    CurveSetup* linkB;
    CurveSetup* linkA;

    linkUIDs[0] = -1;
    linkUIDs[1] = -1;
    linkUIDs[2] = -1;
    linkUIDs[3] = -1;

    if (curveSetup != NULL) {
        linkUIDs[1] = curveSetup->uID;

        for (i = 0; i < 5; i++) {
            if (curveSetup->links[i] >= 0) {
                linkA = gDLL_26_Curves->vtbl->func_39C(curveSetup->links[i]);
                if ((linkA != NULL) && ((pathID == linkA->campath.unk31) || (pathID == linkA->campath.unk32) || (pathID == linkA->campath.unk33))) {
                    if ((curveSetup->unk1B & (1 << i)) != 0) {
                        linkUIDs[0] = curveSetup->links[i];
                    } else if ((curveSetup->unk1B & (1 << i)) == 0) {
                        linkUIDs[2] = curveSetup->links[i];
                    }
                }
            }
        }

        if (linkUIDs[2] > -1) {
            linkB = gDLL_26_Curves->vtbl->func_39C(linkUIDs[2]);
            if (linkB != NULL) {
                if ((pathID == linkB->campath.unk31) || (pathID == linkB->campath.unk32) || (pathID == linkB->campath.unk33)) {
                    for (i = 0; i < 5; i++) {
                        if ((linkB->links[i] >= 0) && !(linkB->unk1B & (1 << i))) {
                            linkA = gDLL_26_Curves->vtbl->func_39C(linkB->links[i]);
                            if ((linkA != NULL) && ((pathID == linkA->campath.unk31) || (pathID == linkA->campath.unk32) || (pathID == linkA->campath.unk33))) {
                                linkUIDs[3] = linkB->links[i];
                            }
                        }
                    }
                }
            }
        }

        if (linkUIDs[1] < 0 || linkUIDs[2] < 0) {
            if (1) {} // @fake
            STUBBED_PRINTF("PATHCAM error: need at least two control points\n");
        }
    }
}

// offset: 0x19AC | func: 8
static void campath_prepareSplines(s32* pathLinkIDs, f32* splineX, f32* splineY, f32* splineZ, f32* splineYaw, f32* splinePitch, f32* splineRoll, f32* splineFov) {
    f32* spline;
    s32 eulerIdx;
    s32 i;
    CurveSetup* link[4];

    for (i = 0; i < 4; i++) {
        link[i] = gDLL_26_Curves->vtbl->func_39C(pathLinkIDs[i]);
        if (link[i] != NULL) {
            splineX[i] = link[i]->pos.x;
            splineY[i] = link[i]->pos.y;
            splineZ[i] = link[i]->pos.z;
            splineYaw[i] = link[i]->campath.unk34;
            splinePitch[i] = link[i]->campath.unk36;
            splineRoll[i] = link[i]->campath.unk38;
            splineFov[i] = link[i]->campath.unk3A;
        }
    }

    if (link[1] != NULL && link[2] != NULL) {
        for (i = 0; i < 4; i++) {
            if (link[i] == NULL) {
                if (i == 0) {
                    splineX[i] = link[1]->pos.x + (link[1]->pos.x - link[2]->pos.x);
                    splineY[i] = link[1]->pos.y + (link[1]->pos.y - link[2]->pos.y);
                    splineZ[i] = link[1]->pos.z + (link[1]->pos.z - link[2]->pos.z);
                    splineYaw[i] = (link[1]->campath.unk34 + (link[1]->campath.unk34 - link[2]->campath.unk34));
                    splinePitch[i] = (link[1]->campath.unk36 + (link[1]->campath.unk36 - link[2]->campath.unk36));
                    splineRoll[i] = (link[1]->campath.unk38 + (link[1]->campath.unk38 - link[2]->campath.unk38));
                    splineFov[i] = link[1]->campath.unk3A + ((f32) link[1]->campath.unk3A - (f32) link[2]->campath.unk3A);
                } else if (i == 3) {
                    splineX[i] = link[2]->pos.x + (link[2]->pos.x - link[1]->pos.x);
                    splineY[i] = link[2]->pos.y + (link[2]->pos.y - link[1]->pos.y);
                    splineZ[i] = link[2]->pos.z + (link[2]->pos.z - link[1]->pos.z);
                    splineYaw[i] = (link[2]->campath.unk34 + (link[2]->campath.unk34 - link[1]->campath.unk34));
                    splinePitch[i] = (link[2]->campath.unk36 + (link[2]->campath.unk36 - link[1]->campath.unk36));
                    splineRoll[i] = (link[2]->campath.unk38 + (link[2]->campath.unk38 - link[1]->campath.unk38));
                    splineFov[i] = link[2]->campath.unk3A + ((f32) link[2]->campath.unk3A - (f32) link[1]->campath.unk3A);
                }
            }
        }

        for (eulerIdx = 0; eulerIdx < 3; eulerIdx++) {
            if (eulerIdx == 0) {
                spline = splineYaw;
            } else if (eulerIdx == 1) {
                spline = splinePitch;
            } else {
                spline = splineRoll;
            }

            if (spline != NULL) {
                for (i = 0; i < 3; i++) {
                    if (((spline[i] - spline[i + 1]) > M_180_DEGREES) || ((spline[i] - spline[i + 1]) < -M_180_DEGREES)) {
                        if (spline[i] < 0.0f) {
                            spline[i] += M_360_DEGREES - 1;
                        } else if (spline[i + 1] < 0.0f) {
                            spline[i + 1] += M_360_DEGREES - 1;
                        }
                    }
                }
            }
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
        campath_findPathLinks(gDLL_26_Curves->vtbl->func_39C(*arg0), sp50, arg5);
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
