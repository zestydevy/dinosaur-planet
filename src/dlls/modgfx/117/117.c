#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"
#include "dlls/objects/common/campfire.h"

/*0x0*/ static s16 data_0[] = {
    0, 0, 1000, 0, 0, 866, 200, 500, 0, 10, 866, 40, -500, 0, 21, 0,
    150, -1000, 0, 31, -866, 90, -500, 0, 42, -866, 10, 500, 0, 52, 0, 150,
    1000, 0, 63, 0, 6400, 1000, 63, 0, 866, 6300, 500, 63, 10, 866, 6200, -500,
    63, 21, 0, 6450, -1000, 63, 31, -866, 6400, -500, 63, 42, -866, 6380, 500, 63,
    52, 0, 6440, 1000, 63, 63
};
/*0x8C*/ static s16 data_8C[] = {
    0, 1, 8, 0, 8, 7, 1, 2, 9, 1, 9, 8, 2, 3, 10, 2,
    10, 9, 3, 4, 11, 3, 11, 10, 4, 5, 12, 4, 12, 11, 5, 6,
    13, 5, 13, 12
};
/*0xD4*/ static s16 data_D4[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
/*0xF0*/ static s16 data_F0[] = { 0, 1, 2, 3, 4, 5, 6, 0 };
/*0x100*/ static s16 data_100[] = { 7, 8, 9, 10, 11, 12, 13, 0 };
/*0x110*/ static s16 data_110[] = { 0, 260, 60, 1, 260, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_117_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_117_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_117_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
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
