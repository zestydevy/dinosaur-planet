#include "dlls/engine/2_camcontrol.h"
#include "dlls/objects/210_player.h"
#include "sys/math.h"

/*0x0*/ static f32 dPlayerOffsetY = 37.0f;

/*0x0*/ static f32 sOrbitDistanceInitial;
/*0x4*/ static f32 sOrbitDistance;

static void camspellaim_calculateIntersectDistance(Cam* cam, Object* player, f32* orbitDistance);

// offset: 0x0 | ctor
void camspellaim_ctor(void* dll) { }

// offset: 0xC | dtor
void camspellaim_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void camspellaim_Setup(Cam* cam, s32 arg1, void* data) {
    camspellaim_calculateIntersectDistance(cam, cam->player, &sOrbitDistanceInitial);
}

// offset: 0x64 | func: 1 | export: 1
void camspellaim_Control(Cam* cam) {
    Object* player;
    f32 pad;
    f32 distanceLateral;
    f32 distanceVertical;
    f32 sinYaw;
    f32 cosYaw;
    f32 cosPitch;
    f32 sinPitch;
    s16 yawSpeed;
    s16 pitchSpeed;
    f32 orbitOrigin[3];

    player = cam->player;

    distanceVertical = sOrbitDistanceInitial;
    cam->highlightFlags |= 2;
    sOrbitDistance = distanceVertical;

    ((DLL_210_Player*)player->dll)->vtbl->func62(player, &yawSpeed, &pitchSpeed);
    yawSpeed = ((yawSpeed >> 1) - player->srt.yaw) + M_180_DEGREES;
    pitchSpeed >>= 1;
    orbitOrigin[2] = player->srt.transl.x;
    orbitOrigin[1] = player->srt.transl.y + dPlayerOffsetY;
    orbitOrigin[0] = player->srt.transl.z;
    
    yawSpeed -= (cam->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(yawSpeed);
    cam->srt.yaw += (yawSpeed * gUpdateRate) >> 3;

    pitchSpeed -= (cam->srt.pitch & 0xFFFF);
    CIRCLE_WRAP(pitchSpeed);
    cam->srt.pitch += (pitchSpeed * gUpdateRate) >> 3;

    sinYaw = mathSinfInterp((cam->srt.yaw - M_90_DEGREES));
    cosYaw = mathCosfInterp((cam->srt.yaw - M_90_DEGREES));
    cosPitch = mathCosfInterp(cam->srt.pitch);
    sinPitch = mathSinfInterp(cam->srt.pitch);

    distanceVertical = sOrbitDistance;
    distanceLateral = distanceVertical * cosPitch;

    cam->srt.transl.x = orbitOrigin[2] + (distanceLateral * cosYaw);
    cam->srt.transl.y = orbitOrigin[1] + (distanceVertical * sinPitch);
    cam->srt.transl.z = orbitOrigin[0] + (distanceLateral * sinYaw);
}

// offset: 0x2C8 | func: 2 | export: 2
void camspellaim_Free(Cam* cam) {

}

// offset: 0x2D4 | func: 3 | export: 3
void camspellaim_Func_2D4(void* arg0, s32 arg1) {

}

// offset: 0x2E4 | func: 4
static void camspellaim_calculateIntersectDistance(Cam* cam, Object* player, f32* distance) {
    f32 sin;
    f32 cos;
    f32 dx;
    f32 dz;
    Vec3f initial;
    Vec3f goal;
    AABBs32 aabb;
    TrackIntersectResult result;

    sin = mathSinfInterp(player->srt.yaw);
    cos = mathCosfInterp(player->srt.yaw);
    goal.x = player->globalPosition.x + (sin * 60.0f);
    goal.y = player->globalPosition.y + 37.0f;
    goal.z = player->globalPosition.z + (cos * 60.0f);
    initial.x = player->globalPosition.x;
    initial.y = goal.y;
    initial.z = player->globalPosition.z;
    result.unk50[0] = -1;
    result.unk54[0] = 4;
    result.unk40[0] = 4.5f;
    trackIntersectBuildAABB(&aabb, &initial, &goal, &result.unk40[0], 1);
    trackIntersectBroadphase(player, &aabb, 1);
    if (trackGetIntersect(player, initial.f, goal.f, 1, &result, 0)) {
        dx = goal.x - initial.x;
        dz = goal.z - initial.z;
        *distance = sqrtf(SQ(dx) + SQ(dz));
    } else {
        *distance = 60.0f;
    }
}
