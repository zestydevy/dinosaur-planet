#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, 0, 1000, 0, 0, 866, 0, 500, 11, 0, 866, 0, -500, 22, 0, 0,
    0, -1000, 32, 0, -866, 0, -500, 42, 0, -866, 0, 500, 53, 0, 0, 0,
    1000, 64, 0, 0, 6000, 1000, 0, 31, 866, 6000, 500, 11, 31, 866, 6000, -500,
    22, 31, 0, 6000, -1000, 32, 31, -866, 6000, -500, 42, 31, -866, 6000, 500, 53,
    31, 0, 6000, 1000, 64, 31
};
/*0x8C*/ static s16 data_8C[] = {
    0, 1, 8, 0, 8, 7, 1, 2, 9, 1, 9, 8, 2, 3, 10, 2,
    10, 9, 3, 4, 11, 3, 11, 10, 4, 5, 12, 4, 12, 11, 5, 6,
    13, 5, 13, 12
};
/*0xD4*/ static s16 data_D4[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
/*0xF0*/ static s16 data_F0[] = { 0, 1, 2, 3, 4, 5, 6, 0 };
/*0x100*/ static s16 data_100[] = { 7, 8, 9, 10, 11, 12, 13, 0 };
/*0x110*/ static s16 data_110[] = { 0, 20, 40, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_136_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_136_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_136_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 0xC8;
    temp->unk10 = NULL;
    temp->unk0 = 0x800000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    if (transform != NULL) {
        temp->unk4.f[2] = transform->yaw;
    } else {
        temp->unk4.f[2] = 0.0f;
    }
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
    temp->unk4.f[0] = 0.3f;
    temp->unk4.f[1] = 0.7f;
    temp->unk4.f[2] = 0.3f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 2;
    if (transform != NULL) {
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 0.5f;
        temp->unk4.f[2] = 1.0f;
    } else {
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 0.5f;
        temp->unk4.f[2] = 1.0f;
    }
    temp++;
    
    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_100;
    temp->unk0 = 2;
    if (transform != NULL) {
        temp->unk4.f[0] = transform->roll * 3.5f * 0.01f;
        temp->unk4.f[1] = transform->roll * 2.0f * 0.01f;
        temp->unk4.f[2] = transform->roll * 3.5f * 0.01f;
    } else {
        temp->unk4.f[0] = 3.5f;
        temp->unk4.f[1] = 2.0f;
        temp->unk4.f[2] = 3.5f;
    }
    temp++;
    temp->unk16 = 1;
    temp->unk14 = 0x58;
    temp->unk10 = 0;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -1.5f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 2;
    temp->unk4.f[0] = 3.0f;
    temp->unk4.f[1] = 0.1f;
    temp->unk4.f[2] = 3.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0xE;
    temp->unk10 = data_D4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -3.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 7;
    temp->unk10 = data_F0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    // @bug missing temp++ here?

    sp348.unk58 = 0;
    sp348.unk4 = obj;
    sp348.unk44 = type;
    if (transform != NULL) {
        sp348.unk2C.f[0] = transform->transl.f[0];
        sp348.unk2C.f[1] = transform->transl.f[1];
        sp348.unk2C.f[2] = transform->transl.f[2];
    } else {
        sp348.unk2C.f[0] = 0.0f;
        sp348.unk2C.f[1] = 0.0f;
        sp348.unk2C.f[2] = 0.0f;
    }
    sp348.unk40 = 1;
    sp348.unk59 = 0xE;
    sp348.unk5B = 0x10;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk3C = 0;
    sp348.unk5A = 0;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_110[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x04000400;
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

    return dll_modgfx->Func1(&sp348, 0, 0xE, data_0, 0xC, data_8C, TEXTABLE_34, NULL);
}
