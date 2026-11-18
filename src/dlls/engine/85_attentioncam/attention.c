#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/84_camnormal.h"
#include "dlls/engine/85_attentioncam.h"
#include "dlls/engine/86_cam1stperson.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "sys/joypad.h"
#include "sys/math.h"
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
    f32 nearestFloorY;
    f32 nearestCeilingY;
    f32 controlPointsX[20];
    f32 controlPointsY[20];
    f32 controlPointsZ[20];
    Vec4f spline;
    f32 skipTimer;
    CurvesStruct curves;
    u8 finished;
} CamAttention;

/*0x0*/ static CamAttention* sState;

static void attentioncam_handleButtons(Cam* cam, Object* player);
static void attentioncam_calculateControlPoints(f32 dx, f32 dz, f32 camX, f32 camY, f32 camZ, f32 goalY, s16 yawDiffx2, s16 thresholdAngle, s32* oControlPointCount);
static void attentioncam_calculateAngleSteps(s16* angles, u16* stepCount, s16 angle, s16 yawDiff, s16 thresholdAngle);
static void attentioncam_aimYawAtPlayer(Cam* cam, f32 dx, f32 dz);
static s32 attentioncam_advanceEase(f32* ox, f32* oy, f32* oz, Object* player);
static void attentioncam_skipToBehindPlayer(Cam* cam, Object* player);

// offset: 0x0 | ctor
void attentioncam_ctor(void* dll) { }

// offset: 0xC | dtor
void attentioncam_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void attentioncam_Setup(Cam* cam, s32 arg1, AttentionCam_Params* data) {
    s16 yawDiff;
    s16 yawDiffx2;
    f32 uGoalX;
    f32 uGoalY;
    f32 uGoalZ;
    f32 normalise;
    Object* player;
    f32 orbitRadius;
    f32 sin;
    f32 cos;
    Vec3f goalPos;
    Vec3f initialPos;
    Vec3f sp108;
    Vec3s16 sp100;
    Vec3s16 spF8;
    Vec3s16 spF0;
    s32 count;
    f32 dx;
    f32 dz;
    s32 playerYaw;
    s32 idx;
    TrackIntersectResult result;
    AABBs32 aabb;
    CamControl_Module* camnormal;
    f32 _pad;

    if (data == NULL) {
        STUBBED_PRINTF(" ERROR: Attention cam given NULL data \n");
    }

    data->doIntersectCheck = TRUE;
    player = cam->player;

    sState = mmAlloc(sizeof(CamAttention), ALLOC_TAG_CAM_COL, ALLOC_NAME("attentioncam"));
    bzero(sState, sizeof(CamAttention));

    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func7(
        &sState->unk0, &sState->unk4, &sState->unk8, 0, &sState->unkC);

    sState->finished = FALSE;

    sin = mathSinfInterp(player->srt.yaw);
    cos = mathCosfInterp(player->srt.yaw);
    dx = cam->srt.transl.x - player->srt.transl.x;
    dz = cam->srt.transl.z - player->srt.transl.z;
    yawDiff = player->srt.yaw - (u16)mathAtan2f(dx, dz);
    CIRCLE_WRAP(yawDiff);
    if (yawDiff < 0) {
        yawDiff = -yawDiff;
    }
    if (yawDiff < (s16) (data->endThresholdDegrees * M_1_DEGREE_F)) {
        sState->finished = TRUE;
        return;
    }

    dx = SQ(sState->unk0) - SQ(sState->unk8);
    if (dx < 5.0f) {
        dx = 5.0f;
    }
    orbitRadius = sqrtf(dx);

    goalPos.x = player->srt.transl.x + (sin * orbitRadius);
    goalPos.y = player->srt.transl.y + sState->unkC + sState->unk8;
    goalPos.z = player->srt.transl.z + (cos * orbitRadius);

    if (data->doIntersectCheck) {
        player->srt.transl.y += sState->unkC;
        vox_func_80007EE0(&player->srt.transl, &sp100);
        vox_func_80007EE0(&goalPos, &spF8);
        vox_func_80007E2C(&sp108, &spF0); // @bug: spF0 is uninitialized, result is unused

        uGoalX = goalPos.x - player->srt.transl.x;
        uGoalY = goalPos.y - player->srt.transl.y;
        uGoalZ = goalPos.z - player->srt.transl.z;
        normalise = sqrtf(SQ(uGoalX) + SQ(uGoalY) + SQ(uGoalZ));
        if (normalise != 0.0f) {
            normalise = 1.0f / normalise;
            uGoalX *= normalise;
            uGoalY *= normalise;
            uGoalZ *= normalise;
        }

        initialPos.x = player->srt.transl.x - (uGoalX * 20.0f);
        initialPos.y = player->srt.transl.y - (uGoalY * 20.0f);
        initialPos.z = player->srt.transl.z - (uGoalZ * 20.0f);
        initialPos.y = goalPos.y;
        result.unk40[0] = 4.5f;
        result.unk50[0] = -1;
        result.unk54[0] = 3;
        trackIntersectBuildAABB(&aabb, &player->srt.transl, &goalPos, &result.unk40[0], 1);
        trackIntersectBroadphase(player, &aabb, 1);
        trackGetIntersect(player, initialPos.f, goalPos.f, 1, &result, 0);
        player->srt.transl.y -= sState->unkC;
    }

    for (count = 0; count < 3; count++) {
        sState->controlPointsX[count] = cam->srt.transl.x;
        sState->controlPointsY[count] = cam->srt.transl.y;
        sState->controlPointsZ[count] = cam->srt.transl.z;
    }

    uGoalX = cam->srt.transl.x - goalPos.x;
    uGoalZ = cam->srt.transl.z - goalPos.z;
    normalise = 0.5f * sqrtf(SQ(uGoalX) + SQ(uGoalZ));
    playerYaw = mathAtan2f(-sin, -cos);
    yawDiff = playerYaw - (u16)mathAtan2f(uGoalX, uGoalZ);\
    CIRCLE_WRAP(yawDiff);
    yawDiffx2 = yawDiff;
    if (yawDiff < 0) {
        yawDiff = -yawDiff;
    }
    if (yawDiff > M_90_DEGREES) {
        yawDiff = 0;
    } else {
        yawDiff = M_90_DEGREES - yawDiff;
    }

    if (yawDiffx2 < 0) {
        yawDiffx2 = -(yawDiff * 2);
    } else {
        yawDiffx2 = yawDiff * 2;
    }

    if (yawDiff != 0) {
        normalise = normalise / mathSinfInterp(yawDiff);
    } else {
        normalise = 0.0f;
    }
    dx = goalPos.x - (sin * normalise);
    dz = goalPos.z - (cos * normalise);
    sState->curves.unk84 = sState->controlPointsX;
    sState->curves.unk88 = sState->controlPointsY;
    sState->curves.unk8C = sState->controlPointsZ;
    sState->curves.splineFunc = curvesBSpline;
    sState->curves.splineConverterFunc = curvesBSplineConverter;

    attentioncam_calculateControlPoints(
        dx, 
        dz, 
        cam->srt.transl.x, 
        cam->srt.transl.y, 
        cam->srt.transl.z, 
        goalPos.y, 
        yawDiffx2, 
        M_30_DEGREES, 
        &count);

    for (idx = count; idx < (count + 3); idx++) {
        sState->controlPointsX[idx] = goalPos.x;
        sState->controlPointsY[idx] = goalPos.y;
        sState->controlPointsZ[idx] = goalPos.z;
    }

    sState->curves.numControlPoints = idx;
    sState->curves.unk80 = 0;
    curvesMove(&sState->curves);
    if (yawDiffx2 < 0) {
        yawDiff = yawDiffx2 * -1;
    } else {
        yawDiff = yawDiffx2;
    }

    if ((yawDiff >= M_45_DEGREES) && data->playWhooshSound) {
        dll_amSfx->Play(player, SOUND_1E, MAX_VOLUME, NULL, NULL, 0, NULL);
    }

    gDLL_2_Camera->vtbl->func12(sState->curves.unkC, &sState->spline, 20.0f, 0.5f, 1.0f, -10.0f);

    sState->nearestFloorY = -100000.0f;
    sState->nearestCeilingY = 100000.0f;
}

// offset: 0x848 | func: 1 | export: 1
void attentioncam_Control(Cam* cam) {
    u8 finished;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distance2D;
    Vec3f coordsNew;
    Object* player;
    CamControl_Module* camnormal;

    if (sState->finished) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
        return;
    }

    player = cam->player;
    coordsNew.y = cam->srt.transl.y;
    finished = attentioncam_advanceEase(&coordsNew.x, &coordsNew.y, &coordsNew.z, player);
    
    cam->srt.transl.x = coordsNew.x;
    cam->srt.transl.z = coordsNew.z;
    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func8(cam, 3, 3, &sState->nearestFloorY, &sState->nearestCeilingY);

    //Skip the ease if the camera gets stuck for too long
    if (cam->unk34.unk68) {
        sState->skipTimer += gUpdateRateF;
    }
    if (sState->skipTimer > 10.0f) {
        attentioncam_skipToBehindPlayer(cam, player);
        finished = TRUE;
    }

    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &dx, &dy, &dz, &distance2D, 0.0f);
    attentioncam_aimYawAtPlayer(cam, dx, dz);

    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func5(cam, player->srt.transl.y, distance2D);
    if (finished) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
    }

    attentioncam_handleButtons(cam, player);
}

// offset: 0xAC4 | func: 2 | export: 2
void attentioncam_Free(Cam* cam) {
    mmFree(sState);
}

// offset: 0xB04 | func: 3 | export: 3
void attentioncam_Func_B04(void* params, s32 arg1) {

}

// offset: 0xB14 | func: 4
static void attentioncam_handleButtons(Cam* cam, Object* player) {
    u16 btns;
    Cam1stPerson_Params cam1stPerson;
    u8 lockOnBit;

    if (player->animObj) {
        return;
    }

    btns = joyGetPressed(0);

    if (((cam->highlight != NULL) || (cam->srt.flags & OBJFLAG_UNK_2)) && 
            !(cam->highlightFlags & 2) && 
            (((lockOnBit = mainGetBits(BIT_4AD), (lockOnBit == FALSE)) && (btns & Z_TRIG)) || (lockOnBit && (btns & R_TRIG)) || (cam->targetFlags & 2)) && 
            (player->controlNo == OBJCONTROL_Player) && 
            ((((DLL_210_Player*)player->dll)->vtbl->func60(player) != 0))) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMLOCKON, TRUE, 0, sizeof(&cam->highlight), &cam->highlight, 0x3C, Cam_Ease_All);
    } else if ((btns & U_CBUTTONS) && !(cam->targetFlags & 1)) {
        cam1stPerson.unk0 = sState->unk4;
        cam1stPerson.unk4 = sState->unk8;
        cam1stPerson.unk8 = sState->unkC;
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAM1STPERSON, TRUE, 0, sizeof(cam1stPerson), &cam1stPerson, 0, Cam_Ease_All);
    }
}

// offset: 0xD70 | func: 5
static void attentioncam_calculateControlPoints(f32 dx, f32 dz, f32 camX, f32 camY, f32 camZ, f32 goalY, s16 yawDiffx2, s16 thresholdAngle, s32* oControlPointCount) {
    s16 angles[20];
    s16 rotation[3];
    s16 yawDiff;
    u16 stepCount;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    s32 i;
    s32 pointIdx;
    f32 delta[3];

    if (yawDiffx2 < 0) {
        yawDiff = yawDiffx2 * -1;
    } else {
        yawDiff = yawDiffx2;
    }

    stepCount = 0;
    attentioncam_calculateAngleSteps(angles, &stepCount, 0, yawDiff, thresholdAngle);

    deltaX = camX - dx;
    deltaY = goalY - camY;
    deltaZ = camZ - dz;

    for (i = 1, pointIdx = 3; i < stepCount; i++, pointIdx++) {
        delta[0] = deltaX;
        delta[1] = deltaY;
        delta[2] = deltaZ;
        rotation[0] = yawDiffx2 < 0 ? angles[i] : -angles[i];
        rotation[1] = 0;
        rotation[2] = 0;
        mathRotateRPY((SRT*)&rotation, delta);
        sState->controlPointsX[pointIdx] = dx + delta[0];
        sState->controlPointsY[pointIdx] = camY + (deltaY * ((f32) angles[i] / (f32) yawDiff));
        sState->controlPointsZ[pointIdx] = dz +  delta[2];
    }

    *oControlPointCount = pointIdx;
}

// offset: 0xF84 | func: 6
static void attentioncam_calculateAngleSteps(s16* angles, u16* stepCount, s16 angle, s16 yawDiff, s16 thresholdAngle) {
    if (yawDiff >= thresholdAngle) {
        attentioncam_calculateAngleSteps(angles, stepCount, angle, yawDiff >> 1, thresholdAngle);
        attentioncam_calculateAngleSteps(angles, stepCount, angle + (yawDiff >> 1), yawDiff >> 1, thresholdAngle);
        return;
    }
    angles[(*stepCount)++] = angle;
}

// offset: 0x109C | func: 7
static void attentioncam_aimYawAtPlayer(Cam* cam, f32 dx, f32 dz) {
    s32 yawDiff = (-mathAtan2f(dx, dz) - (cam->srt.yaw & 0xFFFF)) + M_180_DEGREES;
    CIRCLE_WRAP(yawDiff);
    cam->srt.yaw += yawDiff;
}

// offset: 0x112C | func: 8
static s32 attentioncam_advanceEase(f32* ox, f32* oy, f32* oz, Object* player) {
    s32 i;
    s32 finished;
    f32 t;
    CamControl_Module* camnormal;
    Cam cam;

    bzero(&cam, sizeof(cam));
    cam.srt.transl.x = sState->controlPointsX[sState->curves.numControlPoints - 2];
    cam.srt.transl.y = *oy;
    cam.srt.transl.z = sState->controlPointsZ[sState->curves.numControlPoints - 2];
    cam.player = player;
    cam.positionMirror.x = cam.srt.transl.x;
    cam.positionMirror.y = cam.srt.transl.y;
    cam.positionMirror.z = cam.srt.transl.z;
    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func4(&cam, player);
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func8(&cam, 1, 3, &sState->nearestFloorY, &sState->nearestCeilingY);
    
    for (i = sState->curves.numControlPoints - 3; i < sState->curves.numControlPoints; i++) {
        sState->controlPointsX[i] = cam.srt.transl.x;
        sState->controlPointsZ[i] = cam.srt.transl.z;
    }

    if (sState->curves.unkC != 0.0f) {
        t = sState->curves.unk8 / sState->curves.unkC;
    } else {
        t = 0.0f;
    }

    if (t > 1.0f) {
        t = 1.0f;
    } else if (t < 0.0f) {
        t = 0.0f;
    }

    t = curvesHermite(&sState->spline.x, t, NULL);
    if (t < 0.2f) {
        t = 0.2f;
    }

    finished = curves_func_800053B0(&sState->curves, t);

    *ox = sState->curves.unk68.x;
    *oz = sState->curves.unk68.z;

    return finished;
}

// offset: 0x1374 | func: 9
static void attentioncam_skipToBehindPlayer(Cam* cam, Object* player) {
    f32 orbitDistance;
    f32 sin;
    f32 cos;
    Vec3f initial;
    Vec3f goal;
    AABBs32 aabb;
    TrackIntersectResult result;

    sin = mathSinfInterp(player->srt.yaw);
    cos = mathCosfInterp(player->srt.yaw);
    orbitDistance = sqrtf(SQ(sState->unk4) - SQ(sState->unk8));
    goal.x = player->globalPosition.x + (sin * orbitDistance);
    goal.y = player->globalPosition.y + sState->unkC + sState->unk8;
    goal.z = player->globalPosition.z + (cos * orbitDistance);
    initial.x = player->globalPosition.x;
    initial.y = goal.y;
    initial.z = player->globalPosition.z;
    result.unk40[0] = 4.5f;
    result.unk50[0] = -1;
    result.unk54[0] = 3;
    trackIntersectBuildAABB(&aabb, &initial, &goal, &result.unk40[0], 1);
    trackIntersectBroadphase(player, &aabb, 0);
    trackGetIntersect(player, initial.f, goal.f, 1, &result, 0);
    cam->srt.transl.x = goal.x;
    cam->srt.transl.y = goal.y;
    cam->srt.transl.z = goal.z;
}
