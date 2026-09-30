#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/90_camstatic.h"
#include "dlls/objects/715_StaticCamera.h"
#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/objtype.h"
#include "dll.h"

// Positions camera at the location of a StaticCamera object

typedef struct {
    Object* obj;        //StaticCamera object
    u8 _unk4[0x8 - 0x4];
    f32 x;              //Ease's initial value
    f32 goalX;          //Ease's goal value
    f32 y;              //Ease's initial value
    f32 goalY;          //Ease's goal value
    f32 z;              //Ease's initial value
    f32 goalZ;          //Ease's goal value
    f32 yaw;            //Ease's initial value
    f32 goalYaw;        //Ease's goal value
    f32 pitch;          //Ease's initial value
    f32 goalPitch;      //Ease's goal value
    f32 roll;           //Ease's initial value
    f32 goalRoll;       //Ease's goal value
    f32 fov;            //Ease's initial value
    f32 goalFov;        //Ease's goal value
    Vec4f easeSpline;
    f32 easedDistance;  //Distance travelled so far while easing
    f32 goalDistance;   //Total distance that will be travelled, from the ease's start to end position
    u8 _unk58[0xF4 - 0x58];
    u8 easeInFinished;  //Ease into StaticCamera has finished
    u8 cameraLost;      //No StaticCamera found, swapping back to CamNormal
} CamStatic;

/*0x0*/ static CamStatic* sState;

static void camstatic_setupEase(Cam* cam, Vec3f* staticCamCoords, s32 goalYaw, s32 goalPitch, s32 goalRoll, f32 goalFov);
static s32 camstatic_ease(Cam* cam, u8 flags);
static Object* camstatic_findStaticCamera(f32 x, f32 y, f32 z, s32 cameraID, s32 controlNo);

// offset: 0x0 | ctor
void camstatic_ctor(void* dll) { }

// offset: 0xC | dtor
void camstatic_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
/* CamStatic_Setup? */
void camstatic_func_18(Cam* cam, s32 arg1, CamStatic_Params* data) {
    Object* staticCam;
    StaticCamera_Setup* camSetup;
    f32 dx;
    f32 dy;
    f32 dz;
    Object* player;
    s16 yaw;
    s16 pitch;
    s16 roll;
    f32 fov;

    player = cam->player;

    sState = mmAlloc(sizeof(CamStatic), ALLOC_TAG_CAM_COL, ALLOC_NAME("camstatic"));
    sState->easeInFinished = TRUE;
    sState->cameraLost = FALSE;
    
    staticCam = camstatic_findStaticCamera(player->srt.transl.x, player->srt.transl.y, player->srt.transl.z, data->cameraID, OBJCONTROL_StaticCamera);
    if (staticCam == NULL) {
        sState->cameraLost = TRUE;
        return;
    }

    sState->obj = staticCam;
    camSetup = (StaticCamera_Setup*)staticCam->setup;
    dx = staticCam->globalPosition.x - player->globalPosition.x;
    dy = staticCam->globalPosition.y - player->globalPosition.y;
    dz = staticCam->globalPosition.z - player->globalPosition.z;

    if (camSetup->flags & CamStatic_FLAG_Aim_Yaw_at_Player) {
        yaw = M_180_DEGREES - mathAtan2f(dx, dz);
    } else {
        yaw = camSetup->yaw + M_180_DEGREES;
    }

    if (camSetup->flags & CamStatic_FLAG_Aim_Pitch_at_Player) {
        pitch = (mathAtan2f(dy, sqrtf(SQ(dx) + SQ(dz))) & 0xFFFF & 0xFFFF) - camSetup->pitch;
    } else {
        pitch = camSetup->pitch;
    }

    if (camSetup->flags & CamStatic_FLAG_Use_Player_Roll) {
        roll = player->srt.roll;
    } else {
        roll = camSetup->roll;
    }

    fov = camSetup->fov;

    if (data->previousCameraEasesIn == FALSE) {
        //StaticCamera manages the initial ease in
        camstatic_setupEase(cam, &staticCam->globalPosition, yaw, pitch, roll, fov);
    } else {
        cam->srt.transl.x = staticCam->globalPosition.x;
        cam->srt.transl.y = staticCam->globalPosition.y;
        cam->srt.transl.z = staticCam->globalPosition.z;
        cam->srt.yaw = yaw;
        cam->srt.pitch = pitch;
        cam->srt.roll = roll;
        cam->fov = fov;
    }
}

// offset: 0x278 | func: 1 | export: 1
/* CamStatic_Update? */
void camstatic_func_278(Cam* cam) {
    StaticCamera_Setup* camSetup;
    s32 rollDiff;
    s32 pitchDiff;
    f32 dx;
    f32 dy;
    f32 dz;
    Object* player;
    s32 easeFinished;

    if (sState->cameraLost) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
        return;
    }

    player = cam->player;
    camSetup = (StaticCamera_Setup*)sState->obj->setup;

    if (!(camSetup->flags & CamStatic_FLAG_Aim_Yaw_at_Player)) {
        cam->srt.yaw = camSetup->yaw + M_180_DEGREES;
    }
    if (!(camSetup->flags & CamStatic_FLAG_Aim_Pitch_at_Player)) {
        cam->srt.pitch = camSetup->pitch;
    }
    if (!(camSetup->flags & CamStatic_FLAG_Use_Player_Roll)) {
        cam->srt.roll = camSetup->roll;
    }

    cam->srt.transl.x = sState->obj->globalPosition.x;
    cam->srt.transl.y = sState->obj->globalPosition.y;
    cam->srt.transl.z = sState->obj->globalPosition.z;
    cam->fov = camSetup->fov;

    //Apply easing
    if (sState->easeInFinished == FALSE) {
        easeFinished = camstatic_ease(cam, camSetup->flags);
        if (easeFinished) {
            sState->easeInFinished = TRUE;
        }
    }

    dx = cam->srt.transl.x - player->globalPosition.x;
    dy = cam->srt.transl.y - player->globalPosition.y;
    dz = cam->srt.transl.z - player->globalPosition.z;

    if (camSetup->flags & CamStatic_FLAG_Aim_Yaw_at_Player) {
        cam->srt.yaw = M_180_DEGREES - mathAtan2f(dx, dz);
    }

    if (camSetup->flags & CamStatic_FLAG_Aim_Pitch_at_Player) {
        pitchDiff = (mathAtan2f(dy, sqrtf(SQ(dx) + SQ(dz))) - camSetup->pitch) - (cam->srt.pitch & 0xFFFF);
        CIRCLE_WRAP(pitchDiff);
        cam->srt.pitch += (pitchDiff * gUpdateRate) >> 3;
    }

    if (camSetup->flags & CamStatic_FLAG_Use_Player_Roll) {
        rollDiff = cam->srt.roll - (player->srt.roll & 0xFFFF);
        CIRCLE_WRAP(rollDiff);
        cam->srt.roll += (rollDiff * gUpdateRate) >> 3;
    }
}

// offset: 0x584 | func: 2 | export: 2
/* CamStatic_Free? */
void camstatic_func_584(Cam* cam) {
    mmFree(sState);
}

// offset: 0x5C4 | func: 3 | export: 3
void camstatic_func_5C4(void* arg0, s32 arg1) {

}

// offset: 0x5D4 | func: 4
static void camstatic_setupEase(Cam* cam, Vec3f* staticCamCoords, s32 goalYaw, s32 goalPitch, s32 goalRoll, f32 goalFov) {
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
    sState->goalX = staticCamCoords->x;
    sState->goalY = staticCamCoords->y;
    sState->goalZ = staticCamCoords->z;
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

// offset: 0x798 | func: 5
static s32 camstatic_ease(Cam* cam, u8 flags) {
    f32 tValue;
    f32 speed;

    sState->goalX = cam->srt.transl.x;
    sState->goalY = cam->srt.transl.y;
    sState->goalZ = cam->srt.transl.z;
    sState->goalYaw = cam->srt.yaw;
    sState->goalPitch = cam->srt.pitch;
    sState->goalRoll = cam->srt.roll;
    sState->goalFov = cam->fov;

    tValue = sState->easedDistance / sState->goalDistance;
    if (tValue > 1.0f) {
        tValue = 1.0f;
    }

    speed = curvesHermite(sState->easeSpline.f, tValue, NULL);
    if (speed < 0.2f) {
        speed = 0.2f;
    }

    sState->easedDistance += speed * gUpdateRateF;
    tValue = sState->easedDistance / sState->goalDistance;
    if (tValue > 1.0f) {
        tValue = 1.0f;
    }

    //Ease position
    cam->srt.transl.x = curvesLinear(&sState->x, tValue, NULL);
    cam->srt.transl.y = curvesLinear(&sState->y, tValue, NULL);
    cam->srt.transl.z = curvesLinear(&sState->z, tValue, NULL);

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
    if (!(flags & CamStatic_FLAG_Aim_Yaw_at_Player)) {
        cam->srt.yaw = curvesLinear(&sState->yaw, tValue, NULL);
    }

    if (!(flags & CamStatic_FLAG_Aim_Pitch_at_Player)) {
        cam->srt.pitch = curvesLinear(&sState->pitch, tValue, NULL);
    }

    if (!(flags & CamStatic_FLAG_Use_Player_Roll)) {
        cam->srt.roll = curvesLinear(&sState->roll, tValue, NULL);
    }

    //@bug: FOV doesn't ease

    return tValue >= 1.0f;
}

// offset: 0xC04 | func: 6
/**
  * Attempts to find a StaticCamera object in the game world. 
  *
  * The StaticCamera's `cameraID` setup field must match the ID being searched for.
  */
static Object* camstatic_findStaticCamera(f32 x, f32 y, f32 z, s32 cameraID, s32 controlNo) {
    Object** staticCams;
    s32 numObjs;
    Object* closestStaticCam;
    Object* staticCam;
    f32 zDist;
    f32 xDist;
    f32 dist;
    f32 yDist;
    f32 closestDist;
    s32 i;
    StaticCamera_Setup* staticCamSetup;

    closestStaticCam = NULL;
    closestDist = 100000.0f;
    staticCams = objGetAllOfType(OBJTYPE_StaticCamera, &numObjs);
    for (i = 0; i < numObjs; i++) {
        staticCam = staticCams[i];
        if (controlNo == staticCam->controlNo) {
            staticCamSetup = (StaticCamera_Setup*)staticCam->setup;
            if (cameraID == staticCamSetup->cameraID) {
                xDist = x - staticCam->srt.transl.x;
                yDist = y - staticCam->srt.transl.y;
                zDist = z - staticCam->srt.transl.z;
                dist = sqrtf(SQ(xDist) + SQ(yDist) + SQ(zDist));
                if (dist < closestDist) {
                    closestDist = dist;
                    closestStaticCam = staticCam;
                }
            }
        }
    }

    return closestStaticCam;
}
