#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"
#include "sys/rand.h"

/*0x0*/ static s16 data_0[] = {
    1000, 0, 400, 31, 31, 707, -707, 400, 0, 31, 0, -1000, 400, 31, 31, -707,
    -707, 400, 0, 31, -1000, 0, 400, 31, 31, -707, 707, 400, 0, 31, 0, 1000,
    400, 31, 31, 707, 707, 400, 0, 31, 0, 0, -1100, 15, 0, 0
};
/*0x5C*/ static s16 data_5C[] = {
    0, 1, 8, 1, 2, 8, 2, 3, 8, 3, 4, 8, 4, 5, 8, 5,
    6, 8, 6, 7, 8, 7, 0, 8
};
/*0x8C*/ static s16 data_8C[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 8, 0 };
/*0xA4*/ static s16 data_A4[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
/*0xB4*/ static s16 data_B4[] = { 0, 2, 4, 6 };
/*0xBC*/ static s16 data_BC[] = { 0, 50, 30, 1, 1, 0, 0, 0, 0, 0};

// offset: 0x0 | ctor
void dll_137_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_137_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_137_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 i;
    s16 var_t1;
    f32 temp_fa0;
    f32 temp_fv1;

    var_t1 = 0;
    temp = sp50;
    if (type == 0) {
        temp->unk16 = 0;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x80;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 16383.0f;
        temp++;
        temp->unk16 = 0;
        temp->unk14 = 8;
        temp->unk10 = data_8C;
        temp->unk0 = 2;
        temp->unk4.f[0] = 5.2f;
        temp->unk4.f[1] = 5.2f;
        temp->unk4.f[2] = 40.0f;
        temp++;
    } else if (type == 1) {
        data_BC[1] = 0x50;
        data_BC[2] = 0x118;
        temp->unk16 = 0;
        temp->unk14 = 0x69;
        temp->unk10 = NULL;
        temp->unk0 = 0x01800000;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 8;
        temp->unk10 = data_8C;
        temp->unk0 = 2;
        var_t1 = 0;
        temp_fv1 = mathRnd(0, 0xC) * 0.05f;
        temp_fa0 = 3.5f + temp_fv1;
        temp->unk4.f[0] = temp_fa0;
        temp->unk4.f[1] = temp_fa0;
        temp->unk4.f[2] = 20.0f + temp_fv1;
        temp++;

        temp->unk16 = 0; // unk2E
        temp->unk14 = 9; // unk2C
        temp->unk10 = data_8C; // unk28
        temp->unk0 = 0x80; // unk18
        temp->unk4.f[0] = 0.0f; // unk1C
        temp->unk4.f[1] = 0.0f; // unk20
        temp->unk4.f[2] = 32676.0f;
        temp++;

        temp->unk16 = 0; // unk46
        temp->unk14 = 8; // unk44
        temp->unk10 = data_A4; // unk40
        temp->unk0 = 4; // unk30
        temp->unk4.f[0] = 100.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else if (type == 2) {
        data_BC[1] = 0x50;
        data_BC[2] = 0x50;
        temp->unk16 = 0;
        temp->unk14 = 0x1FC;
        temp->unk10 = NULL;
        temp->unk0 = 0x01800000;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 8;
        temp->unk10 = data_8C;
        temp->unk0 = 2;
        temp_fv1 = mathRnd(0, 0xC) * 0.05f;
        temp_fa0 = 1.2f + temp_fv1;
        temp->unk4.f[0] = temp_fa0;
        temp->unk4.f[1] = temp_fa0;
        temp->unk4.f[2] = 12.0f + temp_fv1;
        temp++;

        var_t1 = 0;
        temp->unk16 = 0; // unk2E
        temp->unk14 = 0x8C; // unk2C
        temp->unk10 = NULL; // unk28
        temp->unk0 = 0x20000000; // unk18
        temp->unk4.f[0] = 999.0f; // unk1C
        temp->unk4.f[1] = 96.0f; // unk20
        temp->unk4.f[2] = 97.0f; // unk24
        temp++;

        temp->unk16 = 0; // unk46
        temp->unk14 = 9; // unk44
        temp->unk10 = data_8C; // unk40
        temp->unk0 = 0x80; // unk30
        temp->unk4.f[0] = 0.0f; // unk34
        temp->unk4.f[1] = 0.0f; // unk38
        temp->unk4.f[2] = 32676.0f;
        temp++;
    }
    if (type == 0) {
        temp->unk16 = 1;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x4000;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 1;
        temp->unk14 = 8;
        temp->unk10 = data_8C;
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.5f;
        temp->unk4.f[1] = 0.5f;
        temp->unk4.f[2] = 0.5f;
        temp++;
    } else if (type == 1) {
        temp->unk16 = 1;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x4000;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = -2.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 1;
        temp->unk14 = 0x8F;
        temp->unk10 = NULL;
        temp->unk0 = 0x01800000;
        temp->unk4.f[0] = 12.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 4;
        temp->unk10 = data_B4;
        temp->unk0 = 2;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 1.0f;
        temp->unk4.f[2] = 2.0f;
        temp++;
    } else if (type == 2) {
        temp->unk16 = 1;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x4000;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 1;
        temp->unk14 = 0x1FD;
        temp->unk10 = NULL;
        temp->unk0 = 0x01800000;
        temp->unk4.f[0] = 2.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }
    if (type == 0) {
        temp->unk16 = 1;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 400.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else if (type == 1) {
        temp->unk16 = 1;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 800.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else if (type == 2) {
        temp->unk16 = 1;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 800.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }
    if (type == 0) {
        temp->unk16 = 2;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 400.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 2;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 4;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else if (type == 1) {
        temp->unk16 = 2;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 800.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    } else if (type == 2) {
        temp->unk16 = 2;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 0x100;
        temp->unk4.f[0] = 800.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 2;
        temp->unk14 = 9;
        temp->unk10 = data_8C;
        temp->unk0 = 4;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;
    }
    if (type == 2) {
        temp->unk16 = 3;
        temp->unk14 = 0;
        temp->unk10 = NULL;
        temp->unk0 = 0x20000000;
        temp->unk4.f[0] = 999.0f;
        temp->unk4.f[1] = 96.0f;
        temp->unk4.f[2] = 97.0f;
        temp++;
    }

    sp350.unk44 = type;
    sp350.unk4 = obj;
    if (type == 0) {
        sp350.unk2C.f[0] = 0.0f;
        sp350.unk2C.f[1] = 0.0f;
        sp350.unk2C.f[2] = 0.0f;
    } else {
        sp350.unk2C.f[0] = 0.0f;
        sp350.unk2C.f[1] = 0.0f;
        sp350.unk2C.f[2] = 0.0f;
    }
    sp350.unk59 = 9;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk40 = 1;
    sp350.unk3C = 0;
    sp350.unk5A = 0;
    sp350.unk5B = 0;
    sp350.unk38 = 1.0f;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_BC[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x04000000;
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

    if (type == 0) {
        sp350.unk58 = 0;
        var_t1 = dll_modgfx->Func1(&sp350, 0, 9, data_0, 8, data_5C, TEXTABLE_156, NULL);
    } else if (type == 1) {
        sp350.unk58 = 0;
        sp350.unk54 |= 4;
        var_t1 = dll_modgfx->Func1(&sp350, 0, 9, data_0, 8, data_5C, TEXTABLE_89, NULL);
    } else if (type == 2) {
        sp350.unk58 = 0;
        sp350.unk54 |= 4;
        var_t1 = dll_modgfx->Func1(&sp350, 0, 9, data_0, 8, data_5C, TEXTABLE_23B, NULL);
    }
    return var_t1;
}
