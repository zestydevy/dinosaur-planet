#include "dlls/engine/2_camcontrol.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"

// TODO: figure out what this camera is/when it's used

typedef struct {
    u8 _unk0[0x4 - 0x0];
    f32 tValue;
} UnkCam;

/*0x0*/ static UnkCam* sState;

// offset: 0x0 | ctor
void camunk_ctor(void* dll) { }

// offset: 0xC | dtor
void camunk_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void camunk_func_18(Cam* cam, s32 arg1, void* data) {
    sState = mmAlloc(sizeof(UnkCam), ALLOC_TAG_CAM_COL, NULL);
    sState->tValue = 0.0f;
}

// offset: 0x74 | func: 1 | export: 1
void camunk_func_74(Cam* cam) {
    Object* player;
    f32 cos;
    f32 pad_sp44;
    s32 pad_sp40;
    f32 sin;
    f32 easeVal;
    f32 spline[4];
    s16 angle;

    spline[0] = 0.0f;
    spline[2] = 0.0f;
    spline[3] = 0.0f;
    spline[1] = 1.0f;
    easeVal = curvesHermite(spline, sState->tValue, NULL);

    player = cam->player;
    angle = (M_180_DEGREES - player->srt.yaw);
    angle += (s32) ((M_20_DEGREES * 4) * easeVal);
    cos = mathCosfInterp(angle);
    sin = mathSinfInterp(angle);

    cam->srt.transl.x = player->srt.transl.x + ((20.0f * cos) - (-10.0f * sin));
    cam->srt.transl.z = player->srt.transl.z + ((20.0f * sin) + (-10.0f * cos));
    cam->srt.transl.y = (player->srt.transl.y + 35.0f) - (easeVal * 15.0f);
    cam->srt.pitch = (5 * M_5_DEGREES) - (s32) (easeVal * 182.0f * 35.0f);

    // FAKE
    if (1){}

    cam->srt.yaw = angle + (M_45_DEGREES - 2);
    cam->srt.roll = 0;
    cam->letterboxGoal = 0;
    cam->fov = 60.0f;
    
    sState->tValue += 0.005f * gUpdateRateF;
    if (sState->tValue > 1.0f) {
        sState->tValue = 1.0f;
    }
}

// offset: 0x250 | func: 2 | export: 2
void camunk_func_250(Cam* cam) {
    mmFree(sState);
}

// offset: 0x290 | func: 3 | export: 3
void camunk_func_290(void* arg0, s32 arg1) {

}
