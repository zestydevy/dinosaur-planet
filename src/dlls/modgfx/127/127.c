#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, 0, 0, 15, 0, 333, 40, 0, 0, 11, 235, 40, -235, 31, 11, 1000,
    0, 0, 0, 31, 853, 0, -353, 15, 31, 707, 0, -707, 31, 31, 0, 0,
    0, 15, 0, 0, 40, -333, 0, 11, -234, 40, -235, 31, 11, 0, 0, -1000,
    0, 31, -352, 0, -853, 15, 31, -706, 0, -707, 31, 31, 0, 0, 0, 15,
    0, -333, 40, 0, 0, 11, -235, 40, 234, 31, 11, -1000, 0, 0, 0, 31,
    -853, 0, 352, 15, 31, -707, 0, 706, 31, 31, 0, 0, 0, 15, 0, 0,
    40, 333, 0, 11, 234, 40, 235, 31, 11, 0, 0, 1000, 0, 31, 352, 0,
    853, 15, 31, 706, 0, 707, 31, 31
};
/*0xF0*/ static s16 data_F0[] = {
    0, 2, 1, 1, 4, 3, 1, 2, 4, 2, 5, 4, 6, 8, 7, 7,
    10, 9, 7, 8, 10, 8, 11, 10, 12, 14, 13, 13, 16, 15, 13, 14,
    16, 14, 17, 16, 18, 19, 20, 19, 22, 21, 19, 20, 22, 20, 23, 22
};
/*0x150*/ static s16 data_150[] = {
    0, 6, 12, 18
};
/*0x158*/ static s16 data_158[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23
};
/*0x188*/ static s16 data_188[] = { 1, 2, 7, 8, 13, 14, 19, 20 };
/*0x198*/ static s16 data_198[] = { 3, 4, 5, 9, 10, 11, 15, 16, 17, 21, 22, 23 };
/*0x1B0*/ static s16 data_1B0[] = { 0, 24, 24, 24, 24, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_127_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_127_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_127_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 0x18;
    temp->unk10 = data_158;
    temp->unk0 = 2;
    temp->unk4.f[0] = 3.0f;
    temp->unk4.f[1] = 16.0f;
    temp->unk4.f[2] = 3.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x18;
    temp->unk10 = data_158;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x18;
    temp->unk10 = data_158;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x18;
    temp->unk10 = data_158;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 8;
    temp->unk10 = data_188;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 155.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0xC;
    temp->unk10 = data_198;
    temp->unk0 = 8;
    temp->unk4.f[0] = 235.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0xA4;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x14;
    temp->unk10 = NULL;
    temp->unk0 = 0x800000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x11;
    temp->unk10 = NULL;
    temp->unk0 = 0x800000;
    temp->unk4.f[0] = 40.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 1;
    temp->unk10 = NULL;
    temp->unk0 = 0x02008000;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 155.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 10.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 4;
    temp->unk10 = data_150;
    temp->unk0 = 4;
    temp->unk4.f[0] = 85.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 8;
    temp->unk10 = data_188;
    temp->unk0 = 4;
    temp->unk4.f[0] = 25.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x18;
    temp->unk10 = data_158;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -0.6f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x9C;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 4;
    temp->unk10 = data_150;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 8;
    temp->unk10 = data_188;
    temp->unk0 = 4;
    temp->unk4.f[0] = 155.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x18;
    temp->unk10 = data_158;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -0.6f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 30.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 8;
    temp->unk10 = data_188;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0xC;
    temp->unk10 = data_198;
    temp->unk0 = 4;
    temp->unk4.f[0] = 115.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x18;
    temp->unk10 = data_158;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -0.6f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0xC;
    temp->unk10 = data_198;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0x18;
    temp->unk10 = data_158;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -0.6f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x02008000;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 155.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 200.0f;
    temp++;


    // BUG! This is at index 0x1E but requires this code to match
    temp = &sp48[0x14];
    
    sp348.unk44 = type;
    sp348.unk4 = obj;
    sp348.unk40 = 1;
    sp348.unk58 = 0;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk38 = 1.0f;
    sp348.unk3C = 0;
    sp348.unk54 = 0x04000084;
    do { } while (0);
    sp348.unk59 = 0x18;
    sp348.unk5A = 0;
    sp348.unk5B = 0x10;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_1B0[i]; }
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

    return dll_modgfx->Func1(&sp348,0, 0x18, data_0, 0x10, data_F0, TEXTABLE_48, NULL);
}
