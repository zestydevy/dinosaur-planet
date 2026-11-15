#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    -230, 0, 0, 0, 15, 0, 0, 0, 127, 15, 230, 0, 0, 255, 15, -230,
    0, 1000, 0, 0, 0, 0, 1000, 127, 0, 230, 0, 1000, 255, 0
};
/*0x3C*/ static s16 data_3C[] = { 0, 4, 3, 0, 1, 4, 1, 2, 4, 2, 5, 4 };
/*0x54*/ static s16 data_54[] = { 1, 0, 4, 0, 0, 2, 3, 5 };
/*0x64*/ static s16 data_64[] = { 0, 1, 2, 3, 4, 5 };
/*0x70*/ static s16 data_70[] = { 0, 2, 3, 4, 5, 0 };
/*0x7C*/ static s16 data_7C[] = { 0, 6, 20, 26, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_162_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_162_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_162_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, f32* data) {
    ModgfxStruct sp348;
    ModgfxStruct_0 sp48[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s16 i;
    f32 var_fa0;

    var_fa0 = 1.0f;
    if (data != NULL) {
        var_fa0 = *data;
    }

    temp = sp48;
    temp->unk16 = 0;
    temp->unk14 = 5;
    temp->unk10 = data_70;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 1;
    temp->unk10 = data_54;
    temp->unk0 = 4;
    if (type == 1) {
        temp->unk4.f[0] = 155.0f;
    } else {
        temp->unk4.f[0] = 55.0f;
    }
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 6;
    temp->unk10 = data_64;
    temp->unk0 = 2;
    if (type == 1) {
        temp->unk4.f[0] = 0.15f * var_fa0;
        temp->unk4.f[1] = 0.15f * var_fa0;
        temp->unk4.f[2] = 0.15f * var_fa0;
    } else {
        temp->unk4.f[0] = 0.1f * var_fa0;
        temp->unk4.f[1] = 0.1f * var_fa0;
        temp->unk4.f[2] = 0.1f * var_fa0;
    }
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 6;
    temp->unk10 = data_64;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.5f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 6;
    temp->unk10 = data_64;
    temp->unk0 = 2;
    temp->unk4.f[0] = 4.0f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 25.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 6;
    temp->unk10 = data_64;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.5f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 6;
    temp->unk10 = data_64;
    temp->unk0 = 2;
    temp->unk4.f[0] = 8.0f;
    temp->unk4.f[1] = 8.0f;
    temp->unk4.f[2] = 1.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 6;
    temp->unk10 = data_64;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = -0.5f;
    temp->unk4.f[1] = 1.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 1;
    temp->unk10 = data_54;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp348.unk44 = type;
    sp348.unk4 = obj;
    sp348.unk58 = 0;
    sp348.unk2C.f[0] = 0.0f;
    sp348.unk2C.f[1] = 0.0f;
    sp348.unk2C.f[2] = 0.0f;
    sp348.unk20 = 0.0f;
    sp348.unk24 = 0.0f;
    sp348.unk28 = 0.0f;
    sp348.unk40 = 1;
    sp348.unk3C = 0;
    sp348.unk59 = 6;
    sp348.unk5A = 0;
    sp348.unk5B = 0;
    sp348.unk38 = 2.0f;
    sp348.unk5D = temp - sp48;
    for (i = 0; i < 7; i++) { sp348.unk46[i] = data_7C[i]; }
    sp348.unk0 = sp48;
    sp348.unk54 = flags | 0x04000400;
    if (sp348.unk54 & 1) {
        if ((sp348.unk4 != NULL) && (transform != NULL)) {
            sp348.unk2C.f[0] += sp348.unk4->globalPosition.f[0] + transform->transl.f[0];
            sp348.unk2C.f[1] += sp348.unk4->globalPosition.f[1] + transform->transl.f[1];
            sp348.unk2C.f[2] += sp348.unk4->globalPosition.f[2] + transform->transl.f[2];
        } else if (sp348.unk4 != NULL) {
            sp348.unk2C.f[0] += sp348.unk4->globalPosition.f[0];
            sp348.unk2C.f[1] += sp348.unk4->globalPosition.f[1];
            sp348.unk2C.f[2] += sp348.unk4->globalPosition.f[2];
        } else if (transform != NULL) {
            sp348.unk2C.f[0] += transform->transl.f[0];
            sp348.unk2C.f[1] += transform->transl.f[1];
            sp348.unk2C.f[2] += transform->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp348, 0, 6, data_0, 4, data_3C, TEXTABLE_3C, NULL);
}
