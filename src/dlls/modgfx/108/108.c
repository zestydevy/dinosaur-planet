#include "common.h"
#include "sys/gfx/modgfx.h"

/*0x0*/ static s16 data_0[] = {
    0, 0, 1000, 0, 0, 
    866, 0, 500, 11, 0,
    866, 0, -500, 22, 0,
    0, 0, -1000, 32, 0,
    -866, 0, -500, 42, 0,
    -866, 0, 500, 52, 0,
    0, 0, 1000, 63, 0,
    0, 3000, 1000, 0, 31,
    866, 3000, 500, 11, 31,
    866, 3000, -500, 22, 31,
    0, 3000, -1000, 32, 31,
    -866, 3000, -500, 42, 31,
    -866, 3000, 500, 52, 31,
    0, 3000, 1000, 63, 31,
    0, 6000, 1000, 0, 63,
    866, 6000, 500, 11, 63,
    866, 6000, -500, 22, 63,
    0, 6000, -1000, 32, 63,
    -866, 6000, -500, 42, 63,
    -866, 6000, 500, 52, 63,
    0, 6000, 1000, 63, 63
};
/*0xD4*/ static s16 data_D4[] = {
    0, 1, 8, 0, 8, 7, 1, 2, 9, 1, 9, 8, 2, 3, 10, 2,
    10, 9, 3, 4, 11, 3, 11, 10, 4, 5, 12, 4, 12, 11, 5, 6,
    13, 5, 13, 12, 7, 8, 15, 7, 15, 14, 8, 9, 16, 8, 16, 15,
    9, 10, 17, 9, 17, 16, 10, 11, 18, 10, 18, 17, 11, 12, 19, 11,
    19, 18, 12, 13, 20, 12, 20, 19, 0, 1, 2, 3, 4, 5, 6, 0
};
/*0x174*/ static s16 data_174[] = {
    7, 8, 9, 10, 11, 12, 13, 0, 14, 15, 16, 17, 18, 19, 20, 0,
    0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 17, 18, 19, 20
};
/*0x1B0*/ static s16 data_1B0[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 0
};
/*0x1DC*/ static s16 data_1DC[] = { 0, 30, 80, 30, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_108_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_108_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_108_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 4;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = 0.065f;
    temp->unk4.f[1] = 1.3f;
    temp->unk4.f[2] = 0.065f;
    temp->unk0 = 2;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 100.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 0x400000;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk4.f[0] = 155.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 4;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = -2.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 0x4000;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -100.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 0x400000;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = -2.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 0x4000;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 10.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 0x400000;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = 12.0f;
    temp->unk4.f[1] = 1.3f;
    temp->unk4.f[2] = 12.0f;
    temp->unk0 = 2;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 4;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = -2.0f;
    temp->unk4.f[2] = 0.0f;
    temp->unk0 = 0x4000;
    temp++;

    sp348.unk40 = 2;
    sp348.unk3C = 7;
    sp348.unk58 = 0;
    sp348.unk59 = 0xE;
    sp348.unk5B = 0x1E;
    sp348.unk44 = type;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = -10.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk5D = temp - sp48;
    sp348.unk4 = obj;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk5A = 0;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_1DC[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x0C000040;
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
    return dll_modgfx->Func1(&sp348, 0, 0x15, data_0, 0x18, data_D4, 0x20B, NULL);
}
