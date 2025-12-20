#include "dlls/engine/4_race.h"
#include "sys/curves.h"
#include "sys/rand.h"
#include "unktypes.h"

/*0x0*/ static RaceStruct *_bss_0[10];
/*0x28*/ static RaceStruct *_bss_28[10];
/*0x50*/ static RaceStruct **_bss_50;
/*0x54*/ static RaceStruct **_bss_54;
/*0x58*/ static s16 _bss_58;
/*0x5A*/ static s16 _bss_5A;
/*0x60*/ static RaceCheckpoint _bss_60[200];
/*0x6A0*/ static s32 _bss_6A0;

void dll_4_func_64(void);
static s32 dll_4_func_1758(RaceCheckpointSetup* arg0, s32 arg1, Vec4f* arg2, Vec4f* arg3, Vec4f* arg4, u8 arg5, f32 arg6, f32 arg7);
RaceCheckpointSetup *dll_4_func_1F60(s32 checkpointUID, s32 *arg1);

// offset: 0x0 | ctor
void dll_4_ctor(void* dll) {
    dll_4_func_64();
    _bss_50 = _bss_0;
    _bss_54 = _bss_28;
}

// offset: 0x58 | dtor
void dll_4_dtor(void *dll) { }

// offset: 0x64 | func: 0 | export: 0
void dll_4_func_64(void) {
    _bss_6A0 = 0;
}

// offset: 0x80 | func: 1 | export: 1
void dll_4_func_80(RaceCheckpointSetup* arg0) {
    s32 i;
    s32 temp;

    i = 0;
    while (i < _bss_6A0 && _bss_60[i].uID < arg0->uID) {
        i += 1;
    }

    temp = _bss_6A0;
    while (temp > i) {
        _bss_60[temp].setup = _bss_60[temp - 1].setup;
        _bss_60[temp].uID = _bss_60[temp - 1].uID;
        temp--;
    }

    _bss_6A0 += 1;
    _bss_60[i].setup = arg0;
    _bss_60[i].uID = arg0->uID;
}

// offset: 0x150 | func: 2 | export: 2
void dll_4_func_150(RaceCheckpointSetup* arg0) {
    s32 i;

    i = 0;
    while (i < _bss_6A0 && arg0->uID != _bss_60[i].uID) {
        i++;
    }

    if (i < _bss_6A0) {
        _bss_6A0--;

        while (i < _bss_6A0) {
            i++;
            _bss_60[i - 1].setup = _bss_60[i].setup;
            _bss_60[i - 1].uID = _bss_60[i].uID;
        }
    }
}

// offset: 0x200 | func: 3 | export: 3
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/4_race/dll_4_func_200.s")
#else
void dll_4_func_200(Object* arg0, RaceStruct* arg1, s32 arg2) {
    f32 sp2CC;
    f32 sp2C8;
    RaceCheckpointSetup* temp_s0;
    RaceCheckpointSetup* temp_v0_3;
    Vec3f sp2B4;
    f32 pad_sp2B0;
    f32 temp_fa1;
    f32 pad_sp2A8;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fs3;
    f32 temp_fs4;
    f32 pad_sp290;
    f32 pad_sp28C;
    f32 temp_ft4;
    f32 sp284;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 sp278;
    f32 var_ft1;
    f32 pad_sp274;
    f32 var_fv1;
    s32 var_s2;
    s32 var_s3;
    s32 var_s5;
    s32 var_v0;
    s32 sp258;
    s32 sp254;
    s32 sp154[0x40]; // unknwon size
    s8 sp8C[0xC8]; // unknown size

    var_s5 = 0;
    for (var_s2 = 0; var_s2 < _bss_6A0; var_s2++) {
        sp8C[var_s2] = FALSE;
    }
    if (dll_4_func_1F60(arg1->unk10, &sp258) != NULL) {
        var_s5 = 1;
        sp154[0] = sp258;
    } else {
        for (var_s2 = 0; var_s2 < _bss_6A0; var_s2++) {
            temp_s0 = _bss_60[var_s2].setup;
            if (sp8C[var_s2] != FALSE) {
                continue;
            }

            if ((arg2 == -1) || (arg2 == temp_s0->unk28)) {
                sp2B4.z = temp_s0->pos.x - arg0->srt.transl.x;
                sp2B4.y = temp_s0->pos.y - arg0->srt.transl.y;
                sp2B4.x = temp_s0->pos.z - arg0->srt.transl.z;
                if ((SQ(sp2B4.z) + SQ(sp2B4.y) + SQ(sp2B4.x)) < 409600.0f) { // 640 * 640
                    sp154[var_s5] = var_s2;
                    var_s5++;
                    for (var_v0 = var_s2; var_v0 < _bss_6A0; var_v0++) {
                        if (arg2 == _bss_60[var_v0].setup->unk28) {
                            sp8C[var_v0] = TRUE;
                        }
                    }
                }
            }
        }
    }
    for (var_s2 = 0; var_s2 < _bss_6A0; var_s2++) {
        sp8C[var_s2] = FALSE;
    }

    // probably not correct to do a while (1) here
    while (1) {
        if (var_s5 > 0) {
            var_s5 -= 1;
            sp258 = sp154[var_s5];
            temp_s0 = _bss_60[sp258].setup;
        } else {
            arg1->unk10 = -1;
            return;
        }

        if (temp_s0 == NULL) {
            return;
        }

        for (var_s3 = 0; var_s3 < 2; var_s3++) {
            temp_v0_3 = dll_4_func_1F60(temp_s0->unk20[var_s3], &sp254);
            if (temp_v0_3 == NULL) {
                continue;
            }

            temp_fs3 = fsin16_precise(temp_s0->unk29 << 8);
            sp284 = fcos16_precise(temp_s0->unk29 << 8);
            temp_fs0 = -((temp_s0->pos.x * temp_fs3) + (temp_s0->pos.z * sp284));
            temp_fs4 = fsin16_precise(temp_v0_3->unk29 << 8);
            sp278 = fcos16_precise(temp_v0_3->unk29 << 8);
            temp_fa1 = -((temp_v0_3->pos.x * temp_fs4) + (temp_v0_3->pos.z * sp278));
            sp2CC = (arg0->srt.transl.x * temp_fs3) + (sp284 * arg0->srt.transl.z) + temp_fs0;
            sp2C8 = (arg0->srt.transl.x * temp_fs4) + (sp278 * arg0->srt.transl.z) + temp_fa1;
            temp_ft4 = (temp_v0_3->pos.x * temp_fs3) + (sp284 * temp_v0_3->pos.z) + temp_fs0;
            temp_ft5 = temp_s0->pos.x;
            temp_ft5 = (temp_ft5 * temp_fs4) + (sp278 * temp_s0->pos.z) + temp_fa1;
            if (
            (
                (temp_ft4 <= 0.0f && sp2CC <= 0.0f) || (temp_ft4 > 0.0f && sp2CC > 0.0f)
            ) && (
                (temp_ft5 <= 0.0f && sp2C8 <= 0.0f) || (temp_ft5 > 0.0f && sp2C8 > 0.0f)
            )
            ) {
                temp_fs1 = temp_s0->pos.x - temp_v0_3->pos.x;
                temp_fs0 = temp_s0->pos.y - temp_v0_3->pos.y;
                temp_fs2 = temp_s0->pos.z - temp_v0_3->pos.z;
                temp_fv0 = sqrtf(SQ(temp_fs1) + SQ(temp_fs0) + SQ(temp_fs2));
                if (temp_fv0 > 0.0) {
                    temp_fv0 = 1.0f / temp_fv0;
                    sp2B4.z = temp_fs1 * temp_fv0;
                    sp2B4.x = temp_fs2 * temp_fv0;
                }

                temp_fa1 = sp2B4.z;
                var_ft1 = sp2B4.x;
                temp_fv0 = (-sp2CC / ((temp_fs3 * temp_fa1) + (sp284 * var_ft1))) + (sp2C8 / ((temp_fs4 * temp_fa1) + (sp278 * var_ft1)));
                if ((temp_fv0 > 0.1f) || (temp_fv0 < -0.1f)) {
                    var_fv1 = (-sp2CC / ((temp_fs3 * temp_fa1) + (sp284 * var_ft1))) / temp_fv0;
                } else {
                    var_fv1 = 0.0f;
                }
                if (var_fv1 < 0.0f) {
                    var_fv1 = 0.0f;
                }
                if (var_fv1 >= 0.999f) {
                    var_fv1 = 0.999f;
                }
                temp_fv0 = (f32)temp_s0->unk2A * 1;
                var_ft1 = (f32)temp_v0_3->unk2A * 1;
                temp_fs1 = temp_s0->pos.x - (temp_fs1 * var_fv1);
                temp_fs0 = temp_s0->pos.y - (temp_fs0 * var_fv1);
                temp_fs2 = temp_s0->pos.z - (temp_fs2 * var_fv1);
                temp_fa1 = ((var_ft1 - temp_fv0) * var_fv1) + temp_fv0;
                if (1) { }
                temp_ft4 = (arg0->srt.transl.y - temp_fs0) / (temp_fa1);
                temp_fv0 = ((arg0->srt.transl.x * sp2B4.x) - (arg0->srt.transl.z * sp2B4.z)) - ((temp_fs1 * sp2B4.x) - (temp_fs2 * sp2B4.z));
                temp_fv0 /= temp_fa1;
                if (!(temp_fv0 < -8.0f) && !(temp_fv0 > 8.0f) && !(temp_ft4 < -4.0f) && !(temp_ft4 > 8.0f)) {
                    arg1->unk10 = temp_s0->uID;
                    arg1->unk14 = temp_s0->uID;
                    arg1->unk0 = temp_fv0;
                    arg1->unk4 = temp_ft4;
                    arg1->unk8 = var_fv1;
                    arg1->unk20 = temp_s0->unk28;
                    return;
                }
            }
        }

        if (sp8C[sp258] == FALSE) {
            for (var_s3 = 1; var_s3 >= 0; var_s3--) {
                if ((dll_4_func_1F60(temp_s0->unk18[var_s3], &sp254) != NULL) && (sp8C[sp254] == FALSE) && (var_s5 < 60)) {
                    sp154[var_s5] = sp254;
                    var_s5++;
                }
                if ((dll_4_func_1F60(temp_s0->unk20[var_s3], &sp254) != NULL) && (sp8C[sp254] == FALSE) && (var_s5 < 60)) {
                    sp154[var_s5] = sp254;
                    var_s5++;
                }
            }
            sp8C[sp258] = TRUE;
        }
    }
}
#endif

// offset: 0x9B0 | func: 4 | export: 4
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/4_race/dll_4_func_9B0.s")
#else
s32 dll_4_func_9B0(Object* arg0, RaceStruct* arg1) {
    f32 sp84;
    f32 sp80;
    f32 pad;
    Vec3f sp70;
    f32 pad3;
    f32 temp_fv1; // sp68?
    f32 temp_fv0; // sp64?
    f32 temp_fa1; // sp60?
    RaceCheckpointSetup* sp5C;
    RaceCheckpointSetup* sp58;
    s32 sp54;
    s32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 var_fa0;
    s16 sp36;

    if (arg1->unk18 < 0) {
        arg1->unk1C = 0;
        arg1->unkC = 0.0f;
        if (arg1->unk10 < 0) {
            return 0;
        }
        arg1->unk18 = arg1->unk10;
    }

    sp5C = dll_4_func_1F60(arg1->unk18, &sp50);
    if (sp5C == NULL) {
        arg1->unk18 = -1;
        return 0;
    }
    sp4C = fsin16_precise((s16) (sp5C->unk29 << 8));
    sp48 = fcos16_precise((s16) (sp5C->unk29 << 8));
    sp44 = -((sp5C->pos.x * sp4C) + (sp5C->pos.z * sp48));
    sp84 = (arg0->srt.transl.x * sp4C) + (sp48 * arg0->srt.transl.z) + sp44;
    if ((sp5C->unk18[0] >= 0) && (sp84 >= 0.0f)) {
        arg1->unk18 = sp5C->unk18[0];
        arg1->unkC = 0.99f;
        arg1->unk1C -= 1;
        return sp5C->unk29;
    }
    if (sp5C->unk20[0] < 0) {
        return sp5C->unk29;
    }
    sp58 = dll_4_func_1F60(sp5C->unk20[0], &sp54);
    var_fa0 = sp58->pos.x - sp5C->pos.x;
    sp40 = sp58->pos.z - sp5C->pos.z;
    temp_fa1 = sp40;
    sp36 = arctan2_f(var_fa0, temp_fa1);
    sp40 = fsin16_precise((s16) (sp58->unk29 << 8));
    sp3C = fcos16_precise((s16) (sp58->unk29 << 8));
    temp_fa1 = -((sp58->pos.x * sp40) + (sp58->pos.z * sp3C));
    sp80 = (arg0->srt.transl.x * sp40) + (sp3C * arg0->srt.transl.z) + temp_fa1;
    if (sp80 < 0.0f) {
        arg1->unk18 = sp5C->unk20[0];
        arg1->unkC = 0.0f;
        arg1->unk1C += 1;
        return sp36;
    }
    temp_fv1 = (sp58->pos.x * sp4C) + (sp48 * sp58->pos.z) + sp44;
    var_fa0 = (sp5C->pos.x * sp40) + (sp3C * sp5C->pos.z) + temp_fa1;
    if ((((temp_fv1 < 0.0f) && (sp84 < 0.0f)) || ((temp_fv1 >= 0.0f) && (sp84 >= 0.0f))) && (((var_fa0 <= 0.0f) && (sp80 <= 0.0f)) || ((var_fa0 > 0.0f) && (sp80 > 0.0f)))) {
        temp_fv1 = sp5C->pos.x - sp58->pos.x;
        temp_fv0 = sp5C->pos.y - sp58->pos.y;
        temp_fa1 = sp5C->pos.z - sp58->pos.z;
        temp_fv0 = sqrtf(SQ(temp_fv1) + SQ(temp_fv0) + SQ(temp_fa1));
        if (temp_fv0 > 0.0f) {
            var_fa0 = 1.0f / temp_fv0;
            sp70.f[1] = temp_fv1 * var_fa0;
            sp70.f[0] = temp_fa1 * var_fa0;
        }
        
        temp_fa1 = (sp4C * sp70.y) + (sp48 * sp70.x);
        if ((temp_fa1 > -0.01f) && (temp_fa1 < 0.01f)) {
            return sp36;
        }

        temp_fv1 = -sp84 / temp_fa1;
        temp_fv0 = (sp40 * sp70.y) + (sp3C * sp70.x);
        // 0.02f might be incorrect _rodata_70
        if ((temp_fv0 > -0.02f) && (temp_fv0 < 0.01f)) {
            return sp36;
        }

        var_fa0 = (sp80 / temp_fv0);
        temp_fa1 = temp_fv1 + var_fa0;
        temp_fv1 = temp_fa1 != 0.0f ? temp_fv1 / temp_fa1 : 0.0f;
        arg1->unkC = temp_fv1;
        if (arg1->unkC < 0.0f) {
            arg1->unkC = 0.0f;
        }
        if (arg1->unkC >= 0.999f) {
            arg1->unkC = 0.999f;
        }
    }
    return sp36;
}
#endif

// offset: 0xEA4 | func: 5 | export: 10
void dll_4_func_EA4(RaceStruct* arg0) {
    if (_bss_58 < 10) {
        _bss_50[_bss_58] = arg0;
        _bss_58 += 1;
    }
}

// offset: 0xEF0 | func: 6 | export: 9
void dll_4_func_EF0(RaceStruct* arg0) {
    s32 sp2C;
    RaceCheckpointSetup* temp_v0;

    temp_v0 = dll_4_func_1F60(arg0->unk10, &sp2C);
    if (temp_v0 == NULL) {
        arg0->unk18 = 0;
        arg0->unkC = 0.0f;
        return;
    }
    while (temp_v0->unk18[0] >= 0) {
        temp_v0 = dll_4_func_1F60(temp_v0->unk18[0], &sp2C);
        arg0->unk1C += 1;
    }
    arg0->unk18 = arg0->unk10;
    arg0->unkC = 0.0f;
}

// offset: 0xFB4 | func: 7 | export: 11
RaceStruct** dll_4_func_FB4(s32* arg0) {
    *arg0 = (s32) _bss_5A;
    return _bss_54;
}

// offset: 0xFDC | func: 8 | export: 14
void dll_4_func_FDC(void) {
    RaceStruct** temp_t6;

    temp_t6 = _bss_54;
    _bss_54 = _bss_50;
    _bss_50 = temp_t6;
    _bss_5A = _bss_58;
    _bss_58 = 0;
}

// offset: 0x1028 | func: 9 | export: 13
RaceStruct* dll_4_func_1028(s32 arg0) {
    RaceStruct* temp_a0;
    RaceStruct* temp_t0;
    s32 var_a3;
    s32 var_t1;
    s32 var_v0;

    for (var_v0 = 0; var_v0 < _bss_5A; var_v0++) {
        temp_a0 = _bss_54[var_v0];
        var_a3 = 1;

        for (var_t1 = 0; var_t1 < _bss_5A; var_t1++) {
            temp_t0 = _bss_54[var_t1];
            if (temp_t0 != temp_a0) {
                if (temp_a0->unk1C < temp_t0->unk1C) {
                    var_a3 += 1;
                } else if ((temp_t0->unk1C == temp_a0->unk1C) && (temp_a0->unkC < temp_t0->unkC)) {
                    var_a3 += 1;
                }
            }
        }
        if (var_a3 == arg0) {
            return temp_a0;
        }
    }

    return NULL;
}

// offset: 0x1100 | func: 10 | export: 12
s32 dll_4_func_1100(RaceStruct* arg0) {
    s32 var_a0;
    s32 var_v1;
    RaceStruct* temp_v0;

    var_v1 = 1;
    for (var_a0 = 0; var_a0 < _bss_5A; var_a0++) {
        temp_v0 = _bss_54[var_a0];
        if (temp_v0 != arg0) {
            if (arg0->unk1C < temp_v0->unk1C) {
                var_v1 += 1;
            } else if ((temp_v0->unk1C == arg0->unk1C) && (arg0->unkC < temp_v0->unkC)) {
                var_v1 += 1;
            }
        }
    }
    return var_v1;
}

// offset: 0x119C | func: 11 | export: 5
s32 dll_4_func_119C(SRT* arg0, RaceStruct* arg1, f32 arg2, s32 arg3, u8 arg4, u8 arg5) {
    f32 temp;
    s32 var_s5;
    Vec4f spF0;
    Vec4f spE0;
    Vec4f spD0;
    RaceCheckpointSetup* temp_v0;
    f32 temp_fs2;
    f32 temp_fs3;
    f32 temp_fs5;
    f32 var_fs0;
    f32 spB8;
    f32 spB4;
    f32 spB0;
    f32 var_fv1;
    s8 var_s1;
    s8 var_s2;
    s32 pad[3];
    s32 sp98;
    s32 sp94;
    s32 sp90;

    var_s5 = 0;
    while (var_s5 < 3) {
        if (arg1->unk10 < 0) {
            return 1;
        }
        temp_v0 = dll_4_func_1F60(arg1->unk10, &sp90);
        if (temp_v0 == NULL) {
            return 1;
        }
        if (temp_v0->unk20[0] < 0) {
            arg1->unk10 = -1;
            return 1;
        }
        var_s1 = 0;
        if ((temp_v0->unk20[1] >= 0) && (arg5 != 0)) {
            var_s1 = 1;
        }
        if (dll_4_func_1758(temp_v0, var_s1, &spF0, &spE0, &spD0, arg3 + 2, 0, 0) == 0) {
            return 1;
        }
        var_s2 = 0;
        temp = (arg2 / sqrtf(SQ(spD0.x - spD0.y) + (SQ(spF0.x - spF0.y) + SQ(spE0.x - spE0.y))));
        var_fs0 = temp + arg1->unk8;
        if (var_fs0 < 0.0f) {
            var_fs0 = 0.0f;
            var_s2 = -1;
        }
        if (var_fs0 > 1.0f) {
            var_fs0 = 1.0f;
            var_s2 = 1;
        }
        temp_fs2 = func_80004C5C(&spF0, var_fs0, &spB8);
        temp_fs5 = func_80004C5C(&spE0, var_fs0, &spB4);
        temp_fs3 = func_80004C5C(&spD0, var_fs0, &spB0);
        sp98 = arctan2_f(spB8, spB0) + 0x8000;
        if (arg4) {
            sp94 = arctan2_f(sqrtf((spB8 * spB8) + (spB0 * spB0)), spB4) - 0x4000;
            var_fv1 = sqrtf(SQ(temp_fs2 - arg0->transl.x) + SQ(temp_fs3 - arg0->transl.z));
        } else {
            var_fv1 = sqrtf(SQ(temp_fs2 - arg0->transl.x) + SQ(temp_fs3 - arg0->transl.z));
        }
        if (arg2 < 0.0f) {
            var_fv1 = -var_fv1;
        }
        
        if ((var_s2 == -1) && (var_fv1 < arg2)) {
            arg1->unk10 = temp_v0->unk18[var_s1];
            arg1->unk8 = 0.9999f;
            if ((var_s1 != 0) && (arg1->unk10 < 0)) {
                arg1->unk10 = temp_v0->unk18[0];
            }
        } else if ((var_s2 == 1) && (var_fv1 < arg2)) {
            arg1->unk10 = temp_v0->unk20[var_s1];
            arg1->unk8 = 0.0f;
            if ((var_s1 != 0) && (arg1->unk10 < 0)) {
                arg1->unk10 = temp_v0->unk20[0];
            }
        } else {
            arg1->unk8 = var_fs0;
        }
        arg2 -= var_fv1;
        arg0->transl.x = temp_fs2;
        if (arg4) {
            arg0->transl.y = temp_fs5;
        }
        arg0->transl.z = temp_fs3;
        var_s5++;
    }

    arg0->yaw = (s16) sp98;
    if (arg4 != 0) {
        arg0->pitch = (s16) sp94;
    }
    return 0;
}

// offset: 0x15B4 | func: 12 | export: 6
s32 dll_4_func_15B4(UNK_TYPE_32 a0, UNK_TYPE_32 a1, UNK_TYPE_32 a2, UNK_TYPE_32 a3) {
    return 1;
}

// offset: 0x15D0 | func: 13 | export: 7
void dll_4_func_15D0(s32 arg0, RaceStruct* arg1, s8* arg2) {
    s32 var_v1;
    s32 sp30;
    RaceCheckpointSetup* temp_v0;

    temp_v0 = dll_4_func_1F60(arg0, &sp30);
    if (temp_v0 != NULL) {
        arg1->unk0 = (f32) rand_next(-99, 99) / 100.0f;
        arg1->unk4 = (f32) rand_next(-99, 99) / 100.0f;
        arg1->unk8 = (f32) rand_next(0, 99) / 100.0f;
        var_v1 = 0;
        if (temp_v0->unk20[0] != 0) {
            var_v1 = 0;
            if (dll_4_func_1F60(temp_v0->unk20[0], &sp30)->unk20[0] >= 0) {
                var_v1 = 1;
            }
        }
        if (*arg2 == 0) {
            if (var_v1 != 0) {
                arg1->unk10 = temp_v0->unk20[0];
                return;
            }
            if (temp_v0->unk18[0] >= 0) {
                arg1->unk10 = temp_v0->unk18[0];
                *arg2 = 1;
            }
        } else {
            if (temp_v0->unk18[0] != 0) {
                arg1->unk10 = temp_v0->unk18[0];
                return;
            }
            if (var_v1 != 0) {
                arg1->unk10 = temp_v0->unk20[0];
                *arg2 = 0;
            }
        }
    }
}

// offset: 0x1758 | func: 14
static s32 dll_4_func_1758(RaceCheckpointSetup* arg0, s32 arg1, Vec4f* arg2, Vec4f* arg3, Vec4f* arg4, u8 arg5, f32 arg6, f32 arg7) {
    s32 spD4;
    s32 spD0;
    f32 temp_fs0;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fv1;
    s32 temp_v0;
    s32 var_s7;
    RaceCheckpointSetup *cur;
    RaceCheckpointSetup* spAC[2];
    f32 spA8;
    RaceCheckpointSetup *cur2;
    f32 spA0;
    s32 pad;
    s32 pad2;

    spD0 = 1;
    if (arg0 == NULL) {
        return 0;
    }
    spAC[0] = arg0;
    spAC[1] = dll_4_func_1F60(arg0->unk20[arg1], &spD4);
    if (spAC[1] == NULL) {
        spAC[1] = dll_4_func_1F60(arg0->unk20[1 - arg1], &spD4);
        spD0 = 2;
    }
    if (spAC[1] == NULL) {
        return 0;
    }
    cur = spAC[0];
    cur2 = spAC[1];
    spA8 = -fsin16_precise(cur->unk29 << 8);
    temp_fs0 = -fcos16_precise(cur->unk29 << 8);
    spA0 = -fsin16_precise(cur2->unk29 << 8);
    temp_fv1 = -fcos16_precise(cur2->unk29 << 8);
    
    temp_fs1 = cur->unk2A * 0.011111111f;
    temp_fs2 = cur2->unk2A * 0.011111111f;
    if (arg5 == 1) {
        for (var_s7 = 0; var_s7 < 4; var_s7++) {
            temp_v0 = (var_s7 * 4) >> 2;
            
            arg2[var_s7].x = (cur->unk2D[temp_v0] * (temp_fs1 * temp_fs0)) + cur->pos.x;
            arg2[var_s7].y = (cur2->unk2D[temp_v0] * (temp_fs2 * temp_fv1)) + cur2->pos.x;
            arg2[var_s7].z = 2.0f * (fsin16_precise(cur->unk3E << 8) * cur->unk3D);
            arg2[var_s7].w = 2.0f * (fsin16_precise(cur2->unk3E << 8) * cur2->unk3D);
            
            arg3[var_s7].x = (cur->unk31[temp_v0] * temp_fs1) + cur->pos.y;
            arg3[var_s7].y = (cur2->unk31[temp_v0] * temp_fs2) + cur2->pos.y;
            arg3[var_s7].z = 0.0f;
            arg3[var_s7].w = 0.0f;
            
            arg4[var_s7].x = (cur->unk2D[temp_v0] * (temp_fs1 * -spA8)) + cur->pos.z;
            arg4[var_s7].y = (cur2->unk2D[temp_v0] * (temp_fs2 * -spA0)) + cur2->pos.z;
            arg4[var_s7].z = 2.0f * (fcos16_precise(cur->unk3E << 8) * cur->unk3D);
            arg4[var_s7].w = 2.0f * (fcos16_precise(cur2->unk3E << 8) * cur2->unk3D);
        }
    } else if (!arg5) {
        arg2->x = cur->pos.x + (temp_fs1 * temp_fs0 * arg6);
        arg2->y = cur2->pos.x + (temp_fs2 * temp_fv1 * arg6);
        arg2->z = 2.0f * (fsin16_precise(cur->unk3E << 8) * cur->unk3D);
        arg2->w = 2.0f * (fsin16_precise(cur2->unk3E << 8) * cur2->unk3D);
       
        arg3->x = cur->pos.y + (temp_fs1 * arg7);
        arg3->y = cur2->pos.y + (temp_fs2 * arg7);
        arg3->z = 0.0f;
        arg3->w = 0.0f;
        
        arg4->x = cur->pos.z + (temp_fs1 * -spA8 * arg6);
        arg4->y = cur2->pos.z + (temp_fs2 * -spA0 * arg6);
        arg4->z = 2.0f * (fcos16_precise(cur->unk3E << 8) * cur->unk3D);
        arg4->w = 2.0f * (fcos16_precise(cur2->unk3E << 8) * cur2->unk3D);
    } else {
        arg2->x = (cur->unk2D[arg5 - 2] * (temp_fs1 * temp_fs0)) + cur->pos.x;
        arg2->y = (cur2->unk2D[arg5 - 2] * (temp_fs2 * temp_fv1)) + cur2->pos.x;
        arg2->z = 2.0f * (fsin16_precise(cur->unk3E << 8) * cur->unk3D);
        arg2->w = 2.0f * (fsin16_precise(cur2->unk3E << 8) * cur2->unk3D);

        temp_v0 = arg5 - 2;
        arg5 -= 2;
        
        arg3->x = (cur->unk31[temp_v0] * temp_fs1) + cur->pos.y;
        arg3->y = (cur2->unk31[temp_v0] * temp_fs2) + cur2->pos.y;
        arg3->z = 0.0f;
        arg3->w = 0.0f;
        
        arg4->x = (cur->unk2D[temp_v0] * (temp_fs1 * -spA8)) + cur->pos.z;
        arg4->y = (cur2->unk2D[temp_v0] * (temp_fs2 * -spA0)) + cur2->pos.z;
        arg4->z = 2.0f * (fcos16_precise(cur->unk3E << 8) * cur->unk3D);
        arg4->w = 2.0f * (fcos16_precise(cur2->unk3E << 8) * cur2->unk3D);
    }
    return spD0;
}

// offset: 0x1F60 | func: 15 | export: 8
RaceCheckpointSetup *dll_4_func_1F60(s32 checkpointUID, s32 *arg1) {
    s32 temp_t7;
    s32 max;
    s32 min;

    *arg1 = -1;
    
    if (checkpointUID < 0) {
        return NULL;
    }
    
    max = _bss_6A0 - 1;
    min = 0;

    while (min <= max) {
        temp_t7 = (max + min) >> 1;

        if (_bss_60[temp_t7].uID < (u32)checkpointUID) {
            min = temp_t7 + 1;
        } else if (_bss_60[temp_t7].uID > (u32)checkpointUID) {
            max = temp_t7 - 1;
        } else {
            *arg1 = temp_t7;
            return _bss_60[temp_t7].setup;
        }
    }

    return NULL;
}

/*0x0*/ static const char str_0[] = "Warning: calcPosition: Stack overflow!!\n";
/*0x2C*/ static const char str_2C[] = "racePostion: Error! (romdefno: %d) %d %i  \n";
