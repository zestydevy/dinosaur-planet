#include "common.h"

/*0x0*/ static s16 data_0[] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x000b, 0x0000, 0x0362, 0x0000, 0xfe0c, 0x0016, 0x0000, 0x0000,
    0x0000, 0xfc18, 0x0020, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x002a, 0x0000, 0xfc9e, 0x0000, 0x01f4, 0x0034, 0x0000, 0x0000, 0x0000,
    0x03e8, 0x003f, 0x0000, 0x0000, 0x0bb8, 0x03e8, 0x0000, 0x001f, 0x0362, 0x0bb8, 0x01f4, 0x000b, 0x001f, 0x0362, 0x0bb8, 0xfe0c,
    0x0016, 0x001f, 0x0000, 0x0bb8, 0xfc18, 0x0020, 0x001f, 0xfc9e, 0x0bb8, 0xfe0c, 0x002a, 0x001f, 0xfc9e, 0x0bb8, 0x01f4, 0x0034,
    0x001f, 0x0000, 0x0bb8, 0x03e8, 0x003f, 0x001f, 0x0000, 0x1770, 0x03e8, 0x0000, 0x003f, 0x0362, 0x1770, 0x01f4, 0x000b, 0x003f,
    0x0362, 0x1770, 0xfe0c, 0x0016, 0x003f, 0x0000, 0x1770, 0xfc18, 0x0020, 0x003f, 0xfc9e, 0x1770, 0xfe0c, 0x002a, 0x003f, 0xfc9e,
    0x1770, 0x01f4, 0x0034, 0x003f, 0x0000, 0x1770, 0x03e8, 0x003f, 0x003f, 0x0000
};
/*0xD4*/ static s16 data_D4[] = {
    0x0000, 0x0001, 0x0008, 0x0000, 0x0008, 0x0007, 0x0001, 0x0002, 0x0009, 0x0001, 0x0009, 0x0008, 0x0002, 0x0003, 0x000a, 0x0002,
    0x000a, 0x0009, 0x0003, 0x0004, 0x000b, 0x0003, 0x000b, 0x000a, 0x0004, 0x0005, 0x000c, 0x0004, 0x000c, 0x000b, 0x0005, 0x0006,
    0x000d, 0x0005, 0x000d, 0x000c, 0x0007, 0x0008, 0x000f, 0x0007, 0x000f, 0x000e, 0x0008, 0x0009, 0x0010, 0x0008, 0x0010, 0x000f,
    0x0009, 0x000a, 0x0011, 0x0009, 0x0011, 0x0010, 0x000a, 0x000b, 0x0012, 0x000a, 0x0012, 0x0011, 0x000b, 0x000c, 0x0013, 0x000b,
    0x0013, 0x0012, 0x000c, 0x000d, 0x0014, 0x000c, 0x0014, 0x0013, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0000
};
/*0x174*/ static s16 data_174[] = {
    0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x0000, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0000,
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014
};
/*0x1B0*/ static s16 data_1B0[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
    0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0000
};
/*0x1DC*/ static s16 data_1DC[] = { 0x0000, 0x001e, 0x0050, 0x001e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_109_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_109_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_109_modgfx_Func0(Object* arg0, s32 arg1, SRT* arg2, u32 arg3, s32 arg4, void* arg5) {
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
    temp->unk4.f[0] = 0.1f;
    temp->unk4.f[1] = 1.3f;
    temp->unk4.f[2] = 0.1f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 100.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 4;
    temp->unk4.f[0] = 85.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -2.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
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
    temp->unk4.f[1] = -2.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 10.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 2;
    temp->unk4.f[0] = 12.0f;
    temp->unk4.f[1] = 1.3f;
    temp->unk4.f[2] = 12.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 7;
    temp->unk10 = data_174;
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
    temp->unk4.f[1] = -2.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp348.unk40 = 2;
    sp348.unk3C = 7;
    sp348.unk44 = arg1;
    sp348.unk4 = arg0;
    sp348.unk59 = 0xE;
    sp348.unk5B = 0x1E;
    sp348.unk58 = 0;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = -10.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk5A = 0;
    sp348.unk38 = 1.0f;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_1DC[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = arg3 | 0x0C000040;
    if (sp348.unk54 & 1) {
        if (sp348.unk4 != NULL) {
            sp348.unk2C.f[0] += sp348.unk4->globalPosition.f[0];
            sp348.unk2C.f[1] += sp348.unk4->globalPosition.f[1];
            sp348.unk2C.f[2] += sp348.unk4->globalPosition.f[2];
        } else {
            sp348.unk2C.f[0] += arg2->transl.f[0];
            sp348.unk2C.f[1] += arg2->transl.f[1];
            sp348.unk2C.f[2] += arg2->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp348, 0, 0x15, data_0, 0x18, data_D4, 0x20B, NULL);
}
