#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, -2000, 0, 0, 0, 1000, 1000, 0, 32, 32, 707, 1000, -707, 0, 32, 0,
    1000, -1000, 32, 32, -707, 1000, -707, 0, 32, -1000, 1000, 0, 32, 32, -707, 1000,
    707, 0, 32, 0, 1000, 1000, 32, 32, 707, 1000, 707, 0, 32, 1000, 1000, 0,
    32, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0
};
/*0x168*/ static s16 data_168[] = {
    0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0,
    6, 7, 0, 7, 8, 0, 8, 9
};
/*0x198*/ static s16 data_198[] = { 0, 0 };
/*0x19C*/ static s16 data_19C[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 0
};
/*0x1B0*/ static s16 data_1B0[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};
/*0x1C4*/ static s16 data_1C4[] = {
    0, 90, 200, 90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// offset: 0x0 | ctor
void dll_153_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_153_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_153_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 0xA;
    temp->unk10 = data_1B0;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.1f;
    temp->unk4.f[1] = 1.2f;
    temp->unk4.f[2] = 1.1f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0xA;
    temp->unk10 = data_1B0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 8.0f;
    temp->unk4.f[1] = 72.0f;
    temp->unk4.f[2] = 5.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0xA;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_19C;
    temp->unk0 = 2;
    temp->unk4.f[0] = 32.1f;
    temp->unk4.f[1] = 1.2f;
    temp->unk4.f[2] = 32.1f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 1;
    temp->unk10 = data_198;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0xA;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0xA;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0xA;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0xA;
    temp->unk10 = data_1B0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp348.unk58 = 0;
    sp348.unk44 = type;
    sp348.unk4 = obj;
    sp348.unk40 = 1;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 0.0f;
    sp348.unk3C = 10;
    sp348.unk59 = 10;
    sp348.unk5A = 0;
    sp348.unk5B = 0x10;
    sp348.unk5D = temp - sp48;
    sp348.unk54 = 0x04000494;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_1C4[i]; }
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

    return dll_modgfx->Func1(&sp348, 0, 10, data_0, 8, data_168, TEXTABLE_1FD, NULL);
}
