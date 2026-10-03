#include "common.h"

/*0x0*/ static s16 data_0[] = {
    0x03e8, 0x0000, 0x0000, 0x0000, 0x0000, 0x02c3, 0x0000, 0xfd3d, 0x000f, 0x0000, 0x0000, 0x0000, 0xfc18, 0x001f, 0x0000, 0xfd3d,
    0x0000, 0xfd3d, 0x002f, 0x0000, 0xfc18, 0x0000, 0x0000, 0x003f, 0x0000, 0xfd3d, 0x0000, 0x02c3, 0x004f, 0x0000, 0x0000, 0x0000,
    0x03e8, 0x005f, 0x0000, 0x02c3, 0x0000, 0x02c3, 0x006f, 0x0000, 0x03e8, 0x0000, 0x0000, 0x007f, 0x0000, 0x03e8, 0x07d0, 0x0000,
    0x0000, 0x000f, 0x02c3, 0x07d0, 0xfd3d, 0x000f, 0x000f, 0x0000, 0x07d0, 0xfc18, 0x001f, 0x000f, 0xfd3d, 0x07d0, 0xfd3d, 0x002f,
    0x000f, 0xfc18, 0x07d0, 0x0000, 0x003f, 0x000f, 0xfd3d, 0x07d0, 0x02c3, 0x004f, 0x000f, 0x0000, 0x07d0, 0x03e8, 0x005f, 0x000f,
    0x02c3, 0x07d0, 0x02c3, 0x006f, 0x000f, 0x03e8, 0x07d0, 0x0000, 0x007f, 0x000f
};
/*0xB4*/ static s16 data_B4[] = {
    0x0000, 0x0001, 0x000a, 0x0000, 0x000a, 0x0009, 0x0001, 0x0002, 0x000b, 0x0001, 0x000b, 0x000a, 0x0002, 0x0003, 0x000c, 0x0002,
    0x000c, 0x000b, 0x0003, 0x0004, 0x000d, 0x0003, 0x000d, 0x000c, 0x0004, 0x0005, 0x000e, 0x0004, 0x000e, 0x000d, 0x0005, 0x0006,
    0x000f, 0x0005, 0x000f, 0x000e, 0x0006, 0x0007, 0x0010, 0x0006, 0x0010, 0x000f, 0x0007, 0x0008, 0x0011, 0x0007, 0x0011, 0x0010
};
/*0x114*/ static s16 data_114[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0000
};
/*0x128*/ static s16 data_128[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
    0x0010, 0x0011, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010, 0x0011, 0x0000
};
/*0x160*/ static s16 data_160[] = { 0x0000, 0x0032, 0x0000, 0x0064, 0x0000, 0x0032, 0x0000, 0x0000 };
/*0x170*/ static u8 data_170[] = { 0x32, 0x96, 0xff, 0x32, 0xff, 0x96, 0x9b, 0x64, 0x0a, 0xff, 0x64, 0x82, 0x00, 0x00, 0x00, 0x00 };

// offset: 0x0 | ctor
void dll_122_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_122_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_122_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk14 = 0x12;
    temp->unk16 = 0;
    temp->unk10 = data_128;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x12;
    temp->unk10 = data_128;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.2f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.2f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_114;
    temp->unk0 = 8;
    temp->unk4.f[0] = data_170[type * 3 + 0];
    temp->unk4.f[1] = data_170[type * 3 + 1];
    temp->unk4.f[2] = data_170[type * 3 + 2];
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x12;
    temp->unk10 = data_128;
    temp->unk0 = 4;
    temp->unk4.f[0] = 85.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x12;
    temp->unk10 = data_128;
    temp->unk0 = 2;
    temp->unk4.f[0] = 15.0f;
    temp->unk4.f[1] = 0.15f;
    temp->unk4.f[2] = 15.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x12;
    temp->unk10 = data_128;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 30.0f;
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
    temp->unk14 = 0x12;
    temp->unk10 = data_128;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 0x12;
    temp->unk10 = data_128;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.1f;
    temp->unk4.f[1] = 10.0f;
    temp->unk4.f[2] = 0.1f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 0xB2;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp348.unk40 = 1;
    sp348.unk44 = type;
    sp348.unk4 = obj;
    sp348.unk58 = 0;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk3C = 0;
    sp348.unk38 = 1.0f;
    sp348.unk59 = 0x12;
    sp348.unk5A = 0;
    sp348.unk5B = 0x10;
    sp348.unk5D = temp - sp48;
    sp348.unk54 = 0x05000004;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_160[i]; }
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

    return dll_modgfx->Func1(&sp348, 0, 0x12, data_0, 0x10, data_B4, TEXTABLE_3E, NULL);
}
