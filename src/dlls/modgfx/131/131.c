#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"
#include "sys/rand.h"

/*0x0*/ static u32 data_0 = 0x00000000;
/*0x4*/ static s16 data_4[] = {
    0, 0, 1000, 0, 0, 866, 0, 500, 11, 0, 866, 0, -500, 22, 0, 0,
    0, -1000, 32, 0, -866, 0, -500, 22, 0, -866, 0, 500, 11, 0, 0, 0,
    1000, 0, 0, 0, 500, 1000, 0, 4, 866, 500, 500, 11, 4, 866, 500, -500,
    22, 4, 0, 500, -1000, 32, 4, -866, 500, -500, 22, 4, -866, 500, 500, 11,
    4, 0, 500, 1000, 0, 4, 0, 6000, 1000, 0, 63, 866, 6000, 500, 11, 63,
    866, 6000, -500, 22, 63, 0, 6000, -1000, 32, 63, -866, 6000, -500, 22, 63, -866,
    6000, 500, 11, 63, 0, 6000, 1000, 0, 63, 0
};
/*0xD8*/ static s16 data_D8[] = {
    0, 1, 8, 0, 8, 7, 1, 2, 9, 1, 9, 8, 2, 3, 10, 2,
    10, 9, 3, 4, 11, 3, 11, 10, 4, 5, 12, 4, 12, 11, 5, 6,
    13, 5, 13, 12, 7, 8, 15, 7, 15, 14, 8, 9, 16, 8, 16, 15,
    9, 10, 17, 9, 17, 16, 10, 11, 18, 10, 18, 17, 11, 12, 19, 11,
    19, 18, 12, 13, 20, 12, 20, 19, 0, 1, 2, 3, 4, 5, 6, 0,
    7, 8, 9, 10, 11, 12, 13, 0, 14, 15, 16, 17, 18, 19, 20, 0,
    0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 17, 18, 19, 20
};
/*0x1B4*/ static s16 data_1B4[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 0
};
/*0x1E0*/ static s16 data_1E0[] = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
/*0x1FC*/ static s16 data_1FC[] = { 0, 50, 150, 50, 1, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_131_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_131_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_131_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0* temp;
    s32 pad[3];
    s32 i;

    temp = sp50;
    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.01f;
    temp->unk4.f[1] = 3.0f;
    temp->unk4.f[2] = 0.01f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 100.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0x124;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 2;
    temp->unk4.f[0] = 200.0f;
    temp->unk4.f[1] = 1.5f;
    temp->unk4.f[2] = 200.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0xE;
    temp->unk10 = data_1E0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -100.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 8;
    temp->unk4.f[0] = mathRnd(0x64, 0xFF);
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 255.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 8;
    temp->unk4.f[0] = mathRnd(0x64, 0xFF);
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 255.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x124;
    temp->unk10 = 0;
    temp->unk0 = 0x20000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0xE;
    temp->unk10 = data_1E0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 2.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B4;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.01f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.01f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 100.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;   

    temp->unk16 = 3;
    temp->unk14 = 0;
    temp->unk10 = 0;
    temp->unk0 = 0x80000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 400.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp350.unk4 = obj;
    sp350.unk44 = type;
    sp350.unk40 = 2;
    sp350.unk3C = 7;
    sp350.unk59 = 0xE;
    sp350.unk5B = 0x1E;
    sp350.unk58 = 0;
    sp350.unk5A = 0;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 0.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk38 = 1.0f;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_1FC[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x0C0104C0;
    if (sp350.unk54 & 1) {
        if (sp350.unk4 != NULL) {
            sp350.unk2C.f[0] += sp350.unk4->srt.transl.f[0];
            sp350.unk2C.f[1] += sp350.unk4->srt.transl.f[1];
            sp350.unk2C.f[2] += sp350.unk4->srt.transl.f[2];
        } else {
            sp350.unk2C.f[0] += transform->transl.f[0];
            sp350.unk2C.f[1] += transform->transl.f[1];
            sp350.unk2C.f[2] += transform->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp350, 0, 0x15, data_4, 0x18, data_D8, TEXTABLE_D9, NULL);
}
