#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, 3400, 0, 0, 0, 866, 2900, 0, 8, 0, 866, 1900, 0, 15, 0, 0,
    1400, 0, 31, 0, -866, 1900, 0, 15, 0, -866, 2900, 0, 8, 0, 0, 3400,
    0, 0, 0, 0, 3400, 2500, 0, 31, 866, 2900, 2500, 8, 31, 866, 1900, 2500,
    15, 31, 0, 1400, 2500, 31, 31, -866, 1900, 2500, 15, 31, -866, 2900, 2500, 8,
    31, 0, 3400, 2500, 0, 31, 0, 3400, 6000, 0, 63, 866, 2900, 6000, 8, 63,
    866, 1900, 6000, 15, 63, 0, 1400, 6000, 31, 63, -866, 1900, 6000, 15, 63, -866,
    2900, 6000, 8, 63, 0, 3400, 6000, 0, 63, 0
};
/*0xD4*/ static s16 data_D4[] = {
    0, 8, 1, 0, 7, 8, 1, 9, 2, 1, 8, 9, 2, 10, 3, 2,
    9, 10, 3, 11, 4, 3, 10, 11, 4, 12, 5, 4, 11, 12, 5, 13,
    6, 5, 12, 13, 7, 15, 8, 7, 14, 15, 8, 16, 9, 8, 15, 16,
    9, 17, 10, 9, 16, 17, 10, 18, 11, 10, 17, 18, 11, 19, 12, 11,
    18, 19, 12, 20, 13, 12, 19, 20
};
/*0x164*/ static s16 data_164[] = { 0, 1, 2, 3, 4, 5, 6, 0 };
/*0x174*/ static s16 data_174[] = { 7, 8, 9, 10, 11, 12, 13, 0 };
/*0x184*/ static s16 data_184[] = {
    14, 15, 16, 17, 18, 19, 20, 0, 0, 1, 2, 3, 4, 5, 6, 14,
    15, 16, 17, 18, 19, 20
};
/*0x1B0*/ static s16 data_1B0[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20
};
/*0x1F8*/ static s16 data_1F8[] = { 0, 0, 15, 45, 0, 0, 0, 0 };
/*0x208*/ static s32 data_208 = 0;

// offset: 0x0 | ctor
void dll_141_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_141_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_141_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, f32* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;
    f32 var_fa0;
    s16 temp_v0;

    var_fa0 = 1.0f;
    if (data != NULL) {
        var_fa0 = *data;
    }

    temp = sp50;
    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.15f * var_fa0;
    temp->unk4.f[1] = 0.15f * var_fa0;
    temp->unk4.f[2] = 0.9f * var_fa0;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_184;
    temp->unk0 = 2;
    temp->unk4.f[0] = 3.0f;
    temp->unk4.f[1] = 3.0f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 7;
    temp->unk10 = data_164;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 2;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 4.0f;
    temp->unk4.f[1] = -6.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 4.0f;
    temp->unk4.f[1] = -6.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 7;
    temp->unk10 = data_164;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp350.unk4 = obj;
    sp350.unk44 = type;
    sp350.unk58 = 0;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 0.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk38 = 1.0f;
    sp350.unk40 = 2;
    sp350.unk3C = 7;
    sp350.unk59 = 0xE;
    sp350.unk5A = 0;
    sp350.unk5B = 0xA;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_1F8[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x0C010080;
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

    temp_v0 = dll_modgfx->Func1(&sp350, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_89, NULL);
    data_208++;
    if (data_208 == 5) {
        data_208 = 0;
    }

    return temp_v0;
}
