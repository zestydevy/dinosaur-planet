#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    400, 0, 400, 31, 31, 1107, -1107, 400, 0, 31, 0, -400, 400, 31, 31, -1107,
    -1107, 400, 0, 31, -400, 0, 400, 31, 31, -1107, 1107, 400, 0, 31, 0, 400,
    400, 31, 31, 1107, 1107, 400, 0, 31, 0, 0, 0, 15, 0, 0
};
/*0x5C*/ static s16 data_5C[] = {
    1000, 0, 400, 31, 31, 707, -707, 400, 0, 31, 0, -1000, 400, 31, 31, -707,
    -707, 400, 0, 31, -1000, 0, 400, 31, 31, -707, 707, 400, 0, 31, 0, 1000,
    400, 31, 31, 707, 707, 400, 0, 31, 0, 0, 0, 15, 0, 0
};
/*0xB8*/ static s16 data_B8[] = {
    0, 1, 8, 1, 2, 8, 2, 3, 8, 3, 4, 8, 4, 5, 8, 5,
    6, 8, 6, 7, 8, 7, 0, 8
};
/*0xE8*/ static s16 data_E8[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0 };
/*0xFC*/ static s16 data_FC[] = { 8, 0, 0, 1, 2, 3, 4, 5, 6, 7 };
/*0x110*/ static s16 data_110[] = { 0, 60, 30, 1, 1, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_143_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_143_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_143_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 i;
    s16 var_v1;

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 0x8C;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000000;
    temp->unk4.f[0] = 999.0f;
    temp->unk4.f[1] = 94.0f;
    temp->unk4.f[2] = 95.0f;
    temp++;

    if (type != 2) {
        temp->unk16 = 0;
        temp->unk14 = 9;
        temp->unk10 = data_E8;
        temp->unk0 = 0x80;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 16383.0f;
        temp++;
    }

    if (type == 0) {
        temp->unk16 = 0;
        temp->unk14 = 8;
        temp->unk10 = &data_FC[2];
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.7f;
        temp->unk4.f[1] = 0.7f;
        temp->unk4.f[2] = 3.3333333f;
        temp++;
    } else {
        temp->unk16 = 0;
        temp->unk14 = 8;
        temp->unk10 = &data_FC[2];
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.36666667f;
        temp->unk4.f[1] = 0.36666667f;
        temp->unk4.f[2] = 5.0f;
        temp++;
    }

    if (type == 0) {
        temp->unk16 = 1;
        temp->unk14 = 8;
        temp->unk10 = data_E8;
        temp->unk0 = 2;
        temp->unk4.f[0] = 3.0f;
        temp->unk4.f[1] = 3.0f;
        temp->unk4.f[2] = 3.0f;
        temp++;
    } else {
        temp->unk16 = 1;
        temp->unk14 = 8;
        temp->unk10 = data_E8;
        temp->unk0 = 2;
        temp->unk4.f[0] = 3.0f;
        temp->unk4.f[1] = 3.0f;
        temp->unk4.f[2] = 3.0f;
        temp++;
    }

    if (type == 0) {
        temp->unk16 = 1;
        temp->unk14 = 9;
        temp->unk10 = data_E8;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = -400.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 1;
        temp->unk14 = 1;
        temp->unk10 = data_FC;
        temp->unk0 = 0x4000;
        temp->unk4.f[0] = -4.0f;
        temp->unk4.f[1] = -4.0f;
        temp->unk4.f[2] = 0.0f;
    } else if (type == 1) {
        temp->unk16 = 1;
        temp->unk14 = 9;
        temp->unk10 = data_E8;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 400.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }

    if (type == 0) {
        temp->unk16 = 2;
        temp->unk14 = 9;
        temp->unk10 = data_E8;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = -400.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 2;
        temp->unk14 = 1;
        temp->unk10 = data_FC;
        temp->unk0 = 0x4000;
        temp->unk4.f[0] = -4.0f;
        temp->unk4.f[1] = -4.0f;
        temp->unk4.f[2] = 0.0f;
        // @bug! missing temp++
    } else if (type == 1) {
        temp->unk16 = 2;
        temp->unk14 = 9;
        temp->unk10 = data_E8;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 400.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }

    temp->unk16 = 2;
    temp->unk14 = 9;
    temp->unk10 = data_E8;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x20000000;
    temp->unk4.f[0] = 999.0f;
    temp->unk4.f[1] = 94.0f;
    temp->unk4.f[2] = 95.0f;
    temp++;

    sp348.unk4 = obj;
    sp348.unk44 = type;
    sp348.unk40 = 1;
    sp348.unk59 = 9;
    sp348.unk5B = 0x20;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk3C = 0;
    sp348.unk5A = 0;
    sp348.unk38 = 1.0f;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_110[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x04000000;
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

    if (type == 0) {
        sp348.unk58 = 0;
        var_v1 = dll_modgfx->Func1(&sp348, 0, 9, data_0, 8, data_B8, TEXTABLE_156, NULL);
    } else {
        sp348.unk58 = 0;
        var_v1 = dll_modgfx->Func1(&sp348, 0, 9, data_5C, 8, data_B8, TEXTABLE_8A, NULL);
    }

    return var_v1;
}
