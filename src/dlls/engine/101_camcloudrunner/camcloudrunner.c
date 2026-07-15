#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/101_camcloudrunner.h"
#include "sys/memory.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} CamCloudRunner;

/*0x0*/ static CamCloudRunner* sState;

// offset: 0x0 | ctor
void camcloudrunner_ctor(void* dll) { }

// offset: 0xC | dtor
void camcloudrunner_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void camcloudrunner_func_18(Cam* cam, s32 arg1, CamCloudRunner_Params* data) {
    s32 _pad;
    s32 _pad2;
    s32 _pad3;
    s32 _pad4;
    f32 v1;
    f32 v2;
    s32 sp2C;
    f32 var_fv0;
    s32 var_v1;
    Object* sp20;

    sp20 = cam->player;
    sState = mmAlloc(sizeof(CamCloudRunner), ALLOC_TAG_CAM_COL, NULL);
    if (data != NULL) {
        sState->unk0 = data->unk0;
        sState->unk4 = (f32) data->unk4;
        sState->unk8 = (f32) data->unk8;
        var_fv0 = data->unkC;
    } else {
        sState->unk0 = sp20->srt.transl.x;
        sState->unk4 = sp20->srt.transl.y;
        sState->unk8 = sp20->srt.transl.z;
        var_fv0 = 150.0f;
    }
    sState->unkC = var_fv0;
    v1 = cam->srt.transl.x - sState->unk0;
    v2 = cam->srt.transl.z - sState->unk8;
    sp2C = mathAtan2f(v1, v2);
    v1 = cam->player->srt.transl.x - sState->unk0;
    v2 = cam->player->srt.transl.z - sState->unk8;
    var_v1 = mathAtan2f(v1, v2) - (sp2C & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    if (var_v1){}
}

// offset: 0x17C | func: 1 | export: 1
void camcloudrunner_func_17C(Cam* cam) {
    f32 temp_fs0;
    f32 sp30;
    f32 sp2C;
    Object* sp28;
    f32 temp_fv0;

    sp28 = cam->player;
    if (sp28 != NULL) {
        temp_fs0 = mathSinfInterp(sp28->srt.yaw);
        temp_fv0 = mathCosfInterp(sp28->srt.yaw);
        cam->srt.transl.x = sp28->srt.transl.x + (temp_fs0 * sState->unkC);
        cam->srt.transl.y = sp28->srt.transl.y + 25.0f;
        cam->srt.transl.z = sp28->srt.transl.z + (temp_fv0 * sState->unkC);
        temp_fs0 = cam->srt.transl.x - sp28->srt.transl.x;
        sp2C = cam->srt.transl.y - sp28->srt.transl.y;
        sp30 = cam->srt.transl.z - sp28->srt.transl.z;
        cam->srt.yaw = 0x8000 - mathAtan2f(temp_fs0, sp30);
        cam->srt.pitch = mathAtan2f(sp2C, sqrtf(SQ(temp_fs0) + SQ(sp30)));
    }
}

// offset: 0x2C4 | func: 2 | export: 2
void camcloudrunner_func_2C4(Cam* cam) {
    mmFree(sState);
}

// offset: 0x304 | func: 3 | export: 3
void camcloudrunner_func_304(void* arg0, s32 arg1) {

}
