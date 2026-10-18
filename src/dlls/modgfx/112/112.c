#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"
#include "sys/rand.h"

/*0x0*/ static s16 data_0[] = {
    -3000, 0, 0, 0, 0, -2200, 0, 1100, 11, 0, -1000, 0, 2200, 22, 0, 0,
    0, 2500, 32, 0, 1000, 0, 2200, 42, 0, 2200, 0, 1100, 52, 0, 3000, 0,
    0, 63, 0, -3000, 1500, 0, 0, 31, -2200, 1500, 1100, 11, 31, -1000, 1500, 2200,
    22, 31, 0, 1500, 2500, 32, 31, 1000, 1500, 2200, 42, 31, 2200, 1500, 1100, 52,
    31, 3000, 1500, 0, 63, 31
};
/*0x8C*/ static s16 data_8C[] = {
    0, 8, 7, 0, 1, 8, 1, 9, 8, 1, 2, 9, 2, 10, 9, 2,
    3, 10, 3, 11, 10, 3, 4, 11, 4, 12, 11, 4, 5, 12, 5, 13,
    12, 5, 6, 13, 0, 1, 2, 3, 4, 5, 6, 0, 7, 8, 9, 10,
    11, 12, 13, 0
};
/*0xF4*/ static s16 data_F4[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 7,
    6, 13
};
/*0x118*/ static s16 data_118[] = { 1, 2, 3, 4, 5, 8, 9, 10, 11, 12 };
/*0x12C*/ static s16 data_12C[] = { 0, 50, 400, 50, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_112_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_112_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_112_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 randResult;
    s32 i;

    temp = sp50;
    temp->unk16 = 0;
    temp->unk14 = 14;
    temp->unk10 = data_F4;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 14;
    temp->unk10 = data_F4;
    temp->unk4.f[0] = 0.1f;
    temp->unk4.f[1] = 0.1f;
    temp->unk4.f[2] = 0.1f;
    temp->unk0 = 2;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 14;
    temp->unk10 = data_F4;
    temp->unk0 = 8;
    temp->unk4.f[0] = mathRnd(0, 0x69) + 150.0f;
    temp->unk4.f[1] = mathRnd(0, 0x69) + 150.0f;
    temp->unk4.f[2] = mathRnd(0, 0x69) + 150.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 472;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    randResult = mathRnd(0, 0xFFFE);
    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = randResult;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 10;
    temp->unk10 = data_118;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 14;
    temp->unk10 = data_F4;
    temp->unk0 = 2;
    temp->unk4.f[0] = 5.0f;
    temp->unk4.f[1] = 5.0f;
    temp->unk4.f[2] = 5.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 14;
    temp->unk10 = data_F4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.5f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 14;
    temp->unk10 = data_F4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.5f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 83;
    temp->unk10 = NULL;
    temp->unk0 = 0x800000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 84;
    temp->unk10 = NULL;
    temp->unk0 = 0x01800000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 8.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 10;
    temp->unk10 = data_118;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 14;
    temp->unk10 = data_F4;
    temp->unk0 = 2;
    temp->unk4.f[0] = 5.0f;
    temp->unk4.f[1] = 5.0f;
    temp->unk4.f[2] = 5.0f;
    temp++;

    sp350.unk4 = obj;
    sp350.unk44 = type;
    sp350.unk40 = 1;
    sp350.unk59 = 14;
    sp350.unk5B = 16;
    sp350.unk58 = 0;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 5.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk3C = 0;
    sp350.unk38 = 1.0f;
    sp350.unk5A = 0;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_12C[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x01000000;
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

    return dll_modgfx->Func1(&sp350, 0, 14, data_0, 0xC, data_8C, TEXTABLE_46, NULL);
}
