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
} CamClimb;

/*0x0*/ static CamClimb* sState;

static void dll_93_func_63C(Cam* cam);

// offset: 0x0 | ctor
void dll_93_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_93_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_93_func_18(Cam* cam, s32 arg1, CamClimb_Params* data) {
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

    if (sState == NULL) {
        sState = mmAlloc(sizeof(CamClimb), ALLOC_TAG_CAM_COL, ALLOC_NAME("camclimb"));
    }
    if ((arg1 != 1) && (arg1 == 2)) {
        sState->unk32 = sState->unk30;
        sState->unk1C = sState->unkC;
        sState->unk24 = sState->unk10;
        sState->unk14 = sState->unk0;
        sState->unk34 = (s16) ((f32) data->unk3 * 182.04f);
        sState->unk20 = (f32) data->unk5;
        sState->unk28 = (f32) data->unk4;
        sState->unk18 = (f32) data->unk2;
        sState->unk2C = data->unk1;
        sState->unk2E = data->unk1;
        return;
    }
    bzero(sState, sizeof(CamClimb));
    camnormal = gDLL_2_Camera->vtbl->get_camnormal_module();
    ((DLL_84_camnormal*)camnormal->dll)->vtbl->func7(&sp4C, &sp48, &sp44, &sp40, &sp3C);
    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp60, &sp5C, &sp58, &sp54, (f32) sState->unk30);
    sState->unk32 = (u16) sp3C;
    sState->unk1C = sp44;
    sState->unk24 = sp40;
    sState->unk14 = sp54;
    sState->unk34 = 0x71C;
    sState->unk20 = 0.0f;
    sState->unk28 = 0.0f;
    sState->unk18 = (f32) ((sp48 + sp4C) * 0.5f);
    sState->unk2C = 0x3C;
    sState->unk2E = 0x3C;
    sState->unk4 = sp54;
    sState->unk8 = 0.05f;
}

// offset: 0x340 | func: 1 | export: 1
void dll_93_func_340(Cam* cam) {
    Object* temp_s0;
    s32 var_v1;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 temp_fa0;
    f32 temp_fa1;

    temp_s0 = cam->player;
    dll_93_func_63C(cam);
    temp_fa0 = temp_s0->srt.transl.y + sState->unk10;
    temp_fa1 = temp_s0->srt.transl.y + sState->unkC;
    if (cam->srt.transl.y < temp_fa1) {
        sp50 = temp_fa1 - cam->srt.transl.y;
    } else if (temp_fa0 < cam->srt.transl.y) {
        sp50 = temp_fa0 - cam->srt.transl.y;
    } else {
        sp50 = 0.0f;
    }
    sp50 *= (sState->unk8 * gUpdateRateF);
    cam->srt.transl.y += sp50;
    sp48 = sState->unk0;
    sp48 -= sState->unk4;
    sp48 *= (0.05f * gUpdateRateF);
    sState->unk4 += sp48;
    cam->srt.transl.x = (fsin16_precise(temp_s0->srt.yaw) * sState->unk4) + temp_s0->srt.transl.x;
    cam->srt.transl.z = (fcos16_precise(temp_s0->srt.yaw) * sState->unk4) + temp_s0->srt.transl.z;
    gDLL_2_Camera->vtbl->get_player_to_camera_distances(cam, &sp54, &sp50, &sp4C, &sp48, 0.0f);
    
    var_v1 = (-arctan2_f(sp54, sp4C) - (cam->srt.yaw & 0xFFFF));
    var_v1 += 0x8000;
    CIRCLE_WRAP(var_v1);
    cam->srt.yaw += var_v1;
    
    var_v1 = (arctan2_f(sp50, sp48) - sState->unk30) - (cam->srt.pitch & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    cam->srt.pitch += ((s32) (var_v1 * gUpdateRate) / 6);
}

// offset: 0x5E8 | func: 2 | export: 2
void dll_93_func_5E8(Cam* cam) {
    mmFree(sState);
    sState = NULL;
}

// offset: 0x62C | func: 3 | export: 3
void dll_93_func_62C(void* arg0, s32 arg1) {

}

// offset: 0x63C | func: 4
static void dll_93_func_63C(Cam* cam) {
    f32 temp_fv0;

    if (sState->unk2C != 0) {
        sState->unk2C -= gUpdateRate;
        if (sState->unk2C < 0) {
            sState->unk2C = 0;
        }
        temp_fv0 = (f32) (sState->unk2E - sState->unk2C) / (f32) sState->unk2E;
        sState->unk30 = (u16) ((f32) sState->unk32 + ((f32) ((u16) sState->unk34 - sState->unk32) * temp_fv0));
        sState->unk0 = sState->unk14 + ((sState->unk18 - sState->unk14) * temp_fv0);
        sState->unkC = sState->unk1C + ((sState->unk20 - sState->unk1C) * temp_fv0);
        sState->unk10 = sState->unk24 + ((sState->unk28 - sState->unk24) * temp_fv0);
    }
}
