#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, 0, 1000, 0, 0, 866, 0, 500, 11, 0, 866, 0, -500, 22, 0, 0,
    0, -1000, 32, 0, -866, 0, -500, 22, 0, -866, 0, 500, 11, 0, 0, 0,
    1000, 0, 0, 0, 500, 1000, 0, 15, 866, 500, 500, 11, 15, 866, 500, -500,
    22, 15, 0, 500, -1000, 32, 15, -866, 500, -500, 22, 15, -866, 500, 500, 11,
    15, 0, 500, 1000, 0, 15, 0, 6000, 1000, 0, 127, 866, 6000, 500, 11, 127,
    866, 6000, -500, 22, 127, 0, 6000, -1000, 32, 127, -866, 6000, -500, 22, 127, -866,
    6000, 500, 11, 127, 0, 6000, 1000, 0, 127, 0
};
/*0xD4*/ static s16 data_D4[] = {
    0, 1, 8, 0, 8, 7, 1, 2, 9, 1, 9, 8, 2, 3, 10, 2,
    10, 9, 3, 4, 11, 3, 11, 10, 4, 5, 12, 4, 12, 11, 5, 6,
    13, 5, 13, 12, 7, 8, 15, 7, 15, 14, 8, 9, 16, 8, 16, 15,
    9, 10, 17, 9, 17, 16, 10, 11, 18, 10, 18, 17, 11, 12, 19, 11,
    19, 18, 12, 13, 20, 12, 20, 19, 0, 1, 2, 3, 4, 5, 6, 0,
    7, 8, 9, 10, 11, 12, 13, 0, 14, 15, 16, 17, 18, 19, 20, 0,
    0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 17, 18, 19, 20
};
/*0x1B0*/ static s16 data_1B0[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 0
};
/*0x1DC*/ static s16 data_1DC[] = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
/*0x1F8*/ static s16 data_1F8[] = { 0, 150, 250, 50, 1, 0, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_129_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_129_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_129_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
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
    temp->unk4.f[0] = 0.01f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 0.01f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 100.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x124;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 2;
    temp->unk4.f[0] = 200.0f;
    temp->unk4.f[1] = 1.3f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0xE;
    temp->unk10 = data_1DC;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -100.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x124;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0xE;
    temp->unk10 = data_1DC;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.01f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.01f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 100.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp348.unk58 = 0;
    sp348.unk4 = obj;
    sp348.unk44 = type;
    sp348.unk40 = 2;
    sp348.unk3C = 7;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk59 = 0xE;
    sp348.unk5A = 0;
    sp348.unk5B = 0x1E;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_1F8[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x0C0100C0;
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

    return dll_modgfx->Func1(&sp348, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_154, NULL);
}
