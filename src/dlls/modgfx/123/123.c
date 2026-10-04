#include "common.h"

/*0x0*/ static s16 data_0[] = {
    0xfc18, 0x0000, 0xfc18, 0x0000, 0x0000, 0x03e8, 0x0000, 0xfc18, 0x003f, 0x0000, 0x03e8, 0x0000, 0x03e8, 0x003f, 0x003f, 0xfc18, 
    0x0000, 0x03e8, 0x0000, 0x003f, 0x0000, 0x0000, 0x0000, 0x0020, 0x0020, 0x0000
};
/*0x34*/ static s16 data_34[] = {
    0x0000, 0x0001, 0x0004, 0x0001, 0x0002, 0x0004, 0x0004, 0x0002, 0x0003, 0x0000, 0x0004, 0x0003, 0x0000, 0x0000, 0x0000, 0x0000
};
/*0x54*/ static s16 data_54[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0000, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0000
};
/*0x6C*/ static s16 data_6C[] = { 0x0000, 0x0050, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_123_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_123_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_123_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 5;
    temp->unk10 = data_54;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 5;
    temp->unk10 = data_54;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.01f;
    temp->unk4.f[1] = 0.01f;
    temp->unk4.f[2] = 0.01f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 5;
    temp->unk10 = data_54;
    temp->unk0 = 8;
    temp->unk4.f[0] = 200.0f;
    temp->unk4.f[1] = 200.0f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x93;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 5;
    temp->unk10 = data_54;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 5;
    temp->unk10 = data_54;
    temp->unk0 = 2;
    temp->unk4.f[0] = 4000.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 4000.0f;
    temp++;

    sp348.unk44 = type;
    sp348.unk4 = obj;
    sp348.unk40 = 1;
    sp348.unk58 = 0;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 10.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk3C = 0;
    do { } while (0);
    sp348.unk59 = 5;
    sp348.unk5A = 0;
    sp348.unk5B = 0x10;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_6C[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x04000010;
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

    return dll_modgfx->Func1(&sp348, 0, 5, data_0, 4, data_34, TEXTABLE_5E, NULL);
}
