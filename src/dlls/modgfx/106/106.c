#include "common.h"

typedef struct {
    s32 unk0;
    u16 pad4;
    s16 unk6;
    u16 pad8;
    s16 unkA;
    u16 padC;
    s16 unkE;
} DLL106_Data;

/*0x0*/ static s16 data_0[] = {
    30, 0, 0, 0, 31,
    -30, 0, 0, 15, 31,
    0, 0, 1000, 8, 0
};
/*0x20*/ static s16 data_20[] = {
    15, 0, 0, 0, 31,
    -15, 0, 0, 15, 31,
    15, 0, 2000, 8, 0,
    -15, 0, 2000, 8, 0
};
/*0x48*/ static s16 data_48[] = { 0, 1, 2, 0 };
/*0x50*/ static s16 data_50[] = { 0, 1, 2, 1, 3, 2 };
/*0x5C*/ static s16 data_5C[] = { 0, 1, 2, 0 };
/*0x64*/ static s16 data_64[] = { 0, 1, 2, 3 };
/*0x6C*/ static s16 data_6C[] = { 0, 80, 0, 0, 0, 0, 0, 0, 0, 0 };

// offset: 0x0 | ctor
void dll_106_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_106_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_106_modgfx_Func0(Object* obj, s32 type, SRT* transform, u32 flags, s32 arg4, DLL106_Data* data) {
    ModgfxStruct sp3C8;
    ModgfxStruct_0 spC8[32];
    ModgfxStruct_0* temp_s0;
    f32 temp_fs0;
    f32 temp_fv1;
    s16 var_s5;
    s16 var_s6;
    s16 var_s7;
    s16 var_v1;
    s32 spB0;
    s32 var_a2;
    s32 i;
    s32 var_v1_2;
    SRT sp8C;
    s16* var_a3;
    s16* var_v0_2;
    s32 j;

    var_v1 = 0;
    var_s5 = 0xFF;
    var_s6 = 0xFF;
    var_s7 = 0xFF;
    spB0 = 1;
    if (data != NULL) {
        spB0 = data->unk0;
        var_s5 = data->unk6;
        var_s6 = data->unkA;
        var_s7 = data->unkE;
    }

    for (i = 0; i < spB0; i++) {
        if (type == 0) {
            var_s5 += mathRnd(-27, 27);
            if (var_s5 > 0xFF) {
                var_s5 = 0xFF;
            } else if (var_s5 < 0) {
                var_s5 = 0;
            }
            var_s6 += mathRnd(-27, 27);
            if (var_s6 > 0xFF) {
                var_s6 = 0xFF;
            } else if (var_s6 < 0) {
                var_s6 = 0;
            }
            var_s7 += mathRnd(-27, 27);
            if (var_s7 > 0xFF) {
                var_s7 = 0xFF;
            } else if (var_s7 < 0) {
                var_s7 = 0;
            }
        }

        temp_s0 = spC8;
        temp_s0->unk16 = 0;
        if (type != 0) {
            temp_s0->unk14 = 4;
        } else {
            temp_s0->unk14 = 3;
        }

        if (type != 0) {
            temp_s0->unk10 = data_64;
        } else {
            temp_s0->unk10 = data_5C;
        }

        temp_s0->unk0 = 8;
        temp_s0->unk4.f[0] = var_s5;
        temp_s0->unk4.f[1] = var_s6;
        temp_s0->unk4.f[2] = var_s7;
        temp_s0++;
        temp_fs0 = mathRnd(0, 0xFFFE);
        temp_fv1 = mathRnd(-3000, -12000);
        temp_s0->unk16 = 0;
        temp_s0->unk14 = 0;
        temp_s0->unk10 = NULL;
        temp_s0->unk0 = 0x80;
        temp_s0->unk4.x = 0.0f;
        temp_s0->unk4.f[1] = temp_fv1;
        temp_s0->unk4.f[2] = temp_fs0;
        temp_s0++;
        temp_s0->unk16 = 0;
        if (type != 0) {
            temp_s0->unk14 = 4;
        } else {
            temp_s0->unk14 = 3;
        }

        if (type != 0) {
            temp_s0->unk10 = data_64;
        } else {
            temp_s0->unk10 = data_5C;
        }

        temp_s0->unk0 = 2;
        temp_s0->unk4.x = 1.0f;
        temp_s0->unk4.f[1] = 0.5f;
        temp_s0->unk4.f[2] = 1.5f;
        temp_s0++;

        temp_s0->unk16 = 1;
        temp_s0->unk14 = 0;
        temp_s0->unk10 = 0;
        temp_s0->unk0 = 0x400000;
        temp_s0->unk4.f[0] = 0.0f;
        temp_s0->unk4.f[1] = 0.0f;
        temp_s0->unk4.f[2] = 400.0f;

        sp8C.transl.f[0] = 0.0f;
        sp8C.transl.f[1] = 0.0f;
        sp8C.transl.f[2] = 0.0f;
        sp8C.scale = 1.0f;
        sp8C.roll = 0;
        sp8C.pitch = (s16) temp_fv1;
        sp8C.yaw = (s16) temp_fs0;
        mathRotateRPY(&sp8C, temp_s0->unk4.f);
        sp3C8.unk58 = 0;
        sp3C8.unk44 = type;
        sp3C8.unk2C.f[0] = 0.0f;
        sp3C8.unk2C.f[1] = 0.0f;
        sp3C8.unk2C.f[2] = 0.0f;
        sp3C8.unk20 = 0.0f;
        sp3C8.unk24 = 0.0f;
        sp3C8.unk28 = 0.0f;
        sp3C8.unk38 = 1.0f;
        sp3C8.unk4 = obj;
        sp3C8.unk3C = 0; sp3C8.unk40 = 1;
        if (type != 0) {
            sp3C8.unk59 = 4;
        } else {
            sp3C8.unk59 = 3;
        }

        sp3C8.unk5A = 0;
        sp3C8.unk5B = 0x10;
        sp3C8.unk5D = 4;
        for (j = 0; j < 7; j++) { sp3C8.unk46[j] = data_6C[j]; }
        sp3C8.unk0 = spC8;
        sp3C8.unk54 = flags | 0x02000490;

        if (sp3C8.unk54 & 1) {
            if ((sp3C8.unk4 != NULL) && (transform != NULL)) {
                sp3C8.unk2C.f[0] += sp3C8.unk4->globalPosition.x + transform->transl.x;
                sp3C8.unk2C.f[1] += sp3C8.unk4->globalPosition.f[1] + transform->transl.f[1];
                sp3C8.unk2C.f[2] += sp3C8.unk4->globalPosition.f[2] + transform->transl.f[2];
            } else if (sp3C8.unk4 != NULL) {
                sp3C8.unk2C.f[0] += sp3C8.unk4->globalPosition.x;
                sp3C8.unk2C.f[1] += sp3C8.unk4->globalPosition.f[1];
                sp3C8.unk2C.f[2] += sp3C8.unk4->globalPosition.f[2];
            } else if (transform != NULL) {
                sp3C8.unk2C.f[0] += transform->transl.x;
                sp3C8.unk2C.f[1] += transform->transl.f[1];
                sp3C8.unk2C.f[2] += transform->transl.f[2];
            }
        }

        if (type != 0) {
            var_a2 = 4;
        } else {
            var_a2 = 3;
        }

        if (type != 0) {
            var_a3 = data_20;
        } else {
            var_a3 = data_0;
        }

        if (type != 0) {
            var_v1_2 = 2;
        } else {
            var_v1_2 = 1;
        }

        if (type != 0) {
            var_v0_2 = data_50;
        } else {
            var_v0_2 = data_48;
        }

        var_v1 = dll_modgfx->Func1(&sp3C8, 0, var_a2, var_a3, var_v1_2, var_v0_2, 0, NULL);
    }

    return var_v1;
}
