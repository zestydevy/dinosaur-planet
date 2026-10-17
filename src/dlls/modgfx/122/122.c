#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    1000, 0, 0, 0, 0, 707, 0, -707, 15, 0, 0, 0, -1000, 31, 0, -707,
    0, -707, 47, 0, -1000, 0, 0, 63, 0, -707, 0, 707, 79, 0, 0, 0,
    1000, 95, 0, 707, 0, 707, 111, 0, 1000, 0, 0, 127, 0, 1000, 2000, 0,
    0, 15, 707, 2000, -707, 15, 15, 0, 2000, -1000, 31, 15, -707, 2000, -707, 47,
    15, -1000, 2000, 0, 63, 15, -707, 2000, 707, 79, 15, 0, 2000, 1000, 95, 15,
    707, 2000, 707, 111, 15, 1000, 2000, 0, 127, 15
};
/*0xB4*/ static s16 data_B4[] = {
    0, 1, 10, 0, 10, 9, 1, 2, 11, 1, 11, 10, 2, 3, 12, 2,
    12, 11, 3, 4, 13, 3, 13, 12, 4, 5, 14, 4, 14, 13, 5, 6,
    15, 5, 15, 14, 6, 7, 16, 6, 16, 15, 7, 8, 17, 7, 17, 16
};
/*0x114*/ static s16 data_114[] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 0
};
/*0x128*/ static s16 data_128[] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
16, 17, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0
};
/*0x160*/ static s16 data_160[] = { 0, 50, 0, 100, 0, 50, 0, 0 };
/*0x170*/ static u8 data_170[] = { 50, 150, 255, 50, 255, 150, 155, 100, 10, 255, 100, 130, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_122_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_122_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_122_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
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
