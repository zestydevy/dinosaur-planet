#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, 0, -500, 31, 0, 0, 0, -500, 31, 0, 0, 0, -500, 31, 0, 107,
    100, 407, 78, 8, 707, 100, 707, 63, 31, 738, 100, 383, 46, 31, 680, 100,
    0, 31, 31, 375, 100, -382, 15, 31, 141, 100, -407, 0, 31, 0
};
/*0x5C*/ static s16 data_5C[] = {
    0, 4, 3, 1, 5, 4, 1, 6, 5, 1, 7, 6, 2, 8, 7, 0,
    0, 0
};
/*0x80*/ static s16 data_80[] = { 0, 1, 2, 0, 0, 2, 3, 0, 4, 5, 6, 0 };
/*0x98*/ static s16 data_98[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0 };
/*0xAC*/ static s16 data_AC[] = { 3, 4, 5, 6, 7, 8, 1, 4, 5, 6, 0, 2 };
/*0xC4*/ static s16 data_C4[] = { 0, 10, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_142_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_142_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_142_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, f32* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 i;
    f32 var_fv1;
    s16* temp_v1;
    s32 temp_t9;
    void* temp_v0;

    var_fv1 = 1.0f;
    if (data != NULL) {
        var_fv1 = *data;
    }
    if (transform != NULL) {
        var_fv1 = transform->scale;
    }
    temp = sp50;
    temp->unk16 = 0;
    temp->unk14 = 5;
    temp->unk10 = data_AC;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_98;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    if (type == 1) {
        temp->unk16 = 0;
        temp->unk14 = 9;
        temp->unk10 = data_98;
        temp->unk0 = 2;
        temp->unk4.f[0] = -6.0f * var_fv1;
        temp->unk4.f[1] = 1.0f;
        temp->unk4.f[2] = 4.0f;
        temp++;
    } else {
        temp->unk16 = 0;
        temp->unk14 = 9;
        temp->unk10 = data_98;
        temp->unk0 = 2;
        temp->unk4.f[0] = 6.0f * var_fv1;
        temp->unk4.f[1] = 1.0f;
        temp->unk4.f[2] = 4.0f;
        temp++;
    }

    temp->unk16 = 1;
    temp->unk14 = 3;
    temp->unk10 = data_80;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 5;
    temp->unk10 = data_AC;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -2.5f;
    temp->unk4.f[1] = -1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 5;
    temp->unk10 = data_AC;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -2.5f;
    temp->unk4.f[1] = -1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 3;
    temp->unk10 = data_80;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp350.unk4 = obj;
    sp350.unk44 = type;
    sp350.unk40 = 1;
    sp350.unk3C = 9;
    sp350.unk59 = 9;
    sp350.unk5B = 0xA;
    sp350.unk58 = 0;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 0.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk38 = 1.0f;
    sp350.unk5A = 0;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_C4[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x04010080;
    if (sp350.unk54 & 1) {
        if (sp350.unk4 != NULL) {
            sp350.unk2C.f[0] += sp350.unk4->globalPosition.f[0];
            sp350.unk2C.f[1] += sp350.unk4->globalPosition.f[1];
            sp350.unk2C.f[2] += sp350.unk4->globalPosition.f[2];
        } else {
            sp350.unk2C.f[0] += transform->transl.f[0];
            sp350.unk2C.f[1] += transform->transl.f[1];
            sp350.unk2C.f[2] += transform->transl.f[2];
        }
    }
    return dll_modgfx->Func1(&sp350, 0, 9, data_0, 5, data_5C, TEXTABLE_3C, NULL);
}
