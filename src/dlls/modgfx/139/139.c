#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"
#include "sys/rand.h"

/*0x0*/ static s16 data_0[] = {
    -3000, 0, -300, 0, 0, -2200, 0, 500, 6, 0, -1000, 0, 0, 11, 0, 0,
    0, -600, 16, 0, 1000, 0, 0, 21, 0, 2200, 0, 700, 26, 0, 3000, 0,
    300, 31, 0, -3000, 800, 0, 0, 31, -2200, 1500, -400, 6, 31, -1000, 1300, 0,
    22, 31, 0, 1500, 900, 11, 31, 1000, 1200, -300, 16, 31, 2200, 1500, 0, 26,
    31, 3000, 800, 700, 31, 31
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
/*0x12C*/ static s16 data_12C[] = { 0, 50, 200, 50, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_139_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_139_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_139_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    if (type == 1) {
        data_12C[2] = 0x1130;
    } else {
        data_12C[2] = 0x64;
    }

    temp->unk16 = 0;
    temp->unk14 = 0xE;
    temp->unk10 = data_F4;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    if (type == 1) {
        temp->unk16 = 0;
        temp->unk14 = 0xE;
        temp->unk10 = data_F4;
        temp->unk0 = 2;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 1.0f;
        temp->unk4.f[2] = 1.0f;
        temp++;
    } else {
        temp->unk16 = 0;
        temp->unk14 = 0xE;
        temp->unk10 = data_F4;
        temp->unk0 = 2;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = mathRnd(3, 5) * 0.7f;
        temp->unk4.f[2] = 1.0f;
        temp++;
    }

    temp->unk16 = 0;
    temp->unk14 = 0xE;
    temp->unk10 = data_F4;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 16767.0f;
    temp++;

    if (type == 1) {
        temp->unk16 = 0;
        temp->unk14 = 0xE;
        temp->unk10 = data_F4;
        temp->unk0 = 0x400000;
        temp->unk4.f[0] = 100.0f;
        temp->unk4.f[1] = 75.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 0x190;
        temp->unk10 = NULL;
        temp->unk0 = 0x20000000;
        temp->unk4.f[0] = 999.0f;
        temp->unk4.f[1] = 111.0f;
        temp->unk4.f[2] = 112.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x80000;
        temp->unk4.f[0] = 70.0f;
        temp->unk4.f[1] = 150.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else {
        temp->unk16 = 0;
        temp->unk14 = 0xE;
        temp->unk10 = data_F4;
        temp->unk0 = 0x400000;
        temp->unk4.f[0] = mathRnd(0, 0x14) + 90.0f;
        temp->unk4.f[1] = 75.0f;
        temp->unk4.f[2] = mathRnd(0, 0x1E);
        temp++;
    }

    temp->unk16 = 1;
    temp->unk14 = 0xA;
    temp->unk10 = data_118;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0xE;
    temp->unk10 = data_F4;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    if (type != 1) {
        temp->unk16 = 2;
        temp->unk14 = 0xE;
        temp->unk10 = data_F4;
        temp->unk0 = 0x400000;
        temp->unk4.f[0] = mathRnd(1, 0x28) * -0.3f;
        temp->unk4.f[1] = 0/*.0f*/;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }

    temp->unk16 = 2;
    temp->unk14 = 0xE;
    temp->unk10 = data_F4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = mathRnd(-3, 3) * 0.1f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0xE;
    temp->unk10 = data_F4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.3f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0xA;
    temp->unk10 = data_118;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    if (type == 1) {
        temp->unk16 = 3;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x20000000;
        temp->unk4.f[0] = 999.0f;
        temp->unk4.f[1] = 111.0f;
        temp->unk4.f[2] = 112.0f;
        temp++;
    }

    sp348.unk4 = obj;
    sp348.unk44 = type;
    sp348.unk58 = 0;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk40 = 1;
    sp348.unk3C = 0;
    sp348.unk59 = 0xE;
    sp348.unk5A = 0;
    sp348.unk2C.f[1] = 5.0f;
    sp348.unk5B = mathRnd(0x18, 0x1C);
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_12C[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x01000000;
    if (sp348.unk54 & 1) {
        if (sp348.unk4 != NULL) {
            sp348.unk2C.f[0] += sp348.unk4->globalPosition.f[0];
            sp348.unk2C.f[1] += sp348.unk4->globalPosition.f[1];
            sp348.unk2C.f[2] += sp348.unk4->globalPosition.f[2];
        } else {
            sp348.unk2C.f[0] += transform->transl.f[0];
            sp348.unk2C.f[1] += transform->transl.f[1];
            sp348.unk2C.f[2] += transform->transl.f[2];
        }
    }
    return dll_modgfx->Func1(&sp348, 0, 0xE, data_0, 0xC, data_8C, TEXTABLE_8E, NULL);
}
