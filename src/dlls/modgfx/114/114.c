#include "common.h"
#include "dlls/objects/common/campfire.h"

/*0x0*/ static s16 data_0[] = {
    0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x0362, 0x0000, 0x01f4, 0x000b, 0x0000, 0x0362, 0x0000, 0xfe0c, 0x0016, 0x0000, 0x0000,
    0x0000, 0xfc18, 0x0020, 0x0000, 0xfc9e, 0x0000, 0xfe0c, 0x002a, 0x0000, 0xfc9e, 0x0000, 0x01f4, 0x0034, 0x0000, 0x0000, 0x0000,
    0x03e8, 0x003f, 0x0000, 0x0000, 0x0640, 0x03e8, 0x0000, 0x000f, 0x0362, 0x0640, 0x01f4, 0x000b, 0x000f, 0x0362, 0x0640, 0xfe0c,
    0x0016, 0x000f, 0x0000, 0x0640, 0xfc18, 0x0020, 0x000f, 0xfc9e, 0x0640, 0xfe0c, 0x002a, 0x000f, 0xfc9e, 0x0640, 0x01f4, 0x0034,
    0x000f, 0x0000, 0x0640, 0x03e8, 0x003f, 0x000f, 0x0000, 0x1770, 0x03e8, 0x0000, 0x001f, 0x0362, 0x1770, 0x01f4, 0x000b, 0x001f,
    0x0362, 0x1770, 0xfe0c, 0x0016, 0x001f, 0x0000, 0x1770, 0xfc18, 0x0020, 0x001f, 0xfc9e, 0x1770, 0xfe0c, 0x002a, 0x001f, 0xfc9e,
    0x1770, 0x01f4, 0x0034, 0x001f, 0x0000, 0x1770, 0x03e8, 0x003f, 0x001f, 0x0000
};
/*0xD4*/ static s16 data_D4[] = {
    0x0000, 0x0001, 0x0008, 0x0000, 0x0008, 0x0007, 0x0001, 0x0002, 0x0009, 0x0001, 0x0009, 0x0008, 0x0002, 0x0003, 0x000a, 0x0002,
    0x000a, 0x0009, 0x0003, 0x0004, 0x000b, 0x0003, 0x000b, 0x000a, 0x0004, 0x0005, 0x000c, 0x0004, 0x000c, 0x000b, 0x0005, 0x0006,
    0x000d, 0x0005, 0x000d, 0x000c, 0x0007, 0x0008, 0x000f, 0x0007, 0x000f, 0x000e, 0x0008, 0x0009, 0x0010, 0x0008, 0x0010, 0x000f,
    0x0009, 0x000a, 0x0011, 0x0009, 0x0011, 0x0010, 0x000a, 0x000b, 0x0012, 0x000a, 0x0012, 0x0011, 0x000b, 0x000c, 0x0013, 0x000b,
    0x0013, 0x0012, 0x000c, 0x000d, 0x0014, 0x000c, 0x0014, 0x0013, 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0000
};
/*0x174*/ static s16 data_174[] = {
    0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x0000
};
/*0x184*/ static s16 data_184[] = {
    0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0000
};
/*0x194*/ static s16 data_194[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x000e, 0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014
};
/*0x1B0*/ static s16 data_1B0[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
    0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0000
};
/*0x1DC*/ static s16 data_1DC[] = { 0x0000, 0x0104, 0x003c, 0x003c, 0x0001, 0x0104, 0x0000, 0x0000, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_114_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_114_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_114_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    CampFire_Setup* setup;
    f32 var_fa1;
    u8 var_t0;
    s32 pad[1];
    s32 i;

    var_fa1 = 1.0f;
    setup = (CampFire_Setup*)obj->setup;
    var_t0 = setup->scale;
    if (type == 1) {
        var_fa1 = 4.0f;
        data_1DC[1] = 0;
    } else if (type == 2) {
        var_fa1 = 0.0f;
        var_t0 = 6;
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

    temp->unk16 = 0;
    temp->unk14 = 0xE;
    temp->unk10 = data_194;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.95f;
    temp->unk4.f[1] = 0.4f;
    temp->unk4.f[2] = 0.95f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 2;
    temp->unk4.f[0] = 0.95f;
    temp->unk4.f[1] = 0.4f;
    temp->unk4.f[2] = 0.95f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 7;
    temp->unk10 = data_184;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 10.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x3A;
    temp->unk10 = NULL;
    temp->unk0 = 0x01800000;
    temp->unk4.f[0] = var_fa1;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 5.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 10.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x3A;
    temp->unk10 = NULL;
    temp->unk0 = 0x01800000;
    temp->unk4.f[0] = var_fa1;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 5.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 10.0f;
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
    temp->unk14 = 7;
    temp->unk10 = data_174;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 7;
    temp->unk10 = data_184;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 5;
    temp->unk14 = 0x15;
    temp->unk10 = data_1B0;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 10.0f;
    temp++;

    sp350.unk58 = 0;
    sp350.unk4 = obj;
    sp350.unk44 = type;
    sp350.unk2C.f[0] = 0.0f;
    sp350.unk2C.f[1] = 0.0f;
    sp350.unk2C.f[2] = 0.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    if (var_t0 != 0) {
        sp350.unk38 = var_t0 * 0.1f;
    } else {
        sp350.unk38 = 1.0f;
    }
    sp350.unk40 = 2;
    sp350.unk3C = 7;
    sp350.unk59 = 0xE;
    sp350.unk5B = 0x1E;
    sp350.unk5A = 0;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_1DC[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x0C0000C0;
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

    return dll_modgfx->Func1(&sp350, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_8E, NULL);
}
