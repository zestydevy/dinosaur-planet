#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    -800, 500, -800, 8, 31, 800, 500, -800, 120, 31, 800, 500, 800, 8, 31, -800,
    500, 800, 120, 31, -1000, 0, -1000, 8, 0, 1000, 0, -1000, 120, 0, 1000, 0,
    1000, 8, 0, -1000, 0, 1000, 120, 0
};
/*0x50*/ static s16 data_50[] = {
    0, 1, 5, 0, 5, 4, 1, 2, 6, 1, 6, 5, 2, 3, 7, 2,
    7, 6, 3, 0, 4, 3, 4, 7, 0, 1, 2, 3
};
/*0x88*/ static s16 data_88[] = { 4, 5, 6, 7 };
/*0x90*/ static s16 data_90[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
/*0xA0*/ static s16 data_A0[] = { 0, 790, 10, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_165_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_165_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_165_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp340;
    ModgfxStruct_0 sp40[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s16 i;

    temp = sp40;

    temp->unk16 = 0;
    temp->unk14 = 8;
    temp->unk10 = data_90;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.014f;
    temp->unk4.f[1] = 0.03f;
    temp->unk4.f[2] = 0.014f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 4;
    temp->unk10 = data_88;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 4;
    temp->unk10 = data_90;
    temp->unk0 = 8;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 85.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 80.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 8;
    temp->unk10 = data_90;
    temp->unk0 = 2;
    temp->unk4.f[0] = 100.0f;
    temp->unk4.f[1] = 100.0f;
    temp->unk4.f[2] = 100.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -80.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 8;
    temp->unk10 = data_90;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp340.unk58 = 0;
    sp340.unk4 = obj;
    sp340.unk44 = type;
    sp340.unk2C.f[0] = 0.0f;
    sp340.unk2C.f[1] = 0.0f;
    sp340.unk2C.f[2] = 0.0f;
    sp340.unk20 = 0.0f;
    sp340.unk24 = 0.0f;
    sp340.unk28 = 0.0f;
    sp340.unk38 = 2.0f;
    sp340.unk40 = 1;
    sp340.unk3C = 0;
    sp340.unk59 = 8;
    sp340.unk5A = 0;
    sp340.unk5B = 0x3C;
    sp340.unk5D = temp - sp40;
    for (i = 0; i < 7; i++) { sp340.unk46[i] = data_A0[i]; }
    if (1) {
        sp340.unk0 = sp40;
    }
    sp340.unk54 = 0x04002400;
    return dll_modgfx->Func1(&sp340, 0, 8, data_0, 8, data_50, TEXTABLE_46, NULL);
}
