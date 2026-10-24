#include "game/objects/setups/52_CampFire.h"
#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, 0, 1000, 0, 0, 866, 0, 500, 11, 0, 866, 0, -500, 22, 0, 0,
    0, -1000, 32, 0, -866, 0, -500, 42, 0, -866, 0, 500, 52, 0, 0, 0,
    1000, 63, 0, 0, 1600, 1000, 0, 15, 866, 1600, 500, 11, 15, 866, 1600, -500,
    22, 15, 0, 1600, -1000, 32, 15, -866, 1600, -500, 42, 15, -866, 1600, 500, 52,
    15, 0, 1600, 1000, 63, 15, 0, 6000, 1000, 0, 31, 866, 6000, 500, 11, 31,
    866, 6000, -500, 22, 31, 0, 6000, -1000, 32, 31, -866, 6000, -500, 42, 31, -866,
    6000, 500, 52, 31, 0, 6000, 1000, 63, 31, 0
};
/*0xD4*/ static s16 data_D4[] = {
    0, 1, 8, 0, 8, 7, 1, 2, 9, 1, 9, 8, 2, 3, 10, 2,
    10, 9, 3, 4, 11, 3, 11, 10, 4, 5, 12, 4, 12, 11, 5, 6,
    13, 5, 13, 12, 7, 8, 15, 7, 15, 14, 8, 9, 16, 8, 16, 15,
    9, 10, 17, 9, 17, 16, 10, 11, 18, 10, 18, 17, 11, 12, 19, 11,
    19, 18, 12, 13, 20, 12, 20, 19, 0, 1, 2, 3, 4, 5, 6, 0
};
/*0x174*/ static s16 data_174[] = { 7, 8, 9, 10, 11, 12, 13, 0 };
/*0x184*/ static s16 data_184[] = { 14, 15, 16, 17, 18, 19, 20, 0 };
/*0x194*/ static s16 data_194[] = { 0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 17, 18, 19, 20 };
/*0x1B0*/ static s16 data_1B0[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 0
};
/*0x1DC*/ static s16 data_1DC[] = { 0, 260, 60, 60, 1, 260, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_114_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_114_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_114_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    CampFire_Setup* setup;
    f32 var_fa1;
    u8 var_t0;
    s32 pad[1];
    s32 i;

    var_fa1 = 1.0f;
    setup = (CampFire_Setup*)obj->setup;
    var_t0 = setup->scale;
    if (type == 1) {
        var_fa1 = 4.0f;
        data_1DC[1] = 0;
    } else if (type == 2) {
        var_fa1 = 0.0f;
        var_t0 = 6;
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
    temp->unk14 = 0xE;
    temp->unk10 = data_194;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.95f;
    temp->unk4.f[1] = 0.4f;
    temp->unk4.f[2] = 0.95f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.95f;
    temp->unk4.f[1] = 0.4f;
    temp->unk4.f[2] = 0.95f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_184;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 10.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x3A;
    temp->unk10 = NULL;
    temp->unk0 = 0x01800000;
    temp->unk4.f[0] = var_fa1;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 5.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 10.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x3A;
    temp->unk10 = NULL;
    temp->unk0 = 0x01800000;
    temp->unk4.f[0] = var_fa1;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 5.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 10.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 2;
    temp->unk10 = NULL;
    temp->unk0 = 0x2000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 7;
    temp->unk10 = data_184;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 10.0f;
    temp++;

    sp350.unk58 = 0;
    sp350.unk4 = obj;
    sp350.unk44 = type;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 0.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    if (var_t0 != 0) {
        sp350.unk38 = var_t0 * 0.1f;
    } else {
        sp350.unk38 = 1.0f;
    }
    sp350.unk40 = 2;
    sp350.unk3C = 7;
    sp350.unk59 = 0xE;
    sp350.unk5B = 0x1E;
    sp350.unk5A = 0;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_1DC[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x0C0000C0;
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

    return dll_modgfx->Func1(&sp350, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_8E, NULL);
}
