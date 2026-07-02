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
} BSS0;

/*0x0*/ static BSS0* bss_0;

// offset: 0x0 | ctor
void dll_102_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_102_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_102_func_18(CamControl_Data* camData, s32 arg1, CamControl_Data* action) {
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, NULL);
    bzero(bss_0, sizeof(BSS0));
    bss_0->unk10 = 60.0f;
    bss_0->unkC = 0;
    if (action != NULL) {
        camData->srt.transl.x = action->initialCoords.x;
        camData->srt.transl.y = action->initialCoords.y;
        camData->srt.transl.z = action->initialCoords.z;
        camData->srt.yaw = action->srt.yaw;
        camData->srt.pitch = action->srt.pitch;
        camData->srt.roll = action->srt.roll;
        camData->fov = action->fov;
    }
    bss_0->unk14 = camData->srt.transl.x;
    bss_0->unk18 = camData->srt.transl.y;
    bss_0->unk1C = camData->srt.transl.z;
    bss_0->unk20 = camData->srt.yaw;
    bss_0->unk22 = camData->srt.pitch;
    bss_0->unk24 = camData->srt.roll;
}

// offset: 0x138 | func: 1 | export: 1
void dll_102_func_138(CamControl_Data* camData) {
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

    if (bss_0->unkD != 0) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, 0, 1, 0, NULL, 0, 0xFF);
        return;
    }
    if (bss_0->unk4 == NULL) {
        temp_v0 = get_world_objects(&sp78, &sp74);
        while (sp78 < sp74) {
            if (temp_v0[sp78]->id == OBJ_BossDrakor) {
                bss_0->unk4 = temp_v0[sp78];
            } else if (temp_v0[sp78]->id == OBJ_BossDrakorFlatR) {
                bss_0->unk0 = temp_v0[sp78];
            }
            sp78++;
        }
    }
    if (bss_0->unk8 == NULL) {
        bss_0->unk8 = get_player();
    }
    sp70 = bss_0->unk4->globalPosition.x - bss_0->unk0->globalPosition.x;
    sp6C = bss_0->unk4->globalPosition.y - bss_0->unk0->globalPosition.y;
    sp68 = bss_0->unk4->globalPosition.z - bss_0->unk0->globalPosition.z;
    sp5C = sqrtf(SQ(sp70) + SQ(sp6C) + SQ(sp68));
    sp64 = sp70 / sp5C;
    sp60 = sp68 / sp5C;
    temp_ft4_2 = (bss_0->unk0->globalPosition.x - (sp64 * 140.0f)) - bss_0->unk8->globalPosition.x;
    temp_ft5_2 = (bss_0->unk0->globalPosition.z - (sp60 * 140.0f)) - bss_0->unk8->globalPosition.z;
    temp_fv1_3 = (200.0f - sqrtf(SQ(temp_ft4_2) + SQ(temp_ft5_2))) / 200.0f;
    camData->fov = (temp_fv1_3 * 70.0f) + 45.0f;
    camData->srt.transl.x = bss_0->unk0->srt.transl.x - (sp64 * (-30.0f + (temp_fv1_3 * 350.0f)));
    camData->srt.transl.y = bss_0->unk0->srt.transl.y + 20.0f + (temp_fv1_3 * 60.0f);
    camData->srt.transl.z = bss_0->unk0->srt.transl.z - (sp60 * (-30.0f + (temp_fv1_3 * 350.0f)));
    camData->srt.yaw = -arctan2_f(sp70, sp68);
    camData->srt.pitch = -arctan2_f(sp6C - ((sp5C / 400.0f) * 100.0f), sqrtf(SQ(sp70) + SQ(sp68)));
    if (bss_0->unkC == 0) {
        temp_fa1_3 = bss_0->unk10 / 60.0f;
        camData->srt.transl.x += ((bss_0->unk14 - camData->srt.transl.x) * temp_fa1_3);
        camData->srt.transl.y += ((bss_0->unk18 - camData->srt.transl.y) * temp_fa1_3);
        camData->srt.transl.z += ((bss_0->unk1C - camData->srt.transl.z) * temp_fa1_3);
        var_v0 = bss_0->unk20 - (camData->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(var_v0);
        camData->srt.yaw += ((f32) var_v0 * temp_fa1_3);
        var_v0 = bss_0->unk22 - (camData->srt.pitch & 0xFFFF);
        CIRCLE_WRAP(var_v0);
        camData->srt.pitch += ((f32) var_v0 * temp_fa1_3);
        bss_0->unk10 -= gUpdateRateF;
        if (bss_0->unk10 < 0.0f) {
            bss_0->unkC = 1;
            bss_0->unk10 = 0.0f;
        }
    }
}

// offset: 0x61C | func: 2 | export: 2
void dll_102_func_61C(CamControl_Data* camData) {
    mmFree(bss_0);
}

// offset: 0x65C | func: 3 | export: 3
void dll_102_func_65C(void* arg0, s32 arg1) {

}
