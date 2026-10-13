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
/*0x110*/ static s16 data_110[] = { 0x0000, 0x0104, 0x003c, 0x003c, 0x0001, 0x0104, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_115_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_115_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_115_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    CampFire_Setup* objSetup;
    ModgfxStruct_0 *temp;
    s32 pad[2];
    u8 sp47;
    s32 i;

    if (type == 1) {
        data_110[1] = 0;
    }

    objSetup = (CampFire_Setup*)obj->setup;
    sp47 = objSetup->scale;
    if (type == 2) {
        for (i = 0; i < 14; i++) {
            if (data_0[i * 5 + 0] > 0) {
                data_0[i * 5 + 0] += mathRnd(0, 0x320);
            } else if (data_0[i * 5 + 0] < 0) {
                data_0[i * 5 + 0] -= mathRnd(0, 0x320);
            }
            if (data_0[i * 5 + 1] > 0) {
                data_0[i * 5 + 0] += mathRnd(0, 0x12C);
            } else if (data_0[i * 5 + 1] < 0) {
                data_0[i * 5 + 0] -= mathRnd(0, 0x12C);
            }
            if (data_0[i * 5 + 2] > 0) {
                data_0[i * 5 + 0] += mathRnd(0, 0x320);
            } else if (data_0[i * 5 + 2] < 0) {
                data_0[i * 5 + 0] -= mathRnd(0, 0x320);
            }
        }
    }

    temp = sp48;
    if (type == 2) {
        temp->unk16 = 0;
        temp->unk14 = 7;
        temp->unk10 = data_F0;
        temp->unk0 = 8;
        temp->unk4.f[0] = 100.0f;
        temp->unk4.f[1] = 100.0f;
        temp->unk4.f[2] = 100.0f;
        temp++;
        
        temp->unk16 = 0;
        temp->unk14 = 7;
        temp->unk10 = data_100;
        temp->unk0 = 8;
        temp->unk4.f[0] = 200.0f;
        temp->unk4.f[1] = 200.0f;
        temp->unk4.f[2] = 200.0f;
        temp++;
    } else {
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
    }

    temp->unk16 = 0;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    if (type != 3 || transform == NULL) {
        temp->unk16 = 0;
        temp->unk14 = 7;
        temp->unk10 = data_100;
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.725f;
        temp->unk4.f[1] = 1.2f;
        temp->unk4.f[2] = 0.725f;
        temp++;
        
        temp->unk16 = 0;
        temp->unk14 = 7;
        temp->unk10 = data_F0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.35f;
        temp->unk4.f[1] = 1.0f;
        temp->unk4.f[2] = 0.35f;
        temp++;
    } else {
        temp->unk16 = 0;
        temp->unk14 = 7;
        temp->unk10 = data_100;
        temp->unk0 = 2;
        temp->unk4.f[0] = transform->scale * 0.725f;
        temp->unk4.f[1] = transform->scale * 1.2f;
        temp->unk4.f[2] = transform->scale * 0.725f;
        temp++;
        
        temp->unk16 = 0;
        temp->unk14 = 7;
        temp->unk10 = data_F0;
        temp->unk0 = 2;
        temp->unk4.f[0] = transform->scale * 0.35f;
        temp->unk4.f[1] = transform->scale * 1.0f;
        temp->unk4.f[2] = transform->scale * 0.35f;
        temp++;
    }
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

    temp->unk16 = 1;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 20.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.7f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 20.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.7f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 20.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.7f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 1;
    temp->unk10 = 0;
    temp->unk0 = 0x2000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 20.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.7f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp348.unk58 = 0;
    sp348.unk4 = obj;
    sp348.unk44 = type;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 4.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    if (sp47 != 0) {
        sp348.unk38 = sp47 * 0.1f;
    } else {
        sp348.unk38 = 1.0f;
    }
    sp348.unk40 = 1;
    sp348.unk59 = 0xE;
    sp348.unk5B = 0x1E;
    sp348.unk5D = temp - sp48;
    sp348.unk3C = 0;
    sp348.unk5A = 0;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_110[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x040000C0;
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

    return dll_modgfx->Func1(&sp348, 0, 0xE, data_0, 0xC, data_8C, TEXTABLE_40, NULL);
}
