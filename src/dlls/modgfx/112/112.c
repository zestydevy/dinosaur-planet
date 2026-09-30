#include "common.h"

/*0x0*/ static s16 data_0[] = {
    0xf448, 0x0000, 0x0000, 0x0000, 0x0000, 0xf768, 0x0000, 0x044c, 0x000b, 0x0000, 0xfc18, 0x0000, 0x0898, 0x0016, 0x0000, 0x0000,
    0x0000, 0x09c4, 0x0020, 0x0000, 0x03e8, 0x0000, 0x0898, 0x002a, 0x0000, 0x0898, 0x0000, 0x044c, 0x0034, 0x0000, 0x0bb8, 0x0000,
    0x0000, 0x003f, 0x0000, 0xf448, 0x05dc, 0x0000, 0x0000, 0x001f, 0xf768, 0x05dc, 0x044c, 0x000b, 0x001f, 0xfc18, 0x05dc, 0x0898,
    0x0016, 0x001f, 0x0000, 0x05dc, 0x09c4, 0x0020, 0x001f, 0x03e8, 0x05dc, 0x0898, 0x002a, 0x001f, 0x0898, 0x05dc, 0x044c, 0x0034,
    0x001f, 0x0bb8, 0x05dc, 0x0000, 0x003f, 0x001f
};
/*0x8C*/ static s16 data_8C[] = {
    0x0000, 0x0008, 0x0007, 0x0000, 0x0001, 0x0008, 0x0001, 0x0009, 0x0008, 0x0001, 0x0002, 0x0009, 0x0002, 0x000a, 0x0009, 0x0002,
    0x0003, 0x000a, 0x0003, 0x000b, 0x000a, 0x0003, 0x0004, 0x000b, 0x0004, 0x000c, 0x000b, 0x0004, 0x0005, 0x000c, 0x0005, 0x000d,
    0x000c, 0x0005, 0x0006, 0x000d, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0000, 0x0007, 0x0008, 0x0009, 0x000a,
    0x000b, 0x000c, 0x000d, 0x0000
};
/*0xF4*/ static s16 data_F4[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x0000, 0x0007,
    0x0006, 0x000d
};
/*0x118*/ static s16 data_118[] = {
    0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c
};
/*0x12C*/ static s16 data_12C[] = { 0x0000, 0x0032, 0x0190, 0x0032, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_112_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_112_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_112_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
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

    return dll_modgfx->Func1(&sp350, 0, 14, data_0, 0xC, data_8C, 0x46, NULL);
}
