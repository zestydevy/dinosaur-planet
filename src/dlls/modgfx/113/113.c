#include "common.h"

/*0x0*/ static s16 data_0[] = {
    0x03e8, 0x0000, 0x0190, 0x001f, 0x001f, 0x02c3, 0xfd3d, 0x0190, 0x0000, 0x001f, 0x0000, 0xfc18, 0x0190, 0x001f, 0x001f, 0xfd3d, 
    0xfd3d, 0x0190, 0x0000, 0x001f, 0xfc18, 0x0000, 0x0190, 0x001f, 0x001f, 0xfd3d, 0x02c3, 0x0190, 0x0000, 0x001f, 0x0000, 0x03e8, 
    0x0190, 0x001f, 0x001f, 0x02c3, 0x02c3, 0x0190, 0x0000, 0x001f, 0x0000, 0x0000, 0x0000, 0x000f, 0x0000, 0x0000
};
/*0x5C*/ static s16 data_5C[] = {
    0x0000, 0x0001, 0x0008, 0x0001, 0x0002, 0x0008, 0x0002, 0x0003, 0x0008, 0x0003, 0x0004, 0x0008, 0x0004, 0x0005, 0x0008, 0x0005, 
    0x0006, 0x0008, 0x0006, 0x0007, 0x0008, 0x0007, 0x0000, 0x0008
};
/*0x8C*/ static s16 data_8C[] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0000
};
/*0xA0*/ static s16 data_A0[] = { 0x0008, 0x0000 };
/*0xA4*/ static s16 data_A4[] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007 };
/*0xB4*/ static s16 data_B4[] = { 0x0000, 0x0050, 0x001e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };

// offset: 0x0 | ctor
void dll_113_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_113_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_113_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, void* data) {
    ModgfxStruct sp350;
    ModgfxStruct_0 sp50[32];
    ModgfxStruct_0 *temp;
    s32 pad[2];
    f32 randResult;
    s32 i;

    temp = sp50;
    temp->unk16 = 0;
    temp->unk14 = 8;
    temp->unk10 = data_A4;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 1;
    temp->unk10 = NULL;
    temp->unk0 = 0x02008000;
    temp->unk4.f[0] = 125.0f;
    temp->unk4.f[1] = 255.0f;
    temp->unk4.f[2] = 125.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x02080000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 17.0f;
    temp->unk4.f[2] = -17.0f;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_8C;
    temp->unk0 = 0x80;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = obj->srt.yaw;
    temp++;

    temp->unk16 = 0;
    temp->unk14 = 0xA8;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;
    
    temp->unk16 = 0;
    temp->unk14 = 9;
    temp->unk10 = data_8C;
    temp->unk0 = 2;
    randResult = (mathRnd(0, 12) * 0.05f) + (0, 2.6f);
    temp->unk4.f[0] = randResult;
    temp->unk4.f[1] = randResult;
    temp->unk4.f[2] = randResult;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x10000000;
    temp->unk4.f[0] = 29.0f;
    temp->unk4.f[1] = 2.0f;
    temp->unk4.f[2] = 0/*.0f*/;
    temp++;

    temp->unk14 = 8;
    temp->unk16 = 1;
    temp->unk10 = data_A4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 9;
    temp->unk10 = data_8C;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 600.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x400000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = -200.0f;
    temp++;

    temp->unk16 = 1;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x02080000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 17.0f;
    temp->unk4.f[2] = -200.0f;
    temp++;

    temp->unk14 = 8;
    temp->unk16 = 2;
    temp->unk10 = data_A4;
    temp->unk0 = 0x4000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = -4.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 9;
    temp->unk10 = data_8C;
    temp->unk0 = 0x100;
    temp->unk4.f[0] = 600.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 1;
    temp->unk10 = data_A0;
    temp->unk0 = 4;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    temp->unk16 = 2;
    temp->unk14 = 0;
    temp->unk10 = NULL;
    temp->unk0 = 0x02008000;
    temp->unk4.f[0] = 0.0f;
    temp->unk4.f[1] = 0.0f;
    temp->unk4.f[2] = 0.0f;
    temp++;

    sp350.unk4 = obj;
    sp350.unk44 = type;
    sp350.unk40 = 1;
    sp350.unk59 = 9;
    sp350.unk58 = 0;
    sp350.unk3C = 0;
    sp350.unk5A = 0;
    sp350.unk5B = 0;
    sp350.unk2C.f[0] = 0/*.0f*/;
    sp350.unk2C.f[1] = 17.0f;
    sp350.unk2C.f[2] = -40.0f;
    sp350.unk38 = 1.0f;
    sp350.unk20 = 0.0f;
    sp350.unk24 = 0.0f;
    sp350.unk28 = 0.0f;
    sp350.unk5D = temp - sp50;
    for (i = 0; i < 7; i++) { sp350.unk46[i] = data_B4[i]; }
    sp350.unk0 = sp50;
    sp350.unk54 = flags | 0x04000010;
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
    return dll_modgfx->Func1(&sp350, 0, 9, data_0, 8, data_5C, TEXTABLE_90, NULL);
}
