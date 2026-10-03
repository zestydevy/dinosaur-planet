#include "common.h"
#include "dlls/objects/common/campfire.h"

/*0x0*/ static s16 data_0[] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x00c8, 0x01f4, 0x0000, 0x000a, 0x0362, 0x0028, 0xfe0c, 0x0000, 0x0015, 0x0000,
    0x0096, 0xfc18, 0x0000, 0x001f, 0xfc9e, 0x005a, 0xfe0c, 0x0000, 0x002a, 0xfc9e, 0x000a, 0x01f4, 0x0000, 0x0034, 0x0000, 0x0096,
    0x03e8, 0x0000, 0x003f, 0x0000, 0x1900, 0x03e8, 0x003f, 0x0000, 0x0362, 0x189c, 0x01f4, 0x003f, 0x000a, 0x0362, 0x1838, 0xfe0c,
    0x003f, 0x0015, 0x0000, 0x1932, 0xfc18, 0x003f, 0x001f, 0xfc9e, 0x1900, 0xfe0c, 0x003f, 0x002a, 0xfc9e, 0x18ec, 0x01f4, 0x003f,
    0x0034, 0x0000, 0x1928, 0x03e8, 0x003f, 0x003f
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
/*0x110*/ static s16 data_110[] = { 0x0000, 0x0104, 0x003c, 0x0001, 0x0104, 0x0000, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_117_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_117_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_117_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    u8 sp47;
    CampFire_Setup* objSetup;
    s32 i;

    if (type == 1) {
        data_110[1] = 0;
    }
    objSetup = (CampFire_Setup*)obj->setup;
    sp47 = objSetup->scale;
    temp = sp50;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 8;
    temp->unk4.f[0] = 50.0f;
    temp->unk4.f[1] = 50.0f;
    temp->unk4.f[2] = 50.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 8;
    temp->unk4.f[0] = 200.0f;
    temp->unk4.f[1] = 200.0f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.225f;
    temp->unk4.f[1] = 0.62f;
    temp->unk4.f[2] = 0.225f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.55f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.55f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x12;
    temp->unk10 = data_D4;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 20.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 70.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 4;
    temp->unk4.f[0] = 12.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x12;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.7f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 1;
    temp->unk10 = NULL;
    temp->unk0 = 0x2000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0x12;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.7f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;
    
    sp350.unk58 = 0;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 0.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk4 = obj;
    sp350.unk44 = type;
    if (sp47 != 0) {
        sp350.unk38 = sp47 * 0.1f;
    } else {
        sp350.unk38 = 1.0f;
    }
    sp350.unk40 = 1;
    sp350.unk59 = 0xE;
    sp350.unk3C = 0;
    sp350.unk5A = 0;
    sp350.unk5B = 0x1E;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_110[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x040000C0;
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

    return dll_modgfx->Func1(&sp350, 0, 0xE, data_0, 0xC, data_8C, TEXTABLE_40, NULL);
}
