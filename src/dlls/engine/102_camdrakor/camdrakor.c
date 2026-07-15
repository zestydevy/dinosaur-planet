#include "dlls/engine/2_camcontrol.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "dll.h"

typedef struct {
    Object* unk0;
    Object* unk4;
    Object* unk8;
    u8 unkC;
    u8 unkD;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
} CamDrakor;

/*0x0*/ static CamDrakor* sState;

// offset: 0x0 | ctor
void camdrakor_ctor(void* dll) { }

// offset: 0xC | dtor
void camdrakor_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void camdrakor_func_18(Cam* cam, s32 arg1, Cam* data) {
    sState = mmAlloc(sizeof(CamDrakor), ALLOC_TAG_CAM_COL, ALLOC_NAME("camdrakor"));
    bzero(sState, sizeof(CamDrakor));
    sState->unk10 = 60.0f;
    sState->unkC = 0;
    if (data != NULL) {
        cam->srt.transl.x = data->initialCoords.x;
        cam->srt.transl.y = data->initialCoords.y;
        cam->srt.transl.z = data->initialCoords.z;
        cam->srt.yaw = data->srt.yaw;
        cam->srt.pitch = data->srt.pitch;
        cam->srt.roll = data->srt.roll;
        cam->fov = data->fov;
    }
    sState->unk14 = cam->srt.transl.x;
    sState->unk18 = cam->srt.transl.y;
    sState->unk1C = cam->srt.transl.z;
    sState->unk20 = cam->srt.yaw;
    sState->unk22 = cam->srt.pitch;
    sState->unk24 = cam->srt.roll;
}

// offset: 0x138 | func: 1 | export: 1
void camdrakor_func_138(Cam* cam) {
    Object** temp_v0;
    s32 sp78;
    s32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 temp_fa1_3;
    f32 temp_ft4_2;
    f32 temp_ft5_2;
    f32 temp_fv1_3;
    s16 var_v0;

    if (sState->unkD != 0) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
        return;
    }
    if (sState->unk4 == NULL) {
        temp_v0 = objGetObjects(&sp78, &sp74);
        while (sp78 < sp74) {
            if (temp_v0[sp78]->id == OBJ_BossDrakor) {
                sState->unk4 = temp_v0[sp78];
            } else if (temp_v0[sp78]->id == OBJ_BossDrakorFlatR) {
                sState->unk0 = temp_v0[sp78];
            }
            sp78++;
        }
    }
    if (sState->unk8 == NULL) {
        sState->unk8 = objGetPlayer();
    }
    sp70 = sState->unk4->globalPosition.x - sState->unk0->globalPosition.x;
    sp6C = sState->unk4->globalPosition.y - sState->unk0->globalPosition.y;
    sp68 = sState->unk4->globalPosition.z - sState->unk0->globalPosition.z;
    sp5C = sqrtf(SQ(sp70) + SQ(sp6C) + SQ(sp68));
    sp64 = sp70 / sp5C;
    sp60 = sp68 / sp5C;
    temp_ft4_2 = (sState->unk0->globalPosition.x - (sp64 * 140.0f)) - sState->unk8->globalPosition.x;
    temp_ft5_2 = (sState->unk0->globalPosition.z - (sp60 * 140.0f)) - sState->unk8->globalPosition.z;
    temp_fv1_3 = (200.0f - sqrtf(SQ(temp_ft4_2) + SQ(temp_ft5_2))) / 200.0f;
    cam->fov = (temp_fv1_3 * 70.0f) + 45.0f;
    cam->srt.transl.x = sState->unk0->srt.transl.x - (sp64 * (-30.0f + (temp_fv1_3 * 350.0f)));
    cam->srt.transl.y = sState->unk0->srt.transl.y + 20.0f + (temp_fv1_3 * 60.0f);
    cam->srt.transl.z = sState->unk0->srt.transl.z - (sp60 * (-30.0f + (temp_fv1_3 * 350.0f)));
    cam->srt.yaw = -mathAtan2f(sp70, sp68);
    cam->srt.pitch = -mathAtan2f(sp6C - ((sp5C / 400.0f) * 100.0f), sqrtf(SQ(sp70) + SQ(sp68)));
    if (sState->unkC == 0) {
        temp_fa1_3 = sState->unk10 / 60.0f;
        cam->srt.transl.x += ((sState->unk14 - cam->srt.transl.x) * temp_fa1_3);
        cam->srt.transl.y += ((sState->unk18 - cam->srt.transl.y) * temp_fa1_3);
        cam->srt.transl.z += ((sState->unk1C - cam->srt.transl.z) * temp_fa1_3);
        var_v0 = sState->unk20 - (cam->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(var_v0);
        cam->srt.yaw += ((f32) var_v0 * temp_fa1_3);
        var_v0 = sState->unk22 - (cam->srt.pitch & 0xFFFF);
        CIRCLE_WRAP(var_v0);
        cam->srt.pitch += ((f32) var_v0 * temp_fa1_3);
        sState->unk10 -= gUpdateRateF;
        if (sState->unk10 < 0.0f) {
            sState->unkC = 1;
            sState->unk10 = 0.0f;
        }
    }
}

// offset: 0x61C | func: 2 | export: 2
void camdrakor_func_61C(Cam* cam) {
    mmFree(sState);
}

// offset: 0x65C | func: 3 | export: 3
void camdrakor_func_65C(void* arg0, s32 arg1) {

}
