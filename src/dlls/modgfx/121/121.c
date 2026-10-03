#include "common.h"

/*0x0*/ static s16 data_0[] = {
    0xfc18, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xfc18, 0x0000, 0x0000, 0x03e8, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000,
    0x0000, 0x03e8, 0x000f, 0x0000, 0xfc18, 0x0fa0, 0x0000, 0x0000, 0x001f, 0x0000, 0x0fa0, 0xfc18, 0x0000, 0x001f, 0x03e8, 0x0fa0,
    0x0000, 0x000f, 0x001f, 0x0000, 0x0fa0, 0x03e8, 0x000f, 0x001f
};
/*0x50*/ static s16 data_50[] = {
    0x0000, 0x0002, 0x0006, 0x0000, 0x0006, 0x0004, 0x0001, 0x0003, 0x0007, 0x0001, 0x0007, 0x0005, 0x0000, 0x0001, 0x0002, 0x0003,
    0x0004, 0x0005, 0x0006, 0x0007
};
/*0x78*/ static s16 data_78[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0000, 0x0002, 0x0004, 0x0006, 0x0001, 0x0003, 0x0005, 0x0007
};
/*0x98*/ static s16 data_98[] = { 0x0000, 0x0104, 0x001e, 0x0001, 0x0104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_121_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_121_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0

s32 dll_121_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, s32* data) {
    ModgfxStruct sp368;
    ModgfxStruct_0 sp68[32];
    ModgfxStruct_0 *temp;
    s32 pad[3];
    s32 i;
    s32 var_t0;
    s32 var_t1;
    s32 var_t2;
    s32 var_v0;
    s32 var_v1;

    temp = sp68;

    var_v1 = 0x30;
    var_t0 = 0x31;
    var_t1 = 1;
    var_t2 = 0x50;
    if (data != NULL) {
        var_t1 = data[0];
        var_v1 = data[1];
        var_t0 = data[2];
        var_t2 = data[3];
    }

    temp->unk16 = 0;
    temp->unk14 = 8;
    temp->unk10 = data_78;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 8;
    temp->unk10 = data_78;
    temp->unk0 = 2;
    if (obj != NULL) {
        temp->unk4.f[0] = obj->srt.scale * 7.0f;
        temp->unk4.f[1] = obj->srt.scale * 6.0f;
        temp->unk4.f[2] = obj->srt.scale * 7.0f;
    } else {
        temp->unk4.f[0] = 7.0f;
        temp->unk4.f[1] = 6.0f;
        temp->unk4.f[2] = 7.0f;
    }
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    if (obj != NULL) {
        temp->unk4.f[2] = obj->srt.yaw;
    } else {
        temp->unk4.f[2] = 0/*.0f*/;
    }
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 8;
    temp->unk10 = data_78;
    temp->unk0 = 4;
    temp->unk4.f[0] = 255.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = var_t2;
    temp->unk10 = 0;
    temp->unk0 = 0x20000000;
    temp->unk4.f[0] = var_t1;
    temp->unk4.f[1] = var_v1;
    temp->unk4.f[2] = var_t0;
    temp++;

    if (type == 0) {
        temp->unk16 = 2;
        temp->unk14 = 0x3B;
        temp->unk10 = NULL;
        temp->unk0 = 0x01800000;
        temp->unk4.f[0] = 1.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 10.0f;
        temp++;
    }

    temp->unk16 = 2;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 50.0f;
    temp++;

    temp->unk16 = 3;
    temp->unk14 = 1;
    temp->unk10 = 0;
    temp->unk0 = 0x2000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 8;
    temp->unk10 = data_78;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 4;
    temp->unk14 = 0;
    temp->unk10 = 0;
    temp->unk0 = 0x20000000;
    temp->unk4.f[0] = var_t1;
    temp->unk4.f[1] = var_v1;
    temp->unk4.f[2] = var_t0;
    temp++;

    sp368.unk4 = obj;
    sp368.unk2C.f[0] = 0.0f;
    sp368.unk58 = type;
    sp368.unk44 = type;
    if (transform != NULL) {
        sp368.unk2C.f[1] = transform->transl.f[1];
    } else {
        sp368.unk2C.f[1] = 0.0f;
    }
    sp368.unk40 = 1;
    sp368.unk2C.f[2] = 0.0f;
    sp368.unk20 = 0.0f;
    sp368.unk24 = 0.0f;
    sp368.unk28 = 0.0f;
    sp368.unk3C = 0;
    sp368.unk38 = 1.0f;
    sp368.unk59 = 8;
    sp368.unk5A = 0;
    sp368.unk5B = 0x1E;
    sp368.unk5D = temp - sp68;
    for (i = 0; i < 7; i++) {
        sp368.unk46[i] = data_98[i];
    }
    sp368.unk0 = sp68;
    sp368.unk54 = flags | 0x04000000 | 0x80;
    if (sp368.unk54 & 1) {
        if (sp368.unk4 != NULL) {
            sp368.unk2C.f[0] += sp368.unk4->globalPosition.f[0];
            sp368.unk2C.f[1] += sp368.unk4->globalPosition.f[1];
            sp368.unk2C.f[2] += sp368.unk4->globalPosition.f[2];
        } else {
            sp368.unk2C.f[0] += transform->transl.f[0];
            sp368.unk2C.f[1] += transform->transl.f[1];
            sp368.unk2C.f[2] += transform->transl.f[2];
        }
    }

    return dll_modgfx->Func1(&sp368, 0, 8, data_0, 4, data_50, type == 2 ? TEXTABLE_17B : TEXTABLE_8E, NULL);
}