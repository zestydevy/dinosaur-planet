#include "common.h"

/*0x0*/ static s16 data_0[] = {
    1000, 0, 0, 0, 0, 707, 0, -707, 15, 0, 0, 0, -1000, 31, 0, -707,
    0, -707, 47, 0, -1000, 0, 0, 63, 0, -707, 0, 707, 79, 0, 0, 0,
    1000, 95, 0, 707, 0, 707, 111, 0, 1000, 0, 0, 127, 0, 1000, 2000, 0,
    0, 31, 707, 2000, -707, 15, 31, 0, 2000, -1000, 31, 31, -707, 2000, -707, 47,
    31, -1000, 2000, 0, 63, 31, -707, 2000, 707, 79, 31, 0, 2000, 1000, 95, 31,
    707, 2000, 707, 111, 31, 1000, 2000, 0, 127, 31
};
/*0xB4*/ static s16 data_B4[] = {
    0, 1, 10, 0, 10, 9, 1, 2, 11, 1, 11, 10, 2, 3, 12, 2,
    12, 11, 3, 4, 13, 3, 13, 12, 4, 5, 14, 4, 14, 13, 5, 6,
    15, 5, 15, 14, 6, 7, 16, 6, 16, 15, 7, 8, 17, 7, 17, 16
};
/*0x114*/ static s16 data_114[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0 };
/*0x128*/ static s16 data_128[] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 0
};
/*0x150*/ static s16 data_150[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 0, 2, 4, 6, 8, 10, 12, 14, 16, 0
};
/*0x188*/ static s16 data_188[] = { 9, 11, 13, 15, 17, 0 };
/*0x194*/ static s16 data_194[] = { 10, 12, 14, 16 };
/*0x19C*/ static s16 data_19C[] = { 0, 45, 0, 18, 18, 0, 30, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_128_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_128_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_128_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;
    temp = sp48;
    
    temp->unk16 = 0;
    temp->unk14 = 0x12;
    temp->unk10 = data_150;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_114;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_128;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.01f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x12;
    temp->unk10 = data_150;
    temp->unk0 = 2;
    temp->unk4.f[0] = 3.5f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 3.5f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_128;
    temp->unk0 = 8;
    temp->unk4.f[0] = 205.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 1;
    temp->unk10 = NULL;
    temp->unk0 = 0x8000;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 125.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 10.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x12;
    temp->unk10 = data_150;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 9;
    temp->unk10 = data_128;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 150.0f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x9C;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 10.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x9D;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 9;
    temp->unk10 = data_114;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 155.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x12;
    temp->unk10 = data_150;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = -10.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 5;
    temp->unk10 = data_188;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.98f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.98f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 4;
    temp->unk10 = data_194;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.02f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 1.02f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -30.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 9;
    temp->unk10 = data_114;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0x12;
    temp->unk10 = data_150;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = -10.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 5;
    temp->unk10 = data_188;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.02f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 1.02f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 4;
    temp->unk10 = data_194;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.98f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.98f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 2;
    temp->unk10 = NULL;
    temp->unk0 = 0x1000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 6;
    temp->unk14 = 0x9D;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 6;
    temp->unk14 = 0x9B;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 6;
    temp->unk14 = 0x12;
    temp->unk10 = data_150;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 6;
    temp->unk14 = 0x12;
    temp->unk10 = data_150;
    temp->unk0 = 2;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 2.0f;
    temp++;

    temp->unk16 = 6;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -30.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp348.unk58 = 0;
    sp348.unk4 = obj;
    sp348.unk44 = type;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk40 = 1;
    sp348.unk3C = 0;
    sp348.unk54 = 0x01000082;
    do { } while (0);
    sp348.unk59 = 0x12;
    sp348.unk5A = 0;
    sp348.unk5B = 0xC;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_19C[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 |= flags;
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

    return dll_modgfx->Func1(&sp348, 0, 0x12, data_0, 0x10, data_B4, TEXTABLE_45, NULL);
}
