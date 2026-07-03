#include "dlls/engine/2_camcontrol.h"
#include "dlls/engine/95_camtalk.h"
#include "sys/memory.h"
#include "sys/rand.h"
#include "dll.h"
#include "macros.h"

// Active camera during talk sequences

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 _unkC[0x10 - 0xC];
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s32 unk1C;
} CamTalk;

/*0x0*/ static CamTalk* sState;

// offset: 0x0 | ctor
void dll_95_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_95_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_95_func_18(Cam* cam, s32 arg1, CamTalk_Params* data) {
    s32 _pad;
    s32 _pad2;
    s32 var_v1;
    Object* temp_v0_2;
    Object* temp_v1_2;
    s32 temp_v0_3;
    s32 var_a0;
    s32 sp30;
    f32 xDiff;
    f32 zDiff;
    ObjectStruct74* temp;
    s32 ctype;

    ctype = 0;
    sState = mmAlloc(sizeof(CamTalk), ALLOC_TAG_CAM_COL, ALLOC_NAME("camtalk"));
    if (data != NULL) {
        sState->x = data->x;
        sState->y = data->y;
        sState->z = data->z;
        ctype = data->ctype;
    } else {
        temp_v0_2 = gDLL_2_Camera->vtbl->get_target_object();
        if (temp_v0_2 == NULL) {
            STUBBED_PRINTF(" no lock ");
            sState->x = 0.0f;
            sState->y = 0.0f;
            sState->z = 0.0f;
            return;
        }
        temp = temp_v0_2->unk74;
        if (temp == NULL) {
            STUBBED_PRINTF(" No lock Pos");
            return;
        }
        sState->x = temp->drawPoint.x;
        sState->y = temp->drawPoint.y;
        sState->z = temp->drawPoint.z;
        
    }
    if (ctype == 4) {
        ctype = rand_next(0, 3);
    }
    switch (ctype) {
    default:
        sState->unk10 = 20.0f;
        break;
    case 0:
        sState->unk10 = 20.0f;
        break;
    case 1:
        sState->unk10 = 5.0f;
        break;
    case 2:
        sState->unk10 = 40.0f;
        break;
    case 3:
        sState->unk10 = 1.0f;
        break;
    }
    STUBBED_PRINTF(" ctype %i \n", ctype);
    sState->unk1C = (s32) ctype;
    sState->unk14 = 0.0f;
    sState->unk18 = rand_next(0x2000, 0x2C00);
    xDiff = cam->srt.transl.x - sState->x;
    zDiff = cam->srt.transl.z - sState->z;
    sp30 = arctan2_f(xDiff, zDiff);
    xDiff = cam->player->srt.transl.x - sState->x;
    zDiff = cam->player->srt.transl.z - sState->z;
    temp_v0_3 = arctan2_f(xDiff, zDiff);
    var_a0 = (sState->unk18 + temp_v0_3) - (sp30 & 0xFFFF);
    CIRCLE_WRAP(var_a0);
    var_v1 = (temp_v0_3 - sState->unk18) - (sp30 & 0xFFFF);
    CIRCLE_WRAP(var_v1);
    if (var_a0 < 0) {
        var_a0 = -var_a0;
    }
    if (var_v1 < 0) {
        var_v1 = -var_v1;
    }
    if (var_v1 < var_a0) {
        sState->unk18 = (s16) -sState->unk18;
    }
}

// offset: 0x320 | func: 1 | export: 1
void dll_95_func_320(Cam* cam) {
    s32 pad_sp6C;
    f32 sp68;
    f32 sp64;
    Vec3f *sp60;
    f32 var_f16; // sp5C
    Object* temp_s0;
    f32 var_f20;
    f32 var_f22;
    f32 var_f24;
    s16 sp4A;
    f32 var_f26;

    temp_s0 = cam->player;
    if (temp_s0 == NULL) {
        return;
    }
    sp60 = (Vec3f*)&sState->x;

    var_f20 = temp_s0->srt.transl.x - sp60->x;
    var_f26 = (temp_s0->srt.transl.y + 30.0f) - sp60->y;
    var_f22 = temp_s0->srt.transl.z - sp60->z;
    var_f16 = sqrtf(SQ(var_f20) + SQ(var_f22));
    sp4A = arctan2_f(var_f20, var_f22);
    var_f20 *= 0.5f;
    var_f26 *= 0.72f;
    var_f22 *= 0.5f;
    var_f20 += sp60->x;
    var_f26 += sp60->y;
    var_f22 += sp60->z;
    var_f24 = fsin16_precise((sp4A & 0xFFFF & 0xFFFF) + (sState->unk18));
    sp68 = fcos16_precise((sp4A) + (sState->unk18));
    if (var_f16 < 25.0f) {
        var_f16 = 25.0f;
    }
    var_f16 += sState->unk10;
    cam->srt.transl.x = (var_f24 * var_f16) + var_f20;
    cam->srt.transl.y = temp_s0->srt.transl.y + 25.0f;
    cam->srt.transl.z = (sp68 * var_f16) + var_f22;
    var_f24 = cam->srt.transl.x - var_f20;
    sp64 = cam->srt.transl.y - var_f26;
    sp68 = cam->srt.transl.z - var_f22;
    cam->srt.yaw = 0x8000 - arctan2_f(var_f24, sp68);
    cam->srt.pitch = arctan2_f(sp64, sqrtf(SQ(var_f24) + SQ(sp68)));
}

// offset: 0x544 | func: 2 | export: 2
void dll_95_func_544(Cam* cam) {
    mmFree(sState);
}

// offset: 0x584 | func: 3 | export: 3
void dll_95_func_584(void* arg0, s32 arg1) {

}
