#include "dlls/engine/84_camnormal.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "dll.h"

// Active camera while climbing a wall/ladder

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    s8 unk4;
    s8 unk5;
} CamClimb_Params;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    u16 unk30;
    u16 unk32;
    s16 unk34;
} BSS0;

/*0x0*/ static BSS0* bss_0;

static void dll_93_func_63C(CamControl_Data* arg0);

// offset: 0x0 | ctor
void dll_93_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_93_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_93_func_18(CamControl_Data* camData, s32 arg1, CamClimb_Params* action) {
    s32 _pad;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    s32 _pad2;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    CamControl_Module* camnormal;

    if (bss_0 == NULL) {
        bss_0 = mmAlloc(sizeof(BSS0), ALLOC_TAG_CAM_COL, ALLOC_NAME("camclimb"));
    }
    if ((arg1 != 1) && (arg1 == 2)) {
        bss_0->unk32 = bss_0->unk30;
        bss_0->unk1C = bss_0->unkC;
        bss_0->unk24 = bss_0->unk10;
        bss_0->unk14 = bss_0->unk0;
        bss_0->unk34 = (s16) ((f32) action->unk3 * 182.04f);
        bss_0->unk20 = (f32) action->unk5;
        bss_0->unk28 = (f32) action->unk4;
        bss_0->unk18 = (f32) action->unk2;
        bss_0->unk2C = action->unk1;
        bss_0->unk2E = action->unk1;
        return;
    }
    bzero(bss_0, sizeof(BSS0));
    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func7(&sp4C, &sp48, &sp44, &sp40, &sp3C);
    gDLL_2_Camera->vtbl->get_player_to_camera_distances((Camera* ) camData, &sp60, &sp5C, &sp58, &sp54, (f32) bss_0->unk30);
    bss_0->unk32 = (u16) sp3C;
    bss_0->unk1C = sp44;
    bss_0->unk24 = sp40;
    bss_0->unk14 = sp54;
    bss_0->unk34 = 0x71C;
    bss_0->unk20 = 0.0f;
    bss_0->unk28 = 0.0f;
    bss_0->unk18 = (f32) ((sp48 + sp4C) * 0.5f);
    bss_0->unk2C = 0x3C;
    bss_0->unk2E = 0x3C;
    bss_0->unk4 = sp54;
    bss_0->unk8 = 0.05f;
}

// offset: 0x340 | func: 1 | export: 1
void dll_93_func_340(CamControl_Data* camData) {
    Object* temp_s0;
    s32 var_v1;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 temp_fa0;
    f32 temp_fa1;

    temp_s0 = camData->player;
    dll_93_func_63C(camData);
    temp_fa0 = temp_s0->srt.transl.y + bss_0->unk10;
    temp_fa1 = temp_s0->srt.transl.y + bss_0->unkC;
    if (camData->srt.transl.y < temp_fa1) {
        sp50 = temp_fa1 - camData->srt.transl.y;
    } else if (temp_fa0 < camData->srt.transl.y) {
        sp50 = temp_fa0 - camData->srt.transl.y;
    } else {
        sp50 = 0.0f;
    }
    sp50 *= (bss_0->unk8 * gUpdateRateF);
    camData->srt.transl.y += sp50;
    sp48 = bss_0->unk0;
    sp48 -= bss_0->unk4;
    sp48 *= (0.05f * gUpdateRateF);
    bss_0->unk4 += sp48;
    camData->srt.transl.x = (fsin16_precise(temp_s0->srt.yaw) * bss_0->unk4) + temp_s0->srt.transl.x;
    camData->srt.transl.z = (fcos16_precise(temp_s0->srt.yaw) * bss_0->unk4) + temp_s0->srt.transl.z;
    gDLL_2_Camera->vtbl->get_player_to_camera_distances((Camera* ) camData, &sp54, &sp50, &sp4C, &sp48, 0.0f);
    
    var_v1 = (-arctan2_f(sp54, sp4C) - (camData->srt.yaw & 0xFFFF));
    var_v1 += 0x8000;
    CIRCLE_WRAP(var_v1);
    camData->srt.yaw += var_v1;
    
    var_v1 = (arctan2_f(sp50, sp48) - bss_0->unk30) - (camData->srt.pitch & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    camData->srt.pitch += ((s32) (var_v1 * gUpdateRate) / 6);
}

// offset: 0x5E8 | func: 2 | export: 2
void dll_93_func_5E8(CamControl_Data* camData) {
    mmFree(bss_0);
    bss_0 = NULL;
}

// offset: 0x62C | func: 3 | export: 3
void dll_93_func_62C(CamClimb_Params* arg0, s32 arg1) {

}

// offset: 0x63C | func: 4
static void dll_93_func_63C(CamControl_Data* arg0) {
    f32 temp_fv0;

    if (bss_0->unk2C != 0) {
        bss_0->unk2C -= gUpdateRate;
        if (bss_0->unk2C < 0) {
            bss_0->unk2C = 0;
        }
        temp_fv0 = (f32) (bss_0->unk2E - bss_0->unk2C) / (f32) bss_0->unk2E;
        bss_0->unk30 = (u16) ((f32) bss_0->unk32 + ((f32) ((u16) bss_0->unk34 - bss_0->unk32) * temp_fv0));
        bss_0->unk0 = bss_0->unk14 + ((bss_0->unk18 - bss_0->unk14) * temp_fv0);
        bss_0->unkC = bss_0->unk1C + ((bss_0->unk20 - bss_0->unk1C) * temp_fv0);
        bss_0->unk10 = bss_0->unk24 + ((bss_0->unk28 - bss_0->unk24) * temp_fv0);
    }
}
