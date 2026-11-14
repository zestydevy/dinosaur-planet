#include "sys/math.h"
#include "game/objects/object.h"
#include "dlls/engine/14_modgfx.h"
#include "sys/gfx/textable.h"
#include "dll.h"

/*0x0*/ static s16 data_0[] = {
    0, 1000, 0, 0, 0, 866, 500, 0, 16, 0, 866, -500, 0, 31, 0, 0,
    -1000, 0, 47, 0, -866, -500, 0, 31, 0, -866, 500, 0, 16, 0, 0, 1000,
    0, 0, 0, 0, 1000, 3000, 0, 127, 866, 500, 3000, 16, 127, 866, -500, 3000,
    31, 127, 0, -1000, 3000, 47, 127, -866, -500, 3000, 31, 127, -866, 500, 3000, 16,
    127, 0, 1000, 3000, 0, 127, 0, 1000, 6000, 0, 255, 866, 500, 6000, 16, 255,
    866, -500, 6000, 31, 255, 0, -1000, 6000, 47, 255, -866, -500, 6000, 31, 255, -866,
    500, 6000, 16, 255, 0, 1000, 6000, 0, 255, 0
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
/*0x1F8*/ static s16 data_1F8[] = { 0, 46, 100, 16, 0, 0, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_155_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_155_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_155_modgfx_Spawn(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, f32* data) {
    ModgfxStruct sp3C0;
    ModgfxStruct_0 spC0[32];
    ModgfxStruct_0 *temp;
    s32 pad[1];
    s32 i;
    s32 j;
    f32 var_fs1;
    f32 var_fv0;
    s16 out;
    s32 tempType;
    f32 twoF;

    tempType = type;
    twoF = 2.0f;
    var_fs1 = -1.7f;
    var_fv0 = 1.0f;
    if (data != NULL) {
        var_fv0 = *data;
    }

    for (i = 0; i < 2; i++) {
        temp = spC0;
        if (i == 1) {
            var_fs1 = -4.2f;
        }

        temp->unk16 = 0;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 4;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 0;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 0x80;
        temp->unk4.f[0] = 0.0f;
        temp->unk4.f[1] = obj->srt.pitch;
        temp->unk4.f[2] = (obj->srt.yaw - 16128.0f) + 16383.0f;
        temp++;

        if (i == 0) {
            temp->unk16 = 0;
            temp->unk14 = 0x15;
            temp->unk10 = data_1B0;
            temp->unk0 = 2;
            if (tempType == 4) {
                temp->unk4.f[0] = 0.0375f;
                temp->unk4.f[1] = 0.0375f;
                temp->unk4.f[2] = 0.01f + var_fv0;
            } else {
                temp->unk4.f[0] = 0.0125f;
                temp->unk4.f[1] = 0.0125f;
                temp->unk4.f[2] = 0.01f + var_fv0;
            }
            temp++;
        } else {
            temp->unk16 = 0;
            temp->unk14 = 0x15;
            temp->unk10 = data_1B0;
            temp->unk0 = 2;
            if (tempType == 4) {
                temp->unk4.f[0] = 0.03f;
                temp->unk4.f[1] = 0.03f;
                temp->unk4.f[2] = 0.01f + var_fv0;
            } else {
                temp->unk4.f[0] = 0.01f;
                temp->unk4.f[1] = 0.01f;
                temp->unk4.f[2] = 0.01f + var_fv0;
            }
            temp++;
        }

        temp->unk16 = 0;
        temp->unk14 = 0;
        temp->unk10 = 0;
        temp->unk0 = 0x400000;
        switch (tempType) {
        case 0:
            temp->unk4.f[0] = 0.0f;
            temp->unk4.f[1] = 30.0f;
            temp->unk4.f[2] = 0.0f;
            break;
        case 1:
            temp->unk4.f[0] = 0.0f;
            temp->unk4.f[1] = -30.0f;
            temp->unk4.f[2] = 0.0f;
            break;
        case 2:
            temp->unk4.f[0] = 30.0f;
            temp->unk4.f[1] = 0.0f;
            temp->unk4.f[2] = 0.0f;
            break;
        case 3:
            temp->unk4.f[0] = -30.0f;
            temp->unk4.f[1] = 0.0f;
            temp->unk4.f[2] = 0.0f;
            break;
        case 4:
            temp->unk4.f[0] = 0.0f;
            temp->unk4.f[1] = 0.1f;
            temp->unk4.f[2] = 0.0f;
            break;
        }
        temp++;

        temp->unk16 = 1;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 4;
        temp->unk4.f[0] = 255.0f;
        temp->unk4.f[1] = 0.0f;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 1;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 6.0f;
        temp->unk4.f[1] = 6.0f;
        temp->unk4.f[2] = 300.0f;
        temp++;

        temp->unk16 = 1;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 0x4000;
        temp->unk4.f[0] = twoF * 1;
        temp->unk4.f[1] = var_fs1 * 1/*.0f*/;
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
        temp->unk4.f[0] = 2.0f;
        temp->unk4.f[1] = var_fs1;
        temp->unk4.f[2] = 0.0f;
        temp++;

        temp->unk16 = 3;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 0x4000;
        temp->unk4.f[0] = 2.0f;
        temp->unk4.f[1] = var_fs1;
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

        temp->unk16 = 3;
        temp->unk14 = 0x15;
        temp->unk10 = data_1B0;
        temp->unk0 = 2;
        temp->unk4.f[0] = 0.1f;
        temp->unk4.f[1] = 0.1f;
        temp->unk4.f[2] = 0.1f;
        temp++;

        sp3C0.unk58 = 0;
        sp3C0.unk4 = obj;
        sp3C0.unk44 = type;
        sp3C0.unk2C.f[0] = 0.0f;
        sp3C0.unk2C.f[1] = 0.0f;
        sp3C0.unk2C.f[2] = 0.0f;
        sp3C0.unk20 = 0.0f;
        sp3C0.unk24 = 0.0f;
        sp3C0.unk28 = 0.0f;
        sp3C0.unk38 = 6.4f;
        sp3C0.unk40 = 2;
        sp3C0.unk3C = 7;
        sp3C0.unk59 = 0xE;
        sp3C0.unk5A = 0;
        sp3C0.unk5B = 0x28;
        sp3C0.unk5D = temp - spC0;
        for (j = 0; j < 7; j++) { sp3C0.unk46[j] = data_1F8[j]; }
        sp3C0.unk0 = spC0;
        sp3C0.unk54 = 0x0C0104C0 | flags;
        if (sp3C0.unk54 & 1) {
            if (sp3C0.unk4 != NULL) {
                sp3C0.unk2C.f[0] += sp3C0.unk4->globalPosition.f[0];
                sp3C0.unk2C.f[1] += sp3C0.unk4->globalPosition.f[1];
                sp3C0.unk2C.f[2] += sp3C0.unk4->globalPosition.f[2];
            } else {
                sp3C0.unk2C.f[0] += transform->transl.f[0];
                sp3C0.unk2C.f[1] += transform->transl.f[1];
                sp3C0.unk2C.f[2] += transform->transl.f[2];
            }
        }
    
        out = dll_modgfx->Func1(&sp3C0, 0, 0x15, data_0, 0x18, data_D4, TEXTABLE_D9, NULL);
    }

    return out;
}
