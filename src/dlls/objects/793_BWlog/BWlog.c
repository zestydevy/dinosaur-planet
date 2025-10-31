#include "common.h"
#include "sys/objtype.h"

typedef struct {
    u8 _unk0[0x20C - 0x0];
    f32 unk20C[2];
    u8 _unk214[0x21C - 0x214];
    f32 unk21C[2];
    u8 _unk224[0x22C - 0x224];
    f32 unk22C[2];
    u8 _unk234[0x25C - 0x234];
    s8 unk25C;
    u8 _unk25D[0x260 - 0x25D];
    Vec3f unk260[2];
    f32 unk278;
    f32 unk27C;
    f32 unk280;
    f32 unk284;
    f32 unk288;
    f32 unk28C;
    f32 unk290;
    f32 unk294;
    f32 unk298;
    f32 unk29C;
    u8 _unk2A0[0x2D0 - 0x2A0];
    f32 unk2D0[2];
    f32 unk2D8[2];
    f32 unk2E0[2];
    u8 _unk2E8[0x300 - 0x2E8];
    f32 unk300;
    f32 unk304;
    f32 unk308;
    f32 unk30C;
    f32 unk310;
    u32 unk314;
    u8 _unk318[0x326 - 0x318];
    s16 unk326;
    s16 unk328;
    u8 _unk32A[0x32D - 0x32A];
    u8 unk32D;
    u8 _unk32E[0x33C - 0x32E];
} BWlog_Data;

// Related to DFriverflow (and more?)
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
} ObjType22Setup;

/*0x0*/ static u32 _data_0[] = {
    0x00000000, 0x00000000, 0xc1f00000, 0x00000000, 0x00000000, 0x41f00000
};
/*0x18*/ static u32 _data_18[] = {
    0x41000000, 0x41000000
};
/*0x20*/ static u32 _data_20[] = {
    0x01010000, 0x43fa0000, 0x43fa0000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_793_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_793_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_793_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_setup.s")

// offset: 0x170 | func: 1 | export: 1
void dll_793_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_control.s")

// offset: 0x85C | func: 2 | export: 2
void dll_793_update(Object *self) { }

// offset: 0x868 | func: 3 | export: 3
void dll_793_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x8BC | func: 4 | export: 4
void dll_793_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_free.s")

// offset: 0x92C | func: 5 | export: 5
u32 dll_793_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x93C | func: 6 | export: 6
u32 dll_793_get_data_size(Object *self, u32 a1) {
    return sizeof(BWlog_Data);
}

// offset: 0x950 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_950.s")

// offset: 0x9E0 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_9E0.s")

// offset: 0xB28 | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_B28.s")

// offset: 0xB48 | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_B48.s")

// offset: 0xC3C | func: 11 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_C3C.s")

// offset: 0xC4C | func: 12 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_C4C.s")

// offset: 0xD08 | func: 13 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_D08.s")

// offset: 0xD18 | func: 14 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_D18.s")

// offset: 0xE2C | func: 15 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_E2C.s")

// offset: 0xE48 | func: 16 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_E48.s")

// offset: 0xE70 | func: 17 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_E70.s")

// offset: 0xE80 | func: 18 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_E80.s")

// offset: 0xE8C | func: 19 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_E8C.s")

// offset: 0xE9C | func: 20 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_E9C.s")

// offset: 0xEB0 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_EB0.s")

// offset: 0x1600 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_1600.s")

// offset: 0x178C | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_178C.s")

// offset: 0x1858 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_1858.s")

// offset: 0x1A4C | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_1A4C.s")

// offset: 0x1A5C | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/793_BWlog/dll_793_func_1A5C.s")

// offset: 0x1C18 | func: 27
void dll_793_func_1C18(Object* self, BWlog_Data* objdata) {
    s32 i;
    s32 k;
    s32 sp120[2];
    s32 objListLength;
    Object* obj;
    Object** objList;
    f32 temp_fs0;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;
    f32 sp100;
    f32 spFC;
    f32 spF8;
    SRT spE0;
    MtxF spA0;

    for (i = 0; i < 2; i++) {
        objdata->unk2D0[i] = 0.0f;
        objdata->unk2E0[i] = 0.0f;
        sp120[i] = 0;
    }

    // grabs DFriverflow instances (and possibly more)
    objList = obj_get_all_of_type(OBJTYPE_22, &objListLength);

    for (i = 0; i < objListLength; i++) {
        obj = objList[i];
        for (k = 0; k < 2; k++) {
            temp_fv0 = obj->srt.transl.y - objdata->unk260[k].y;
            if ((temp_fv0 <= 200.0f) && (temp_fv0 >= -200.0f)) {
                temp_fs0 = obj->srt.transl.x - objdata->unk260[k].x;
                temp_fv0_2 = obj->srt.transl.z - objdata->unk260[k].z;
                temp_fs0 = sqrtf(SQ(temp_fs0) + SQ(temp_fv0_2));
                temp_fv1 = (f32) ((ObjType22Setup*)obj->setup)->unk19 * 1.5f;
                if (temp_fs0 < temp_fv1) {
                    temp_fs0 = ((temp_fv1 - temp_fs0) / temp_fv1);
                    temp_fs0 *= (obj->srt.scale * 10.0f);
                    objdata->unk2D0[k] += fsin16_precise(obj->srt.yaw) * temp_fs0;
                    objdata->unk2E0[k] += fcos16_precise(obj->srt.yaw) * temp_fs0;
                    sp120[k] += 1;
                }
            }
        }
    }

    for (i = 0; i < 2; i++) {
        if (sp120[i] != 0) {
            objdata->unk2D0[i] /= sp120[i];
            objdata->unk2E0[i] /= sp120[i];
        }
    }
    
    for (i = 0; i < 2; i++) {
        spE0.yaw = arctan2_f(objdata->unk2D0[i], objdata->unk2E0[i]);
        spE0.pitch = 0;
        spE0.roll = 0;
        spE0.scale = 1.0f;
        spE0.transl.x = 0;
        spE0.transl.y = 0;
        spE0.transl.z = 0;
        matrix_from_srt_reversed(&spA0, &spE0);
        vec3_transform(&spA0, objdata->unk20C[i], objdata->unk21C[i], objdata->unk22C[i], &sp100, &spFC, &spF8);
        spE0.yaw = 0;
        spE0.pitch = 0x4000 - arctan2_f(spFC, spF8);
        spE0.roll = -(0x4000 - arctan2_f(spFC, sp100));
        matrix_from_srt_reversed(&spA0, &spE0);
        vec3_transform(&spA0, objdata->unk2D0[i], 0.0f, objdata->unk2E0[i], 
                       &objdata->unk2D0[i], &objdata->unk2D8[i], &objdata->unk2E0[i]);
    }
}

// offset: 0x2020 | func: 28
void dll_793_func_2020(Object* arg0, BWlog_Data* arg1) {
    u8 var_a1;
    u8 sp3E;
    s32 var_v1;

    if (arg1->unk314 == 0) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0xA77, 0x7F, &arg1->unk314, NULL, 0, NULL);
    } else {
        arg1->unk30C = arg1->unk310 * 127.0f;
        arg1->unk30C += fsin16_precise(arg1->unk328) * 30.0f;
        if (arg1->unk30C < 30.0f) {
            arg1->unk30C = 30.0f;
        } else if (arg1->unk30C > 127.0f) {
            arg1->unk30C = 127.0f;
        }
        gDLL_6_AMSFX->vtbl->func_860(arg1->unk314, arg1->unk30C);
        arg1->unk308 = ((arg1->unk304 + arg1->unk300) * 0.5f) / 25.0f;
        if (arg1->unk308 < 0.0f) {
            arg1->unk308 = 0.0f;
        }
        arg1->unk308 = 1.0f - arg1->unk308;
        arg1->unk308 = (arg1->unk308 * 0.2f) + 0.2f;
        arg1->unk308 += (fsin16_precise(arg1->unk326) * 0.1f);
        gDLL_6_AMSFX->vtbl->func_954(arg1->unk314, arg1->unk308);
        arg1->unk326 = arg1->unk326 + (gUpdateRate << 8);
        arg1->unk328 = arg1->unk328 + (gUpdateRate << 9);
    }
    var_v1 = 0;
    sp3E = arg1->unk25C & 3;
    var_a1 = sp3E & (sp3E ^ arg1->unk32D);
    if (var_a1 & 1) {
        var_v1 = (s32) ((sqrtf(SQ(arg1->unk298) + SQ(arg1->unk290)) * 127.0f) / 0.95f);
    }
    if (var_a1 & 2) {
        if (var_v1 > ((sqrtf(SQ(arg1->unk29C) + SQ(arg1->unk294)) * 127.0f) / 0.95f)) {
            var_v1 = (s32) (f32) var_v1; // what
        } else {
            var_v1 = (s32) ((sqrtf(SQ(arg1->unk29C) + SQ(arg1->unk294)) * 127.0f) / 0.95f);
        }
    }
    if (var_v1 >= 0xB) {
        if (var_v1 >= 0x80) {
            var_v1 = 0x7F;
        }
        gDLL_6_AMSFX->vtbl->play_sound(arg0, 0x76D, var_v1, NULL, NULL, 0, NULL);
    }
    arg1->unk32D = sp3E;
}

// offset: 0x2444 | func: 29
void dll_793_func_2444(Object* self, BWlog_Data* objdata) {
    f32 var_fs0;
    f32 var_fs1;
    f32 spF4;
    f32 spF0;
    f32 spEC;
    Vec3f spE0;
    Vec3f particleConfig;
    Vec3f spC8;
    f32 temp_fv0;
    s32 temp_ft5;
    SRT spA8;
    MtxF sp68;
    s32 i;

    var_fs0 = (objdata->unk2D0[0] + objdata->unk2D0[1]) * 0.5f;
    var_fs1 = (objdata->unk2E0[0] + objdata->unk2E0[1]) * 0.5f;
    spEC = sqrtf(SQ(var_fs0) + SQ(var_fs1));
    if (spEC != 0.0f) {
        var_fs0 /= spEC;
        var_fs1 /= spEC;
    }
    spF4 = fsin16_precise(self->srt.yaw);
    spE0.y = -fcos16_precise(self->srt.yaw);
    spE0.x = spF4;
    particleConfig.x = -400.0f;
    particleConfig.y = 400.0f;
    spA8.yaw = arctan2_f(var_fs0, var_fs1);
    spA8.pitch = 0;
    spA8.roll = 0;
    spA8.scale = 1.0f;
    spA8.transl.x = 0.0f;
    spA8.transl.y = 0.0f;
    spA8.transl.z = 0.0f;
    spC8.y = (objdata->unk284 + objdata->unk278) * 0.5f;
    spC8.x = (objdata->unk28C + objdata->unk280) * 0.5f;
    matrix_from_srt_reversed(&sp68, &spA8);
    vec3_transform(&sp68, spC8.y, 0, spC8.x, &spF4, &spC8.z, &spE0.z);
    spEC = spEC * 1.5f;
    spEC = spEC - spE0.z;
    if (spEC < 0.0f) {
        spEC = 0.0f;
    }
    temp_ft5 = (s32) spEC;
    particleConfig.z = (0.001f * spEC) + 0.004f;
    spA8.transl.x = (var_fs0 * spE0.y) + (var_fs1 * spE0.x);
    if (spA8.transl.x < 0.0f) {
        for (i = 0; i < temp_ft5; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_76C, &spA8, PARTFXFLAG_4, -1, &particleConfig);
        }
    } else {
        for (i = 0; i < temp_ft5; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_76C, &spA8, PARTFXFLAG_4, -1, &particleConfig);
        }
    }
}
