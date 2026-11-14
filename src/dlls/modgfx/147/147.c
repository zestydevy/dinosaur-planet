#include "common.h"

/*0x0*/ static s16 data_0[] = {
    1100, 0, 0, 0, 0, 807, 0, -707, 15, 0, 0, 0, -1000, 31, 0, -607,
    0, -707, 47, 0, -900, 0, 0, 63, 0, -607, 0, 707, 79, 0, 0, 0,
    1000, 95, 0, 807, 0, 707, 111, 0, 1100, 0, 0, 127, 0, 1200, 2000, 100,
    0, 31, 807, 2000, -607, 15, 31, 100, 2000, -900, 31, 31, -607, 2000, -607, 47,
    31, -900, 2000, 100, 63, 31, -607, 2000, 807, 79, 31, 0, 2000, 1100, 95, 31,
    807, 2000, 807, 111, 31, 1200, 2000, 100, 127, 31, 900, 4000, 100, 0, 63, 607,
    4000, -607, 15, 63, -100, 4000, -900, 31, 63, -807, 4000, -607, 47, 63, -1100, 4000,
    100, 63, 63, -807, 4000, 807, 79, 63, 100, 4000, 1100, 95, 63, 607, 4000, 807,
    111, 63, 900, 4000, 100, 127, 63, 1000, 6000, -100, 0, 94, 707, 6000, -807, 15,
    94, 0, 6000, -1100, 31, 94, -707, 6000, -807, 47, 94, -1000, 6000, -100, 63, 94,
    -707, 6000, 607, 79, 94, 0, 6000, 900, 95, 94, 707, 6000, 607, 111, 94, 1000,
    6000, -100, 127, 94
};
/*0x168*/ static s16 data_168[] = {
    0, 1, 10, 0, 10, 9, 1, 2, 11, 1, 11, 10, 2, 3, 12, 2,
    12, 11, 3, 4, 13, 3, 13, 12, 4, 5, 14, 4, 14, 13, 5, 6,
    15, 5, 15, 14, 6, 7, 16, 6, 16, 15, 7, 8, 17, 7, 17, 16
};
/*0x1C8*/ static s16 data_1C8[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 0
};
/*0x1DC*/ static s16 data_1DC[] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17, 0
};
/*0x1F0*/ static s16 data_1F0[] = {
    18, 19, 20, 21, 22, 23, 24, 25, 26, 0
};
/*0x204*/ static s16 data_204[] = {
    27, 28, 29, 30, 31, 32, 33, 34, 35, 0, 0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
};
/*0x260*/ static s16 data_260[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35
};
/*0x2A8*/ static s16 data_2A8[] = {
    9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26
};
/*0x2CC*/ static s16 data_2CC[] = { 0, 100, 90, 660, 90, 0, 0, 0 };
/*0x2DC*/ static s32 data_2DC[] = { TEXTABLE_1D9, TEXTABLE_1FD, TEXTABLE_201, TEXTABLE_203, 0 };
// offset: 0x0 | ctor
void dll_147_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_147_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_147_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_1C8;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.1f;
    temp->unk4.f[1] = 0.2f;
    temp->unk4.f[2] = 0.1f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_1DC;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.15f;
    temp->unk4.f[1] = 0.2f;
    temp->unk4.f[2] = 0.15f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_1F0;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.15f;
    temp->unk4.f[1] = 0.2f;
    temp->unk4.f[2] = 0.15f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_204;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.15f;
    temp->unk4.f[1] = 0.2f;
    temp->unk4.f[2] = 0.15f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 8.0f;
    temp->unk4.f[1] = 32.0f;
    temp->unk4.f[2] = 5.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 2;
    temp->unk4.f[0] = 18.5f;
    temp->unk4.f[1] = 5.4f;
    temp->unk4.f[2] = 18.5f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 80.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 18;
    temp->unk10 = data_2A8;
    temp->unk0 = 4;
    temp->unk4.f[0] = 200.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 2;
    temp->unk4.f[0] = 4.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 4.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = -8.0f;
    temp->unk4.f[1] = -32.0f;
    temp->unk4.f[2] = -5.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 80.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 80.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 18;
    temp->unk10 = data_2A8;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0x24;
    temp->unk10 = data_260;
    temp->unk0 = 2;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 2.0f;
    temp++;

    sp348.unk58 = 0;
    sp348.unk44 = type;
    sp348.unk4 = obj;
    sp348.unk40 = 3;
    sp348.unk38 = 1.0f;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk3C = 9;
    sp348.unk59 = 18;
    sp348.unk5A = 0;
    sp348.unk5B = 16;
    sp348.unk54 = 0x04000484;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_2CC[i]; }
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

    return dll_modgfx->Func1(&sp348, 0, 0x24, data_0, 16, data_168, data_2DC[type], NULL);
}
