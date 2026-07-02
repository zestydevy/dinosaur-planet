#include "dlls/engine/2_camcontrol.h"
#include "sys/memory.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} CamCloudRunner_Params;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} BSS0;

/*0x0*/ static BSS0* bss_0;

// offset: 0x0 | ctor
void dll_101_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_101_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_101_func_18(CamControl_Data* camData, s32 arg1, CamCloudRunner_Params* action) {
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

    sp20 = camData->player;
    bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, NULL);
    if (action != NULL) {
        bss_0->unk0 = action->unk0;
        bss_0->unk4 = (f32) action->unk4;
        bss_0->unk8 = (f32) action->unk8;
        var_fv0 = action->unkC;
    } else {
        bss_0->unk0 = sp20->srt.transl.x;
        bss_0->unk4 = sp20->srt.transl.y;
        bss_0->unk8 = sp20->srt.transl.z;
        var_fv0 = 150.0f;
    }
    bss_0->unkC = var_fv0;
    v1 = camData->srt.transl.x - bss_0->unk0;
    v2 = camData->srt.transl.z - bss_0->unk8;
    sp2C = arctan2_f(v1, v2);
    v1 = camData->player->srt.transl.x - bss_0->unk0;
    v2 = camData->player->srt.transl.z - bss_0->unk8;
    var_v1 = arctan2_f(v1, v2) - (sp2C & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    if (var_v1){}
}

// offset: 0x17C | func: 1 | export: 1
void dll_101_func_17C(CamControl_Data* camData) {
    f32 temp_fs0;
    f32 sp30;
    f32 sp2C;
    Object* sp28;
    f32 temp_fv0;

    sp28 = camData->player;
    if (sp28 != NULL) {
        temp_fs0 = fsin16_precise(sp28->srt.yaw);
        temp_fv0 = fcos16_precise(sp28->srt.yaw);
        camData->srt.transl.x = sp28->srt.transl.x + (temp_fs0 * bss_0->unkC);
        camData->srt.transl.y = sp28->srt.transl.y + 25.0f;
        camData->srt.transl.z = sp28->srt.transl.z + (temp_fv0 * bss_0->unkC);
        temp_fs0 = camData->srt.transl.x - sp28->srt.transl.x;
        sp2C = camData->srt.transl.y - sp28->srt.transl.y;
        sp30 = camData->srt.transl.z - sp28->srt.transl.z;
        camData->srt.yaw = 0x8000 - arctan2_f(temp_fs0, sp30);
        camData->srt.pitch = arctan2_f(sp2C, sqrtf(SQ(temp_fs0) + SQ(sp30)));
    }
}

// offset: 0x2C4 | func: 2 | export: 2
void dll_101_func_2C4(CamControl_Data* camData) {
    mmFree(bss_0);
}

// offset: 0x304 | func: 3 | export: 3
void dll_101_func_304(CamCloudRunner_Params* arg0, s32 arg1) {

}
