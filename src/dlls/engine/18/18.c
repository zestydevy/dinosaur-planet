#include "common.h"

typedef struct {
    u32 unk0;
    DLL27_Data unk4;
    s16 unk264;
    s16 unk266;
    s16 unk268;
    s16 unk26A;
    s16 unk26C;
    s16 unk26E;
    s16 unk270;
    u8 unk272;
    u8 unk273;
    Vec3f *unk274;
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
    f32 unk2A0;
    f32 unk2A4;
    s32 unk2A8;
    f32 unk2AC;
    f32 unk2B0;
    f32 unk2B4;
    f32 unk2B8;
    u8 _unk2BC[0x2C8 - 0x2BC];
    Object *unk2C8;
    u8 _unk2CC[0x2EC - 0x2CC];
    Vec3f unk2EC;
    f32 unk2F8;
    void *unk2FC;
    u8 _unk300[0x308 - 0x300];
    u32 unk308;
    u8 _unk30C[0x322 - 0x30C];
    s16 unk322;
    s16 unk324;
    s16 unk326;
    s16 unk328;
    s16 unk32A;
    s16 unk32C;
    s16 unk32E;
    s32 unk330;
    s32 unk334;
    u8 _unk338[0x33F - 0x338];
    u8 unk33F;
    s8 unk340;
    u8 unk341;
    u8 _unk342[0x34C - 0x342];
} DLL18_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x1];
/*0x1*/ static u8 _bss_1[0x1];
/*0x2*/ static u8 _bss_2[0x2];
/*0x4*/ static s16 _bss_4;
/*0x6*/ static u8 _bss_6[0x2];
/*0x8*/ static u8 _bss_8[0x4];
/*0xC*/ static u8 _bss_C[0x4];
/*0x10*/ static u8 _bss_10[0x4];
/*0x14*/ static u8 _bss_14[0x4];
/*0x18*/ static u8 _bss_18[0x4];
/*0x1C*/ static u8 _bss_1C[0x4];

// offset: 0x0 | ctor
void dll_18_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_18_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_18_func_18(Object* arg0, DLL18_Data* arg1, s32 arg2, s32 arg3) {
    u32 i;
    s8* var_v1;

    i = 0;
    var_v1 = (s8*)arg1;
    var_v1[0] = 0;
    while (i < sizeof(DLL18_Data)) {
        var_v1[i] = 0;
        i++;
    }
    arg1->unk264 = arg2;
    arg1->unk266 = arg3;
    arg1->unk268 = 0;
    arg1->unk26C = 0;
    arg1->unk272 = 1;
    arg1->unk273 = 1;
    arg1->unk330 = -1;
    arg1->unk334 = -1;
    arg1->unk2B0 = 10.0f;
}

// offset: 0x90 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_90.s")

// offset: 0x5E0 | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_5E0.s")

// offset: 0x75C | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_75C.s")

// offset: 0x778 | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_778.s")

// offset: 0x81C | func: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_81C.s")

// offset: 0xB34 | func: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_B34.s")

// offset: 0xC84 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_C84.s")

// offset: 0xE60 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_E60.s")

// offset: 0xED4 | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_ED4.s")

// offset: 0xF78 | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_F78.s")

// offset: 0x1008 | func: 11 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1008.s")

// offset: 0x1120 | func: 12 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1120.s")

// offset: 0x11BC | func: 13 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_11BC.s")

// offset: 0x1280 | func: 14 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1280.s")

// offset: 0x12F4 | func: 15 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_12F4.s")

// offset: 0x13E4 | func: 16 | export: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_13E4.s")

// offset: 0x13F4 | func: 17 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_13F4.s")

// offset: 0x15D0 | func: 18 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_15D0.s")

// offset: 0x1600 | func: 19 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1600.s")

// offset: 0x162C | func: 20 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_162C.s")

// offset: 0x1824 | func: 21 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1824.s")

// offset: 0x1978 | func: 22 | export: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1978.s")

// offset: 0x1AC4 | func: 23
void dll_18_func_1AC4(Object* arg0, DLL18_Data* arg1) {
    f32 temp_fv0;
    f32 temp_fv1;
    s32 var_v0;
    s32 var_v1;

    temp_fv0 = arg1->unk288;
    temp_fv1 = arg1->unk284;
    arg1->unk294 = arg1->unk290;
    arg1->unk290 = sqrtf((temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1));
    if (arg1->unk290 > 65.0f) {
        arg1->unk290 = 65.0f;
    }
    arg1->unk290 /= 65.0f;
    _bss_4 = arctan2_f(arg1->unk288, -arg1->unk284);
    _bss_4 -= arg1->unk324;
    var_v1 = _bss_4;
    var_v1 -= (arg0->srt.yaw & 0xFFFF);
    if (var_v1 >= 0x8001) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    arg1->unk32A = (s16) (s32) ((f32) var_v1 / 182.04f);
    if (var_v1 < 0) {
        arg1->unk328 = -arg1->unk32A;
    } else {
        arg1->unk328 = arg1->unk32A;
    }
    var_v0 = 0x8000 - -var_v1;
    if (var_v0) {}
    var_v0 += 0x2000;
    if (arg1->unk290 < 0.05f) {
        arg1->unk33F = 0;
        return;
    }
    if (var_v0 < 0) {
        var_v0 += 0xFFFF;
    }
    if (var_v0 >= 0x10000) {
        var_v0 += 0xFFFF0001;
    }
    arg1->unk33F = 4 - (var_v0 / 16384);
}

// offset: 0x1C70 | func: 24
void dll_18_func_1C70(Object* arg0, DLL18_Data* arg1, f32 arg2) {
    SRT sp88;
    MtxF sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    if (!(arg1->unk0 & 0x02000000)) {
        if (!(arg1->unk0 & 0x200000)) {
            arg0->speed.y *= 0.97f;
            arg0->speed.y -= arg1->unk29C * arg2;
        }
        
        if (!(arg1->unk340 & 1) || (arg1->unk340 & 4)) {
            sp88.yaw = arg0->srt.yaw;
            sp88.pitch = arg0->srt.pitch;
            sp88.roll = 0;
            sp88.transl.x = 0;
            sp88.transl.y = 0;
            sp88.transl.z = 0;
            sp88.scale = 1.0f;
            matrix_from_srt(&sp48, &sp88);
            if (arg1->unk0 & 0x10000) {
                vec3_transform(&sp48, arg1->unk27C, arg1->unk280, -arg1->unk278, &sp40, &arg0->speed.y, &sp3C);
            } else {
                vec3_transform(&sp48, arg1->unk27C, 0.0f, -arg1->unk278, &sp40, &sp44, &sp3C);
            }
            arg0->speed.x = sp40;
            arg0->speed.z = sp3C;
        }
        obj_integrate_speed(arg0, arg0->speed.x * arg2, arg0->speed.y * arg2, arg0->speed.z * arg2);
    }
}

// offset: 0x1E30 | func: 25
void dll_18_func_1E30(Object* arg0, DLL18_Data* arg1) {
    MtxF sp50;
    SRT sp38;

    sp38.yaw = arg0->srt.yaw;
    sp38.pitch = arg0->srt.pitch;
    sp38.roll = arg0->srt.roll;
    sp38.transl.x = 0;
    sp38.transl.y = 0;
    sp38.transl.z = 0;
    sp38.scale = 1.0f;
    matrix_from_srt(&sp50, &sp38);
    
    vec3_transform(&sp50, 0.0f, 0.0f, 1.0f, &arg1->unk274[0].x, &arg1->unk274[0].y, &arg1->unk274[0].z);
    vec3_transform(&sp50, 0.0f, 1.0f, 0.0f, &arg1->unk274[1].x, &arg1->unk274[1].y, &arg1->unk274[1].z);
    vec3_transform(&sp50, 1.0f, 0.0f, 0.0f, &arg1->unk274[2].x, &arg1->unk274[2].y, &arg1->unk274[2].z);
}

// offset: 0x1F64 | func: 26 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1F64.s")

// offset: 0x20CC | func: 27 | export: 6
void dll_18_func_20CC(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 temp_fv1;
    f32 temp_fa1;
    f32 temp_fv0;

    temp_fv1 = arg0->srt.transl.x - arg2;
    temp_fa1 = arg0->srt.transl.z - arg3;
    temp_fv0 = sqrtf((temp_fv1 * temp_fv1) + (temp_fa1 * temp_fa1));
    arg1->unk2B4 = temp_fv0;
    if (temp_fv0 != 0.0f) {
        temp_fv1 /= temp_fv0;
        temp_fa1 /= temp_fv0;
    }
    if ((arg4 + arg5) < arg1->unk2B4) {
        arg1->unk288 = temp_fv1 * arg6;
        arg1->unk284 = -temp_fa1 * arg6;
    } else {
        arg1->unk28C *= 0.9f;
        arg1->unk288 = 0.0f;
        arg1->unk284 = 0.0f;
    }
    if (arg1->unk288 > 65.0f) {
        arg1->unk288 = 65.0f;
    }
    if (arg1->unk288 < -65.0f) {
        arg1->unk288 = -65.0f;
    }
    if (arg1->unk284 > 65.0f) {
        arg1->unk284 = 65.0f;
    }
    if (arg1->unk284 < -65.0f) {
        arg1->unk284 = -65.0f;
    }
}
