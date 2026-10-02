#include "common.h"

/*0x0*/ static s16 data_0[] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x001f, 0x0000, 0x0362, 0x0000, 0xfe0c, 0x003f, 0x0000, 0x0000,
    0x0000, 0xfc18, 0x005f, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x007f, 0x0000, 0xfc9e, 0x0000, 0x01f4, 0x009f, 0x0000, 0x0000, 0x0000,
    0x03e8, 0x00bf, 0x0000, 0x0000, 0x1770, 0x03e8, 0x0000, 0x003f, 0x0362, 0x1770, 0x01f4, 0x001f, 0x003f, 0x0362, 0x1770, 0xfe0c,
    0x003f, 0x003f, 0x0000, 0x1770, 0xfc18, 0x005f, 0x003f, 0xfc9e, 0x1770, 0xfe0c, 0x007f, 0x003f, 0xfc9e, 0x1770, 0x01f4, 0x009f,
    0x003f, 0x0000, 0x1770, 0x03e8, 0x00bf, 0x003f
};
/*0x8C*/ static s16 data_8C[] = {
    0x0000, 0x0001, 0x0008, 0x0000, 0x0008, 0x0007, 0x0001, 0x0002, 0x0009, 0x0001, 0x0009, 0x0008, 0x0002, 0x0003, 0x000a, 0x0002,
    0x000a, 0x0009, 0x0003, 0x0004, 0x000b, 0x0003, 0x000b, 0x000a, 0x0004, 0x0005, 0x000c, 0x0004, 0x000c, 0x000b, 0x0005, 0x0006,
    0x000d, 0x0005, 0x000d, 0x000c
};
/*0xD4*/ static s16 data_D4[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d
};
/*0xF0*/ static s16 data_F0[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0000
};
/*0x100*/ static s16 data_100[] = {
    0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x0000
};
/*0x110*/ static s16 data_110[] = { 0x0000, 0x0014, 0x00aa, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };


// offset: 0x0 | ctor
void dll_111_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_111_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_111_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 0x32;
    temp->unk10 = NULL;
    temp->unk0 = 0x800000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x9B;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.7f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.7f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.2f;
    temp->unk4.f[1] = -1.0f;
    temp->unk4.f[2] = 1.2f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 8;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 160.0f;
    temp->unk4.f[2] = 115.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 115.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 1;
    temp->unk10 = NULL;
    temp->unk0 = 0x8000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 1;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -130.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 1;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 14;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 1;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 90.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp348.unk5D = temp - sp48;
    sp348.unk40 = 1;
    sp348.unk59 = 0xE;
    sp348.unk5B = 0x10;
    sp348.unk58 = 0;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk3C = 0;
    sp348.unk5A = 0;
    sp348.unk5D = 0;
    sp348.unk44 = type;
    sp348.unk4 = obj;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_110[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x04000002;
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

    return dll_modgfx->Func1(&sp348, 0, 14, data_0, 12, data_8C, 72, NULL);
}
