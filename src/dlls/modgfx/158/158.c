#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/rand.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = { 0, 230, 1300, 0, 31, 0, -230, 1300, 31, 31, 0, 0, 0, 15, 16, 0 };
/*0x20*/ static s16 data_20[] = { 0, 1, 2, 0, 0, 0, 1, 2 };
/*0x30*/ static s16 data_30[] = { 0, 1, 2, 0 };
/*0x38*/ static s16 data_38[] = { 0, 140, 140, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_158_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_158_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_158_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    s32 i;
    f32 sp3C;
    f32 temp_ft3;

    temp = sp50;
    if (type == 0) {
        temp->unk16 = 0;
        temp->unk14 = 3;
        temp->unk10 = data_30;
        temp->unk0 = 8;
        temp->unk4.f[0] = mathRnd(0, 105) + 140;
        temp->unk4.f[1] = mathRnd(0, 105) + 140;
        temp->unk4.f[2] = mathRnd(0, 30) + 225;
        temp++;
    } else if (type == 1) {
        temp->unk16 = 0;
        temp->unk14 = 3;
        temp->unk10 = data_30;
        temp->unk0 = 8;
        temp->unk4.f[0] = mathRnd(0, 30) + 225;
        temp->unk4.f[1] = mathRnd(0, 105) + 140;
        temp->unk4.f[2] = mathRnd(0, 65) + 120;
        temp++;
    }

    sp3C = mathRnd(0, 0xFFFE);
    temp_ft3 = mathRnd(-3000, -12000);
    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = temp_ft3;
    temp->unk4.f[2] = sp3C;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 3;
    temp->unk10 = data_30;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 3;
    temp->unk10 = data_30;
    temp->unk0 = 2;
    temp->unk4.f[0] = 1.0f;
    temp->unk4.f[1] = (mathRnd(0, 50) * 0.01f) + 0.5f;
    temp->unk4.f[2] = (mathRnd(0, 20) * 0.01f) + 0.8f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 3;
    temp->unk10 = data_30;
    temp->unk0 = 4;
    if (mathRnd(0, 10) == 0) {
        temp->unk4.f[0] = mathRnd(0, 30) + 145.0f;
    } else {
        temp->unk4.f[0] = mathRnd(0, 10) + 25.0f;
    }
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0;
    temp->unk10 = 0;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = mathRnd(0, 0xFFFE);
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 3;
    temp->unk10 = data_30;
    temp->unk0 = 2;
    temp->unk4.f[0] = 10.0f;
    temp->unk4.f[1] = 12.0f;
    temp->unk4.f[2] = 21.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0;
    temp->unk10 = 0;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = mathRnd(0, 0xFFFE);
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 3;
    temp->unk10 = data_30;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 3;
    temp->unk10 = data_30;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.1f;
    temp->unk4.f[1] = 4.0f;
    temp->unk4.f[2] = 0.05f;
    temp++;

    sp350.unk58 = 0;
    sp350.unk4 = obj;
    sp350.unk44 = type;
    sp350.unk2C.f[0] = 0.0f;
    if (type == 0) {
        sp350.unk2C.f[1] = 0.0f;
    } else if (type == 1) {
        sp350.unk2C.f[1] = 200.0f;
    }
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk40 = 1;
    sp350.unk59 = 3;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk38 = 4.0f;
    sp350.unk3C = 0;
    sp350.unk5A = 0;
    sp350.unk5B = 0;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_38[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x04000410;
    if (sp350.unk54 & 1) {
        if ((sp350.unk4 != NULL) && (transform != NULL)) {
            sp350.unk2C.f[0] += sp350.unk4->globalPosition.f[0] + transform->transl.f[0];
            sp350.unk2C.f[1] += sp350.unk4->globalPosition.f[1] + transform->transl.f[1];
            sp350.unk2C.f[2] += sp350.unk4->globalPosition.f[2] + transform->transl.f[2];
        } else if (sp350.unk4 != NULL) {
            sp350.unk2C.f[0] += sp350.unk4->globalPosition.f[0];
            sp350.unk2C.f[1] += sp350.unk4->globalPosition.f[1];
            sp350.unk2C.f[2] += sp350.unk4->globalPosition.f[2];
        } else if (transform != NULL) {
            sp350.unk2C.f[0] += transform->transl.f[0];
            sp350.unk2C.f[1] += transform->transl.f[1];
            sp350.unk2C.f[2] += transform->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp350, 0, 3, data_0, 1, data_20, TEXTABLE_26A, NULL);
}
