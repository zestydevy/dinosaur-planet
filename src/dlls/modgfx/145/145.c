#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, 1000, 0, 0, 0, 866, 500, 0, 8, 0, 866, -500, 0, 15, 0, 0,
    -1000, 0, 31, 0, -866, -500, 0, 15, 0, -866, 500, 0, 8, 0, 0, 1000,
    0, 0, 0, 0, 1000, 3000, 0, 127, 866, 500, 3000, 8, 127, 866, -500, 3000,
    15, 127, 0, -1000, 3000, 31, 127, -866, -500, 3000, 15, 127, -866, 500, 3000, 8,
    127, 0, 1000, 3000, 0, 127, 0, 1000, 6000, 0, 255, 866, 500, 6000, 8, 255,
    866, -500, 6000, 15, 255, 0, -1000, 6000, 31, 255, -866, -500, 6000, 15, 255, -866,
    500, 6000, 8, 255, 0, 1000, 6000, 0, 255, 0
};
/*0xD4*/ static s16 data_D4[] = {
    0, 8, 1, 0, 7, 8, 1, 9, 2, 1, 8, 9, 2, 10, 3, 2,
    9, 10, 3, 11, 4, 3, 10, 11, 4, 12, 5, 4, 11, 12, 5, 13,
    6, 5, 12, 13, 7, 15, 8, 7, 14, 15, 8, 16, 9, 8, 15, 16,
    9, 17, 10, 9, 16, 17, 10, 18, 11, 10, 17, 18, 11, 19, 12, 11,
    18, 19, 12, 20, 13, 12, 19, 20, 0, 1, 2, 3, 4, 5, 6, 0,
    7, 8, 9, 10, 11, 12, 13, 0, 14, 15, 16, 17, 18, 19, 20, 0,
    0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 17, 18, 19, 20
};
/*0x1B0*/ static s16 data_1B0[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20
};
/*0x1F8*/ static s16 data_1F8[] = { 0, 36, 100, 6, 0, 0, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_145_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_145_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
extern s16 data_1FA;
extern s16 data_1FC;

s32 dll_145_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;
    f32 var_fa1;
    s16 var_v1;

    var_fa1 = -12.0f;
    if (type == 0 || type == 2 || type == 0x1E) {
        data_1F8[1] = 0x24;
    } else if (type == 1 || type == 3) {
        var_fa1 *= -1.0f;
        data_1F8[1] = 6;
        data_1F8[3] = 0x32;
    }

    temp = sp50;
    temp->unk16 = 0;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    if (type == 0 || type == 2) {
        temp->unk16 = 0;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.35f;
        temp->unk4.f[1] = 0.35f;
        temp->unk4.f[2] = 0.01f;
        temp++;
    } else if (type == 0xE) {
        temp->unk16 = 0;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.2f;
        temp->unk4.f[1] = 0.2f;
        temp->unk4.f[2] = 5.0f;
        temp++;
    } else if (type == 0x1E) {
        temp->unk16 = 0;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.45f;
        temp->unk4.f[1] = 0.45f;
        temp->unk4.f[2] = 0.01f;
        temp++;
    } else {
        temp->unk16 = 0;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.35f;
        temp->unk4.f[1] = 0.35f;
        temp->unk4.f[2] = 3.0f;
        temp++;
    }

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    if ((type == 0) || (type == 2)) {
        temp->unk16 = 1;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 1.0f;
        temp->unk4.f[2] = 300.0f;
        temp++;
    } else if (type == 0x1E) {
        temp->unk16 = 1;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 1.0f;
        temp->unk4.f[2] = 460.0f;
        temp++;
    }

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 4.0f;
    temp->unk4.f[1] = var_fa1;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 4.0f;
    temp->unk4.f[1] = var_fa1;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 4.0f;
    temp->unk4.f[1] = var_fa1;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    if ((type == 1) || (type == 3)) {
        temp->unk16 = 3;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 1.0f;
        temp->unk4.f[2] = 0.01f;
        temp++;
    }

    sp350.unk4 = obj;
    sp350.unk58 = 0;
    sp350.unk44 = type;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 0.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk3C = 7;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk38 = 1.0f;
    sp350.unk59 = 0xE;
    sp350.unk40 = 2;
    sp350.unk5A = 0;
    sp350.unk5B = 0xA;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_1F8[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x0C010480;
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

    if (type == 0x1E) {
        var_v1 = dll_modgfx->Func1(&sp350, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_3E9, NULL);
    } else if (type == 2 || type == 3) {
        var_v1 = dll_modgfx->Func1(&sp350, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_23D, NULL);
    } else if (type == 0xA || type == 0xB || type == 0xC || type == 0xD || type == 0xE) {
        var_v1 = dll_modgfx->Func1(&sp350, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_2E, NULL);
    } else {
        var_v1 = dll_modgfx->Func1(&sp350, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_D9, NULL);
    }

    return var_v1;
}
