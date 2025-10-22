#include "common.h"
#include "sys/curves.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"

struct DLL18_Data;

typedef s16 (*dll18_callback)(Object*, struct DLL18_Data*,f32);
typedef s16 (*dll18_callback2)(Object*, struct DLL18_Data*);

// size:0x34C
typedef struct DLL18_Data {
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
    dll18_callback2 unk2FC;
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
    s8 unk338;
    s8 unk339;
    s8 unk33A;
    u8 _unk33B[0x33F - 0x33B];
    u8 unk33F;
    s8 unk340;
    u8 unk341;
    u8 _unk342[0x34C - 0x342];
} DLL18_Data;

/*0x0*/ static SRT *_data_0 = NULL;

/*0x0*/ static s8 _bss_0;
/*0x1*/ static s8 _bss_1;
/*0x2*/ static s8 _bss_2;
/*0x4*/ static s16 _bss_4;
/*0x6*/ static u8 _bss_6[0x2];
/*0x8*/ static f32 _bss_8;
/*0xC*/ static f32 _bss_C;
/*0x10*/ static u8 _bss_10;
/*0x14*/ static s32 _bss_14;
/*0x18*/ static s32 _bss_18;
/*0x1C*/ static s8 _bss_1C;

void dll_18_func_13E4(DLL18_Data* arg0, void *arg1);
void dll_18_func_1F64(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
/*static*/ void dll_18_func_81C(Object* arg0, DLL18_Data* arg1, f32 arg2, dll18_callback *arg3);
/*static*/ void dll_18_func_B34(Object* arg0, DLL18_Data* arg1, f32 arg2, dll18_callback *arg3);
/*static*/ void dll_18_func_1AC4(Object* arg0, DLL18_Data* arg1);
/*static*/ void dll_18_func_1C70(Object* arg0, DLL18_Data* arg1, f32 arg2);
/*static*/ void dll_18_func_1E30(Object* arg0, DLL18_Data* arg1);

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
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_90.s")
#else
void dll_18_func_90(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3, dll18_callback *arg4, dll18_callback *arg5) {
    s8 sp2F;
    DLL27_Data* temp_a1;
    Object* temp_a0;
    Object* temp_v0;
    ObjectHitInfo* temp_v0_3;
    SRT* temp_v0_2;
    f32 temp_fa1;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv0_3;
    f32 temp_fv0_4;
    f32 temp_fv1;
    f32 temp_fv1_2;
    f32 var_fv1;
    s8 temp_v0_4;
    u32 temp_v1;

    _bss_2 = 0;
    sp2F = arg1->unk341;
    if (arg1->unk2C8 != NULL) {
        temp_fv0 = arg1->unk2C8->srt.transl.f[0] - arg0->srt.transl.f[0];
        temp_fv1 = arg1->unk2C8->srt.transl.f[2] - arg0->srt.transl.f[2];
        arg1->unk2B8 = sqrtf((temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1));
    } else {
        arg1->unk2B8 = 0.0f;
    }
    if ((arg1->unk0 & 0x8000) && (arg0->unk0xc0 == NULL)) {
        dll_18_func_B34(arg0, arg1, arg2, arg5);
        arg1->unk322 += arg2;
        if ((f32) arg1->unk322 > 10000.0f) {
            arg1->unk322 = 10000;
        }
    }
    arg1->unk0 |= 0x8000;
    if (arg1->unk274 != NULL) {
        dll_18_func_1E30(arg0, arg1);
    }
    dll_18_func_1AC4(arg0, arg1);
    arg1->unk0 &= ~0x200000;
    arg1->unk341 = 0;
    _bss_1C = 0;
    arg1->unk0 &= ~0x80000;
    arg1->unk340 = 0;
    _bss_1 = 0;
    dll_18_func_81C(arg0, arg1, arg2, arg4);
    arg1->unk32C += arg2;
    if ((f32) arg1->unk32C > 10000.0f) {
        arg1->unk32C = 10000;
    }
    _bss_8 = arg0->srt.transl.f[0];
    _bss_C = arg0->srt.transl.f[2];
    if (!(arg1->unk0 & 0x01000000)) {
        dll_18_func_1C70(arg0, arg1, arg2);
    }
    temp_v0_2 = _data_0;
    if (temp_v0_2 != NULL) {
        temp_fa1 = temp_v0_2->transl.f[0] - _bss_8;
        temp_ft4 = temp_v0_2->transl.f[2] - _bss_C;
        temp_fv0_2 = sqrtf((temp_fa1 * temp_fa1) + (temp_ft4 * temp_ft4));
        if (temp_fv0_2 < 10.0f) {
            temp_fv0_3 = arg0->srt.transl.f[0] - _bss_8;
            temp_fv1_2 = arg0->srt.transl.f[2] - _bss_C;
            temp_fv0_4 = sqrtf((temp_fv0_3 * temp_fv0_3) + (temp_fv1_2 * temp_fv1_2));
            var_fv1 = temp_fv0_4;
            if (temp_fv0_4 < 0.1f) {
                var_fv1 = 0.1f;
            }
            if (temp_fv0_2 < 1.0f) {
                arg0->srt.transl.f[0] = temp_v0_2->transl.f[0];
                arg0->srt.transl.f[2] = temp_v0_2->transl.f[2];
            } else {
                if (temp_fv0_2 < var_fv1) {
                    var_fv1 = temp_fv0_2;
                }
                arg0->srt.transl.f[0] = ((temp_fa1 / temp_fv0_2) * var_fv1) + _bss_8;
                arg0->srt.transl.f[2] = ((temp_ft4 / temp_fv0_2) * var_fv1) + _bss_C;
            }
        }
    }
    _data_0 = NULL;
    if (!(arg1->unk0 & 0x01000000) && !(arg1->unk0 & 0x400000)) {
        gDLL_27->vtbl->func_1e8(arg0, &arg1->unk4, arg2);
        gDLL_27->vtbl->func_5a8(arg0, &arg1->unk4);
        gDLL_27->vtbl->func_624(arg0, &arg1->unk4, arg3);
        if (arg1->unk4.unk25C & 0x10) {
            arg1->unk0 |= 0x40000;
        } else {
            arg1->unk0 &= ~0x40000;
        }
        if (arg1->unk0 & 0x800000) {
            if ((arg1->unk4.unk25C & 2) || (arg1->unk4.hitsTouchBits != 0)) {
                arg0->speed.f[0] = (arg0->srt.transl.f[0] - arg0->objhitInfo->unk_0x10.f[0]) / arg2;
                arg0->speed.f[2] = (arg0->srt.transl.f[2] - arg0->objhitInfo->unk_0x10.f[2]) / arg2;
            }
            arg1->unk0 &= ~0x800000;
        }
    }
    if (arg1->unk2C8 != NULL) {
        if (sp2F != arg1->unk341) {
            obj_send_mesg(arg1->unk2C8, 0xB, arg0, (void* ) arg1->unk341);
        }
    }
}
#endif

// offset: 0x5E0 | func: 2 | export: 2
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_5E0.s")
#else
// needs dll_18_func_81C to be static
void dll_18_func_5E0(Object* arg0, DLL18_Data* arg1, s32 arg2) {
    f32 sp24;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;

    if (arg1->unk340 & 1) {
        sp24 = fsin16_precise(arg0->srt.yaw);
        temp_fv0 = fcos16_precise(arg0->srt.yaw);
        if (arg1->unk340 & 8) {
            arg1->unk278 = (-arg0->speed.z * temp_fv0) - (arg0->speed.x * sp24);
            arg1->unk28C = arg1->unk278;
        } else {
            arg1->unk27C = (arg0->speed.x * temp_fv0) - (arg0->speed.z * sp24);
            arg1->unk278 = (-arg0->speed.z * temp_fv0) - (arg0->speed.x * sp24);
            if (arg1->unk340 & 4) {
                arg1->unk28C = sqrtf(SQ(arg0->speed.x) + SQ(arg0->speed.z));
            }
        }
        arg1->unk340 = 0;
        arg1->unk0 |= 0x80000;
        _bss_1C = 1;
        _bss_1 = 0;
        _bss_2 = 1;
        dll_18_func_81C(arg0, arg1, delayFloat, (s16 (**)(Object*, DLL18_Data*, f32)) arg2);
    }
}
#endif

// offset: 0x75C | func: 3 | export: 3
void dll_18_func_75C(SRT *arg0) {
    _data_0 = arg0;
}

// offset: 0x778 | func: 4 | export: 4
void dll_18_func_778(Object* arg0, DLL18_Data* arg1, u32 arg2) {
    dll18_callback2 temp_v1;
    s16 var_v0;

    var_v0 = arg1->unk26C;
    if (arg2 != var_v0) {
        temp_v1 = arg1->unk2FC;
        if (temp_v1 != NULL) {
            temp_v1(arg0, arg1);
            arg1->unk2FC = NULL;
            var_v0 = arg1->unk26C;
        }
        arg1->unk26E = var_v0;
        arg1->unk26C = (s16) arg2;
    }
    arg1->unk32C = 0;
    arg1->unk272 = 1;
    arg1->unk341 = 0;
    arg1->unk340 = 0;
    arg1->_unk342[8] = 0;
    arg1->unk270 = 0;
    if (arg0->objhitInfo != NULL) {
        arg0->objhitInfo->unk_0x61 = 0;
    }
}

// offset: 0x81C | func: 5
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_81C.s")
#else
void dll_18_func_81C(Object* arg0, DLL18_Data* arg1, f32 arg2, dll18_callback *arg3) {
    UnkFunc_80024108Struct sp50;
    ObjectHitInfo* temp_v0_4;
    ObjectHitInfo* temp_v0_6;
    UnkFunc_80024108Struct* var_v0;
    s32 temp_s2;
    s16 temp_t0;
    s16 temp_v0;
    s32 temp_v0_2;
    s16 temp_v0_7;
    s16 temp_v1;
    u32 var_s1;
    s32 var_s5;
    s32 var_s7;
    s32 var_v1;
    dll18_callback2 temp_v0_3;
    dll18_callback2 temp_v0_5;
    dll18_callback callback;

    var_s7 = 0;
    var_s5 = 0;
    _bss_0 = 0;
    _bss_10 = 0;
    if (arg1->unk26C != arg1->unk26E) {
        arg1->unk272 = 1;
        arg1->unk32C = 0;
    }
    do {
        temp_v0 = arg1->unk26C;
        var_s1 = 0;
        callback = arg3[temp_v0];
        temp_v0_2 = callback(arg0, arg1, arg2);
        if (temp_v0_2 > 0) {
            if (arg1->unk2FC != NULL) {
                arg1->unk2FC(arg0, arg1);
                arg1->unk2FC = NULL;
            }
            temp_t0 = arg1->unk26C;
            arg1->unk26C = temp_v0_2 - 1;
            arg1->unk272 = 1;
            arg1->unk32C = 0;
            arg1->unk341 = 0;
            arg1->unk340 = 0;
            arg1->_unk342[8] = 0;
            arg1->unk270 = 0;
            arg1->unk26E = temp_t0;
            if (arg0->objhitInfo != NULL) {
                arg0->objhitInfo->unk_0x61 = 0;
            }
        } else if (temp_v0_2 < 0) {
            temp_s2 = -temp_v0_2;
            var_s7 = 1;
            if (temp_s2 != temp_v0) {
                if (arg1->unk2FC != NULL) {
                    arg1->unk2FC(arg0, arg1);
                    arg1->unk2FC = NULL;
                }
                arg1->unk26E = temp_v0;
                arg1->unk272 = 1;
                arg1->unk32C = 0;
                arg1->unk341 = 0;
                arg1->unk340 = 0;
                arg1->_unk342[8] = 0;
                arg1->unk270 = 0;
                if (arg0->objhitInfo != NULL) {
                    arg0->objhitInfo->unk_0x61 = 0;
                }
            }
            arg1->unk26C = temp_s2;
            var_s1 = 1;
        } else {
            var_s1 = 1;
        }
        var_s5 += 1;
        if (var_s5 >= 0x100) {
            var_s1 = 1;
        }
    } while (var_s1 == 0);
    arg1->unk26E = arg1->unk26C;
    if (var_s7 == 0) {
        arg1->unk272 = 0;
    }
    if ((_bss_10 == 0) && !(arg1->unk340 & 1)) {
        sp50.unk1B = 0;
        arg1->unk33A = func_80024108(arg0, arg1->unk298, arg2, &sp50);
        arg1->unk308 = 0;
        for (var_v1 = 0; var_v1 < sp50.unk1B; var_v1++) {
            arg1->unk308 |= 1 << sp50.unk13[var_v1];
        }
        func_80025780(arg0, arg2, &sp50, NULL);
        arg1->unk0 &= ~0x10000;
    }
    if (!(arg1->unk0 & 0x4000)) {
        arg0->srt.pitch -= (s32) ((f32) arg0->srt.pitch * arg2 * 0.125f);
        arg0->srt.roll -= (s32) ((f32) arg0->srt.roll * arg2 * 0.125f);
    }
}
#endif

// offset: 0xB34 | func: 6
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_B34.s")
#else
// regalloc
// https://decomp.me/scratch/HXVsw
void dll_18_func_B34(Object* arg0, DLL18_Data* arg1, f32 arg2, dll18_callback *arg3) {
    s16 temp_t0;
    s16 temp_v0;
    s16 temp_v0_2;
    s32 temp_v1;
    u32 var_s1;
    s32 var_s3;
    s32 var_s5;
    dll18_callback callback;

    var_s5 = 0;
    var_s3 = 0;
    if (arg1->unk268 != arg1->unk26A) {
        arg1->unk273 = 1;
        arg1->unk322 = 0;
    }
    do {
        temp_v0 = arg1->unk268;
        var_s1 = 0;
        callback = arg3[temp_v0];
        temp_v0_2 = callback(arg0, arg1, arg2);
        if (temp_v0_2 > 0) {
            temp_t0 = arg1->unk268;
            arg1->unk268 = temp_v0_2 - 1;
            arg1->unk273 = 1;
            arg1->unk322 = 0;
            arg1->unk26A = temp_t0;
        } else if (temp_v0_2 < 0) {
            temp_v1 = -temp_v0_2;
            var_s5 = 1;
            if (temp_v1 != temp_v0) {
                arg1->unk26A = temp_v0;
                arg1->unk273 = 1;
                arg1->unk322 = 0;
            } else {
                arg1->unk273 = 0;
            }
            arg1->unk268 = temp_v1;
            var_s1 = 1;
        } else {
            var_s1 = 1;
        }
        var_s3 += 1;
        if (var_s3 >= 0x100) {
            var_s1 = 1;
        }
    } while (var_s1 == 0);
    arg1->unk26A = arg1->unk268;
    if (var_s5 == 0) {
        arg1->unk273 = 0;
    }
}
#endif

// offset: 0xC84 | func: 7 | export: 7
void dll_18_func_C84(Object* arg0, DLL18_Data* arg1, f32 arg2, s32 arg3) {
    UnkFunc_80024108Struct sp34;
    s32 var_v1;

    sp34.unk12 = 0;
    arg1->unk33A = func_80024108(arg0, arg1->unk298, arg2, &sp34);
    arg1->unk308 = 0;
    for (var_v1 = 0; var_v1 < sp34.unk1B; var_v1++) {
        arg1->unk308 |= 1 << sp34.unk13[var_v1];
    }
    func_80025780(arg0, arg2, &sp34, NULL);
    arg1->unk0 &= ~0x10000;
    if (sp34.unk12 != 0) {
        if (arg3 & 0x10) {
            if (arg3 & 1) {
                arg1->unk2AC = -sp34.unk0[2];
            }
            if (arg3 & 2) {
                arg1->unk2AC = sp34.unk0[0];
            }
            if (arg3 & 4) {
                arg1->unk2AC = sp34.unk0[1];
            }
            if (arg3 & 8) {
                arg0->srt.yaw += sp34.unkc[1];
            }
        } else {
            if (arg3 & 1) {
                arg1->unk278 = -sp34.unk0[2] / arg2;
            }
            if (arg3 & 2) {
                arg1->unk27C = sp34.unk0[0] / arg2;
            }
            if (arg3 & 8) {
                arg0->srt.yaw += sp34.unkc[1];
            }
            if (arg3 & 4) {
                arg1->unk280 = sp34.unk0[1] / arg2;
                arg1->unk0 |= 0x10000;
            }
        }
    } else {
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
    }
    _bss_10 = 1;
}

// offset: 0xE60 | func: 8 | export: 8
void dll_18_func_E60(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3, f32 arg4) {
    if (arg1->unk290 > 0.1f) {
        arg0->srt.yaw += (((arg3 * arg2) / arg4) * 182.0f);
    }
}

// offset: 0xED4 | func: 9 | export: 9
void dll_18_func_ED4(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3) {
    f32 temp_fa0;
    f32 var_fv0;

    var_fv0 = arg1->unk2A4 + (arg3 * arg2);
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    temp_fa0 = var_fv0 - arg1->unk2A4;
    if (temp_fa0 > 0.0f) {
        arg0->srt.transl.x += arg1->unk2EC.x * temp_fa0;
        arg0->srt.transl.y += arg1->unk2EC.y * temp_fa0;
        arg0->srt.transl.z += arg1->unk2EC.z * temp_fa0;
        arg1->unk2A4 = var_fv0;
    }
}

// offset: 0xF78 | func: 10 | export: 10
void dll_18_func_F78(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3) {
    f32 temp_fa0;
    f32 var_fv0;

    var_fv0 = arg1->unk2A0 + (arg3 * arg2);
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    temp_fa0 = var_fv0 - arg1->unk2A0;
    if (temp_fa0 > 0.0f) {
        arg0->srt.yaw += (s16) (s32) (arg1->unk2F8 * temp_fa0);
        arg1->unk2A0 = var_fv0;
    }
}

// offset: 0x1008 | func: 11 | export: 11
void dll_18_func_1008(Object* arg0, DLL18_Data* arg1, s32 arg2, s32 arg3) {
    f32 var_fv0;
    f32 var_fv1;
    s32 var_v1;

    if (arg1->unk2C8 != NULL) {
        if (arg1->unk2C8->parent == arg0->parent) {
            var_fv0 = arg1->unk2C8->srt.transl.x - arg0->srt.transl.x;
            var_fv1 = arg1->unk2C8->srt.transl.z - arg0->srt.transl.z;
        } else {
            var_fv0 = arg0->positionMirror.x - arg1->unk2C8->positionMirror.x;
            var_fv1 = arg0->positionMirror.z - arg1->unk2C8->positionMirror.z;
        }
        var_v1 = arctan2_f(-var_fv0, -var_fv1) - (arg0->srt.yaw & 0xFFFF);
        if (var_v1 >= 0x8001) {
            var_v1 += 0xFFFF0001;
        }
        if (var_v1 < -0x8000) {
            var_v1 += 0xFFFF;
        }
        arg0->srt.yaw += (s32) (((f32) var_v1 * delayFloat) / ((f32) arg3 * 3.0f));
    }
}

// offset: 0x1120 | func: 12 | export: 12
void dll_18_func_1120(Object* arg0, DLL18_Data* arg1, s32 arg2, s32 arg3, DLL18_Func_11BC_Struct* arg4) {
    s32 temp_v0;

    temp_v0 = 1 << arg2;
    if (arg1->unk308 & temp_v0) {
        arg1->unk308 = arg1->unk308 & ~temp_v0;
        gDLL_6_AMSFX->vtbl->play_sound(arg0, arg4[arg3].soundID, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0x11BC | func: 13 | export: 13
void dll_18_func_11BC(Object* arg0, DLL18_Data* arg1, s32 arg2, s32 arg3, DLL18_Func_11BC_Struct *arg4, f32 arg5, u8 volume) {
    s32 temp_v0;
    u32 temp_v0_2;

    temp_v0 = 1 << arg2;
    if (arg1->unk308 & temp_v0) {
        arg1->unk308 = arg1->unk308 & ~temp_v0;
        temp_v0_2 = gDLL_6_AMSFX->vtbl->play_sound(arg0, arg4[arg3].soundID, volume, NULL, NULL, 0, NULL);
        if (temp_v0_2 != 0) {
            gDLL_6_AMSFX->vtbl->func_954(temp_v0_2, arg5);
        }
    }
}

// offset: 0x1280 | func: 14 | export: 14
void dll_18_func_1280(Object* arg0, DLL18_Data* arg1, s32 arg2) {
    arg1->unk330 = gDLL_26_Curves->vtbl->curves_func_1e4(
        arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, 
        &arg2, 1, (s32) arg1->unk338);
}

// offset: 0x12F4 | func: 15 | export: 15
void dll_18_func_12F4(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3) {
    CurveSetup* temp_v0;

    if (arg1->unk330 == -1) {
        arg1->unk2B4 = 0.0f;
        return;
    }
    temp_v0 = gDLL_26_Curves->vtbl->curves_func_39c(arg1->unk330);
    if (temp_v0 == NULL) {
        arg1->unk2B4 = 0.0f;
        return;
    }
    dll_18_func_1F64(arg0, arg1, temp_v0->base.x, temp_v0->base.z, arg2, 1);
    if (arg1->unk2B4 < 20.0f) {
        dll_18_func_13E4(arg1, NULL);
    }
}

// offset: 0x13E4 | func: 16 | export: 22
void dll_18_func_13E4(DLL18_Data* arg0, void *arg1) { }

// offset: 0x13F4 | func: 17 | export: 16
void dll_18_func_13F4(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3) {
    f32 var_fa0;
    f32 var_fa1;
    f32 sp2C;
    f32 temp_fv0_4;
    
    arg1->unk340 |= 1;
    if (_bss_1C == 0) {
        var_fa0 = -fsin16_precise(_bss_4) * arg1->unk290 * arg3;
        var_fa1 = -fcos16_precise(_bss_4) * arg1->unk290 * arg3;
        if (arg1->unk290 < 0.02f) {
            var_fa0 = 0.0f;
            var_fa1 = 0.0f;
        }
        arg0->speed.x = arg0->speed.x + (((var_fa0 - arg0->speed.x) * arg2) / arg1->unk2B0);
        arg0->speed.z = arg0->speed.z + (((var_fa1 - arg0->speed.z) * arg2) / arg1->unk2B0);
    } else {
        arg1->unk340 &= ~0x1;
    }
    arg1->unk28C = sqrtf(SQ(arg0->speed.x) + SQ(arg0->speed.z));
    if (arg1->unk28C < 0.04f) {
        arg1->unk28C = 0.0f;
        arg0->speed.x = 0.0f;
        arg0->speed.z = 0.0f;
    }
    sp2C = fsin16_precise(arg0->srt.yaw);
    temp_fv0_4 = fcos16_precise(arg0->srt.yaw);
    arg1->unk27C = (arg0->speed.x * temp_fv0_4) - (arg0->speed.z * sp2C);
    arg1->unk278 = (-arg0->speed.z * temp_fv0_4) - (arg0->speed.x * sp2C);
}

// offset: 0x15D0 | func: 18 | export: 17
void dll_18_func_15D0(Object* arg0, DLL18_Data* arg1) {
    arg0->speed.x = 0.0f;
    arg0->speed.z = 0.0f;
    arg1->unk28C = 0.0f;
    arg1->unk278 = 0.0f;
    arg1->unk27C = 0.0f;
}

// offset: 0x1600 | func: 19 | export: 18
void dll_18_func_1600(Object *arg0, DLL18_Data *arg1, s32 arg2, s32 arg3) {
    _bss_14 = arg2;
    _bss_18 = arg3;
}

// offset: 0x162C | func: 20 | export: 19
void dll_18_func_162C(Object* arg0, DLL18_Data* arg1, s32 arg2, s32 arg3) {
    f32 sp34;
    f32 sp30;
    s32 var_a2;

    if (_bss_1C != 0) {
        if ((arg1->unk278 > 0.0f) && (_bss_14 != arg0->curModAnimId)) {
            func_80023D30(arg0, _bss_14, arg0->animProgress, 0U);
            arg1->unk33A = 0;
        } else if ((arg1->unk278 < 0.0f) && (_bss_18 != arg0->curModAnimId)) {
            func_80023D30(arg0, _bss_18, arg0->animProgress, 0U);
            arg1->unk33A = 0;
        }
        sp30 = sqrtf(SQ(arg1->unk278) + SQ(arg1->unk27C));
        if (func_8002493C(arg0, sp30, &sp34) != 0) {
            arg1->unk298 = sp34;
        }
        if (sp30 != 0.0f) {
            sp34 = arg1->unk27C / sp30;
        } else {
            sp34 = 0.0f;
        }
        var_a2 = (s32) (sp34 * 1023.0f);
        if (var_a2 < 0) {
            var_a2 = -var_a2;
        }
        if (var_a2 >= 0x400) {
            var_a2 = 0x3FF;
        }
        if (arg1->unk27C > 0.0f) {
            func_80025540(arg0, arg3, var_a2);
        } else {
            func_80025540(arg0, arg2, var_a2);
        }
    }
}

// offset: 0x1824 | func: 21 | export: 20
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1824.s")
#else
// stack pos diff
// https://decomp.me/scratch/C9Nsu
void dll_18_func_1824(Object *arg0, DLL18_Data *arg1, s32 arg2, s32 arg3, u32 arg5) {
    DLL_Unknown* temp_s3;

    temp_s3 = dll_load_deferred((arg2 + 0x1000), 1);
    while (arg3 != 0) {
        if (arg5 == 0) {
            temp_s3->vtbl->func[0].withSixArgs((s32)arg0, 0, 0, 1, -1, 0);
        } else if (arg5 == 1) {
             temp_s3->vtbl->func[0].withSixArgs((s32)arg0, 0, 0, 2, -1, 0);
        } else if (arg5 == 2) {
             temp_s3->vtbl->func[0].withSixArgs((s32)arg0, 0, 0, 4, -1, 0);
        }
        arg3 -= 1;
    }
    dll_unload(temp_s3);
}
#endif

// offset: 0x1978 | func: 22 | export: 21
void dll_18_func_1978(Object* arg0, DLL18_Data *arg1, s32 arg2, s32 arg3, u32 arg4) {
    while ((arg3 != 0) && (arg0 != NULL)) {
        if (arg4 == 0) {
            gDLL_17->vtbl->func1(arg0, arg2, NULL, 2, -1, NULL);
        } else if (arg4 == 1) {
            gDLL_17->vtbl->func1(arg0, arg2, NULL, 2, -1, NULL);
        } else if (arg4 == 2) {
            gDLL_17->vtbl->func1(arg0, arg2, NULL, 4, -1, NULL);
        }
        arg3 -= 1;
    }
}

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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/18/dll_18_func_1F64.s")
#else
// regalloc
// https://decomp.me/scratch/pmJz3
void dll_18_func_1F64(Object* arg0, DLL18_Data* arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    f32 temp_fv1;
    f32 temp_fa1;
    f32 var_ft4;
    f32 temp;

    arg1->unk0 &= ~0x100000;
    temp_fv1 = arg0->srt.transl.x - arg2;
    temp_fa1 = arg0->srt.transl.z - arg3;
    arg1->unk2B4 = sqrtf(SQ(temp_fv1) + SQ(temp_fa1));
    if (1) {}
    var_ft4 = 65.0f;
    if (arg1->unk2B4 < 15.0f) {
        var_ft4 = arg1->unk2B4 * 3.0f;
        arg1->unk28C *= 0.9f;
    }
    temp = arg1->unk2B4;
    if (var_ft4 < temp) {
        temp_fv1 /= temp / var_ft4;
        temp_fa1 /= temp / var_ft4;
    }
    arg1->unk288 = temp_fv1 * arg4;
    arg1->unk284 = -temp_fa1 * arg4;
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
#endif

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
