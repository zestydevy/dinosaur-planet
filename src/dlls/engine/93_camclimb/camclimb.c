#include "dlls/engine/84_camnormal.h"
#include "dlls/engine/93_camclimb.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "dll.h"

// Active camera while climbing a wall/ladder

typedef struct {
    f32 desiredDistance;    //Desired lateral distance from the player (eased value)
    f32 distance;           //Camera's lateral distance from the player (actual current value)
    f32 speedY;             //Rate of change of y offset
    f32 minY;               //Lower bound for camera's y difference relative to the player
    f32 maxY;               //Upper bound for camera's y difference relative to the player
    f32 distanceInitial;    //For easing: y difference relative to the player (start)
    f32 distanceGoal;       //For easing: y difference relative to the player (end)
    f32 minYInitial;        //For easing: y offset lower bound (start)
    f32 minYGoal;           //For easing: y offset lower bound (end)
    f32 maxYInitial;        //For easing: y offset upper bound (start)
    f32 maxYGoal;           //For easing: y offset upper bound (end)
    s16 easeTimer;          //Decrementing ease timer
    s16 easeDuration;       //Length of ease in
    u16 pitchOffset;        //Additive camera pitch value
    u16 pitchOffsetInitial; //For easing: pitch offset (start)
    s16 pitchOffsetGoal;    //For easing: pitch offset (end)
} CamClimb;

/*0x0*/ static CamClimb* sState;

static void camclimb_ease(Cam* cam);

// offset: 0x0 | ctor
void camclimb_ctor(void* dll) { }

// offset: 0xC | dtor
void camclimb_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void camclimb_setup(Cam* cam, s32 mode, CamClimb_Params* data) {
    s32 _pad;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distance;
    s32 _pad2;
    f32 distanceMin;
    f32 distanceMax;
    f32 minY;
    f32 maxY;
    f32 currentY;
    CamControl_Module* camnormal;

    if (sState == NULL) {
        sState = mmAlloc(sizeof(CamClimb), ALLOC_TAG_CAM_COL, ALLOC_NAME("camclimb"));
    }
    
    if ((mode != 1) && (mode == 2)) {
        //Cut to camera
        sState->pitchOffsetInitial = sState->pitchOffset;
        sState->minYInitial = sState->minY;
        sState->maxYInitial = sState->maxY;
        sState->distanceInitial = sState->desiredDistance;
        sState->pitchOffsetGoal = data->pitchOffset * M_1_DEGREE_F;
        sState->minYGoal = data->minY;
        sState->maxYGoal = data->maxY;
        sState->distanceGoal = data->distance;
        sState->easeTimer = data->easeDuration;
        sState->easeDuration = data->easeDuration;
        return;
    }
    bzero(sState, sizeof(CamClimb));

    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func7(&distanceMin, &distanceMax, &minY, &maxY, &currentY);
    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &dx, &dy, &dz, &distance, sState->pitchOffset);

    sState->pitchOffsetInitial = currentY;
    sState->minYInitial = minY;
    sState->maxYInitial = maxY;
    sState->distanceInitial = distance;
    sState->pitchOffsetGoal = M_10_DEGREES;
    sState->minYGoal = 0.0f;
    sState->maxYGoal = 0.0f;
    sState->distanceGoal = (distanceMax + distanceMin) * 0.5f;
    sState->easeTimer = 60;
    sState->easeDuration = 60;
    sState->distance = distance;
    sState->speedY = 0.05f;
}

// offset: 0x340 | func: 1 | export: 1
void camclimb_control(Cam* cam) {
    Object* player;
    s32 angleDiff;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distance;
    f32 maxY;
    f32 minY;

    player = cam->player;

    camclimb_ease(cam);

    //Set camera Y (eased)
    {
        maxY = player->srt.transl.y + sState->maxY;
        minY = player->srt.transl.y + sState->minY;

        if (minY > cam->srt.transl.y) {
            dy = minY - cam->srt.transl.y;
        } else if (maxY < cam->srt.transl.y) {
            dy = maxY - cam->srt.transl.y;
        } else {
            dy = 0.0f;
        }
        dy *= (sState->speedY * gUpdateRateF);
        cam->srt.transl.y += dy;
    }

    //Set camera X and Z from player yaw and desired camera distance
    {
        distance = sState->desiredDistance;
        distance -= sState->distance;
        distance *= (0.05f * gUpdateRateF);
        sState->distance += distance;

        cam->srt.transl.x = (mathSinfInterp(player->srt.yaw) * sState->distance) + player->srt.transl.x;
        cam->srt.transl.z = (mathCosfInterp(player->srt.yaw) * sState->distance) + player->srt.transl.z;
    }

    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &dx, &dy, &dz, &distance, 0.0f);
    
    //Set yaw to aim at player
    angleDiff = -mathAtan2f(dx, dz) - (cam->srt.yaw & 0xFFFF);
    angleDiff += M_180_DEGREES;
    CIRCLE_WRAP(angleDiff);
    cam->srt.yaw += angleDiff;
    
    //Set pitch (eased)
    angleDiff = (mathAtan2f(dy, distance) - sState->pitchOffset) - (cam->srt.pitch & 0xFFFF);
    CIRCLE_WRAP(angleDiff);
    cam->srt.pitch += (s32) (angleDiff * gUpdateRate) / 6;
}

// offset: 0x5E8 | func: 2 | export: 2
void camclimb_free(Cam* cam) {
    mmFree(sState);
    sState = NULL;
}

// offset: 0x62C | func: 3 | export: 3
void camclimb_func_62C(void* arg0, s32 arg1) {

}

// offset: 0x63C | func: 4
static void camclimb_ease(Cam* cam) {
    f32 tValue;

    if (sState->easeTimer == 0) {
        return;
    }

    sState->easeTimer -= gUpdateRate;
    if (sState->easeTimer < 0) {
        sState->easeTimer = 0;
    }
    tValue = (f32) (sState->easeDuration - sState->easeTimer) / sState->easeDuration;

    sState->pitchOffset = sState->pitchOffsetInitial + (((u16) sState->pitchOffsetGoal - sState->pitchOffsetInitial) * tValue);
    sState->desiredDistance = sState->distanceInitial + (sState->distanceGoal - sState->distanceInitial) * tValue;
    sState->minY = sState->minYInitial + (sState->minYGoal - sState->minYInitial) * tValue;
    sState->maxY = sState->maxYInitial + (sState->maxYGoal - sState->maxYInitial) * tValue;
}
