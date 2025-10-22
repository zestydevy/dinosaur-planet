#include "common.h"
#include "game/objects/object.h"
#include "dlls/engine/26_curves.h"

/*0x0*/ static CurveSetup *_bss_0;
/*0x4*/ static CurveSetup *_bss_4;
/*0x8*/ static CurveNode _bss_8[1300];
/*0x28A8*/ static s32 _bss_28A8;

s32 dll_26_func_1880(CurveSetup* arg0);
s32 dll_26_func_1924(CurveSetup *setup);
static s32 dll_26_func_2AE8(f32 a0, f32 a1, f32 a2, CurveSetup* a3, CurveSetup* a4, f32 a5);
s32 dll_26_func_4704(UnkCurvesStruct*);
s32 dll_26_func_5698(CurveSetup*, s32, s32);
s32 dll_26_func_577C(CurveSetup*, s32, s32);

// offset: 0x0 | ctor
void dll_26_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_26_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_26_func_18(void) {
    _bss_28A8 = 0;
}

// offset: 0x34 | func: 1 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_34.s")
#else
void dll_26_func_34(CurveSetup* arg0) {
    s32 var_v1;
    s32 temp;

    if ((_bss_28A8 && _bss_28A8) && _bss_28A8) {} // @fake
    
    if (_bss_28A8 == 1300) {
        return;
    }
    
    var_v1 = 0;
    while (var_v1 < _bss_28A8 && _bss_8[var_v1].uID < arg0->base.uID) {
        var_v1 += 1;
    }
    
    temp = _bss_28A8;
    while (temp > var_v1) {
        _bss_8[temp].setup = _bss_8[temp - 1].setup;
        _bss_8[temp].uID = _bss_8[temp - 1].uID;
        temp--;
    }

    _bss_28A8 += 1;
    _bss_8[var_v1].setup = arg0;
    _bss_8[var_v1].uID = arg0->base.uID;
}
#endif

// offset: 0x10C | func: 2 | export: 2
void dll_26_func_10C(CurveSetup* setup) {
    s32 i;
    
    i = 0;
    while (i < _bss_28A8 && setup->base.uID != _bss_8[i].uID) {
        i++;
    }

    if (i < _bss_28A8) {
        _bss_28A8--;

        while (i < _bss_28A8) {
            i++;
            _bss_8[i - 1].setup = _bss_8[i].setup;
            _bss_8[i - 1].uID = _bss_8[i].uID;
        }
    }
}

// offset: 0x1BC | func: 3 | export: 3
CurveNode *dll_26_func_1BC(s32 *count) {
    *count = _bss_28A8;
    return _bss_8;
}

// offset: 0x1E4 | func: 4 | export: 4
s32 dll_26_func_1E4(f32 x, f32 y, f32 z, s32 *arg3, s32 arg4, s32 arg5) {
    CurveSetup* curveSetup;
    CurveSetup* var_s4;
    CurveSetup* var_s5;
    f32 zDiff;
    f32 xDiff;
    f32 dist;
    f32 yDiff;
    f32 var_fs0;
    f32 var_fs1;
    s32 i;
    s32 k;

    var_fs0 = 5000000.0f;
    var_s4 = NULL;
    var_fs1 = 5000000.0f;
    var_s5 = NULL;
    
    for (i = 0; i < _bss_28A8; i++)  {
        curveSetup =_bss_8[i].setup;
        k = 0;

        do {
            if ((arg4 <= 0) || (curveSetup->unk19 == arg3[k])) {
                xDiff = curveSetup->base.x - x;
                yDiff = curveSetup->base.y - y;
                zDiff = curveSetup->base.z - z;
                dist = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
                if (dist < var_fs0) {
                    var_fs0 = dist;
                    var_s4 = curveSetup;
                }
                if ((arg5 == curveSetup->unk18) && (dist < var_fs1)) {
                    var_fs1 = dist;
                    var_s5 = curveSetup;
                }
                k = arg4;
            }

            k++;
        } while (k < arg4);
    }
    if (var_s5 != NULL) {
        var_s4 = var_s5;
    }
    if (var_s4 != NULL) {
        return var_s4->base.uID;
    }
    return -1;
}

// offset: 0x39C | func: 5 | export: 6
CurveSetup* dll_26_func_39C(s32 curveUID) {
    s32 temp_t6;
    s32 max;
    s32 min;

    if (curveUID < 0) {
        return NULL;
    }

    max = _bss_28A8 - 1;
    min = 0;

    while (min <= max) {
        temp_t6 = (max + min) >> 1;

        if (_bss_8[temp_t6].uID < (u32)curveUID) {
            min = temp_t6 + 1;
        } else if (_bss_8[temp_t6].uID > (u32)curveUID) {
            max = temp_t6 - 1;
        } else {
            return _bss_8[temp_t6].setup;
        }
    }

    return NULL;
}

// offset: 0x438 | func: 6 | export: 20
s32 dll_26_func_438(CurveSetup* arg0, s32 arg1) {
    s32 sp38[4];
    s32 var_t0;
    s32 var_a2;
    s32 var_v1;

    var_v1 = 0;
    var_a2 = 1;
    var_t0 = 0;
    while (var_t0 < 4) {
        if ((arg0->unk1C[var_t0] >= 0) && !(arg0->unk1B & var_a2) && (arg1 != arg0->unk1C[var_t0])) {
            sp38[var_v1] = arg0->unk1C[var_t0];
            var_v1 += 1;
        }
        var_t0++;
        var_a2 <<= 1;
    }

    if (var_v1 != 0) {
        var_t0 = rand_next(0, var_v1 - 1);
        return sp38[var_t0];
    }

    return -1;
}

// offset: 0x4F0 | func: 7 | export: 21
s32 dll_26_func_4F0(CurveSetup* arg0, s32 arg1, s32* arg2) {
    s32 var_v1;
    s32 i;
    s32 v0;

    for (i = 0, var_v1 = 0, v0 = 1; i < 4; i++, v0 <<= 1) {
        if ((arg0->unk1C[i] >= 0) && (arg1 != arg0->unk1C[i])) {
            arg2[var_v1] = arg0->unk1C[i];
            var_v1++;
        }
    }

    return var_v1;
}

// offset: 0x590 | func: 8 | export: 23
s32 dll_26_func_590(CurveSetup* arg0, s32 arg1) {
    s32 sp38[4];
    s32 var_t0;
    s32 var_a2;
    s32 var_v1;

    var_v1 = 0;
    var_a2 = 1;
    var_t0 = 0;
    while (var_t0 < 4) {
        if ((arg0->unk1C[var_t0] >= 0) && (arg0->unk1B & var_a2) && (arg1 != arg0->unk1C[var_t0])) {
            sp38[var_v1] = arg0->unk1C[var_t0];
            var_v1 += 1;
        }
        var_t0++;
        var_a2 <<= 1;
    }

    if (var_v1 != 0) {
        var_t0 = rand_next(0, var_v1 - 1);
        return sp38[var_t0];
    }

    return -1;
}

// offset: 0x648 | func: 9
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_648.s")
#else
// probably correct but way off on the match
f32 dll_26_func_648(f32 arg0, f32 arg1, f32 arg2, Vec3f* arg3) {
    f32 diff[3];
    f32 z0;
    f32 x0;
    f32 y0;
    f32 y1;
    f32 z1;
    f32 x1;
    f32 temp_fv0;
    f32 temp_fv1_2;
    f32 temp_fa0;
    f32 var_fa1;
    f32 var_ft2;
    f32 zResult;
    f32 xResult;
    f32 yResult;

    x1 = arg3[1].x;
    x0 = arg3[0].x;
    y1 = arg3[1].y;
    y0 = arg3[0].y;
    z1 = arg3[1].z;
    z0 = arg3[0].z;
    diff[0] = x1 - x0;
    diff[1] = y1 - y0;
    diff[2] = z1 - z0;
    if ((diff[0] == 0.0f) && (diff[1] == 0.0f) && (diff[2] == 0.0f)) {
        var_fa1 = 0.0f;
    } else {
        var_fa1 = (((arg2 - z0) * diff[2]) + (((arg0 - x0) * diff[0]) + ((arg1 - y0) * diff[1]))) 
            / ((diff[0] * diff[0]) + (diff[1] * diff[1]) + (diff[2] * diff[2]));
    }
    if (var_fa1 < 0.0f) {
        xResult = x0;
        yResult = y0;
        zResult = z0;
        temp_fv0 = xResult - arg0;
        temp_fv1_2 = yResult - arg1;
        temp_fa0 = zResult - arg2;
        var_ft2 = -((temp_fv0 * temp_fv0) + (temp_fv1_2 * temp_fv1_2) + (temp_fa0 * temp_fa0));
    } else if (var_fa1 > 1.0f) {
        xResult = x1;
        yResult = y1;
        zResult = z1;
        temp_fv0 = xResult - arg0;
        temp_fv1_2 = yResult - arg1;
        temp_fa0 = zResult - arg2;
        var_ft2 = -((temp_fv0 * temp_fv0) + (temp_fv1_2 * temp_fv1_2) + (temp_fa0 * temp_fa0));
    } else {
        xResult = x0 + (var_fa1 * diff[0]);
        yResult = y0 + (var_fa1 * diff[1]);
        zResult = z0 + (var_fa1 * diff[2]);
        temp_fv0 = xResult - arg0;
        temp_fv1_2 = yResult - arg1;
        temp_fa0 = zResult - arg2;
        var_ft2 = (temp_fv0 * temp_fv0) + (temp_fv1_2 * temp_fv1_2) + (temp_fa0 * temp_fa0);
    }
    arg3[2].x = xResult;
    arg3[2].y = yResult;
    arg3[2].z = zResult;
    return var_ft2;
}
#endif

// offset: 0x854 | func: 10 | export: 22
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_854.s")
#else
// needs dll_26_func_648 to be static
s32 dll_26_func_854(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    CurveSetup* temp_v0;
    s32 spAC[2];
    f32 temp_fa0;
    f32 temp_fv0;
    f32 sp9C[2];
    f32 temp_fv1;
    s32 s3;
    s32 temp_s2;
    Vec3f sp6C[3];
    s32 var_s1;

    spAC[0] = spAC[1] = -1;
    sp9C[0] = sp9C[1] = 0.0f;
    sp6C[0].x = arg0->base.x;
    sp6C[0].y = arg0->base.y;
    sp6C[0].z = arg0->base.z;
    for (var_s1 = 0, s3 = 1; var_s1 < 4; var_s1++, s3 <<= 1) {
        if (arg0->unk1C[var_s1] >= 0) {
            temp_v0 = dll_26_func_39C(arg0->unk1C[var_s1]);
            if (temp_v0 != NULL) {
                temp_s2 = arg1 == arg0->unk1C[var_s1];
                sp6C[1].x = temp_v0->base.x;
                sp6C[1].y = temp_v0->base.y;
                sp6C[1].z = temp_v0->base.z;
                dll_26_func_648(arg2, arg3, arg4, sp6C);
                temp_fv0 = sp6C[2].x - arg2;
                temp_fv1 = sp6C[2].y - arg3;
                temp_fa0 = sp6C[2].z - arg4;
                temp_fv0 = SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa0);
                if (sp9C[temp_s2] < temp_fv0) {
                    sp9C[temp_s2] = temp_fv0;
                    spAC[temp_s2] = arg0->unk1C[var_s1];
                }
            }
        }
    }
    if (spAC[0] != -1) {
        return spAC[0];
    }
    if (spAC[1] != -1) {
        return spAC[1];
    }
    return -1;
}
#endif

// offset: 0xA34 | func: 11 | export: 26
void dll_26_func_A34(CurveSetup* arg0, s32* arg1) {
    CurveSetup* temp_v0_2;
    s32 temp_v1;
    s32 var_a0;

    arg1[0] = -1;
    arg1[1] = -1;
    arg1[2] = -1;
    arg1[3] = -1;
    if (arg0 != NULL) {
        arg1[1] = arg0->base.uID;
        
        for (var_a0 = 0; var_a0 < 4; var_a0++) {
            if (arg0->unk1C[var_a0] != -1) {
                temp_v1 = arg0->unk1B & (1 << var_a0);
                if (temp_v1 != 0) {
                    arg1[0] = arg0->unk1C[var_a0];
                } else if (temp_v1 == 0) {
                    arg1[2] = arg0->unk1C[var_a0];
                }
            }
        }

        if (arg1[2] >= 0) {
            temp_v0_2 = dll_26_func_39C(arg1[2]);
            if (temp_v0_2 != NULL) {
                for (var_a0 = 0; var_a0 < 4; var_a0++) {
                    if ((temp_v0_2->unk1C[var_a0] != -1) && !(temp_v0_2->unk1B & (1 << var_a0))) {
                        arg1[3] = temp_v0_2->unk1C[var_a0];
                    }
                }
            }
        }
    }
}

// offset: 0xB8C | func: 12 | export: 27
s32 dll_26_func_B8C(s32* arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    s32 var_s3;
    s32 i;
    CurveSetup* sp50[4];

    var_s3 = 0;
    for (i = 0; i < 4; i++) {
        sp50[i] = dll_26_func_39C(arg0[i]);
        if (sp50[i] != NULL) {
            arg1[i] = sp50[i]->base.x;
            arg2[i] = sp50[i]->base.y;
            arg3[i] = sp50[i]->base.z;
            var_s3++;
        }
    }
    if (var_s3 < 2 || sp50[1] == NULL || sp50[2] == NULL) {
        return FALSE;
    }

    for (i = 0; i < 4; i++) {
        if (sp50[i] == NULL) {
            if (i == 0) {
                arg1[i] = sp50[1]->base.x + (sp50[1]->base.x - sp50[2]->base.x);
                arg2[i] = sp50[1]->base.y + (sp50[1]->base.y - sp50[2]->base.y);
                arg3[i] = sp50[1]->base.z + (sp50[1]->base.z - sp50[2]->base.z);
            } else if (i == 3) {
                arg1[i] = sp50[2]->base.x + (sp50[2]->base.x - sp50[1]->base.x);
                arg2[i] = sp50[2]->base.y + (sp50[2]->base.y - sp50[1]->base.y);
                arg3[i] = sp50[2]->base.z + (sp50[2]->base.z - sp50[1]->base.z);
            }
        }
    }

    return TRUE;
}

// offset: 0xD8C | func: 13 | export: 28
s32 dll_26_func_D8C(CurveSetup* arg0) {
    s32 var_s1;

    var_s1 = 1;
    while ((arg0 != NULL) && (dll_26_func_1924(arg0) == 0)) {
        arg0 = dll_26_func_39C(dll_26_func_438(arg0, 0));
        if (arg0 != NULL) {
            var_s1 += 1;
        }
    }
    return var_s1;
}

// offset: 0xE40 | func: 14 | export: 29
s32 dll_26_func_E40(CurveSetup* arg0, f32* arg1, f32* arg2, f32* arg3, s8* arg4) {
    CurveSetup* temp_v0;
    s32 var_s5;
    s32 var_v0;

    if (dll_26_func_1924(arg0) != 0) {
        var_v0 = 1;
    } else {
        var_v0 = 0;
    }
    var_s5 = 0;
    if (var_v0 == 0) {
        while ((arg0 != NULL) && (dll_26_func_1924(arg0) == 0)) {
            temp_v0 = dll_26_func_39C(dll_26_func_438(arg0, 0));
            if (temp_v0 != NULL) {
                if (arg4 != NULL) {
                    arg4[var_s5 >> 2] = arg0->unk19;
                }
                arg1[var_s5] = arg0->base.x;
                arg2[var_s5] = arg0->base.y;
                arg3[var_s5] = arg0->base.z;
                var_s5 = var_s5 + 1;
                arg1[var_s5] = temp_v0->base.x;
                arg2[var_s5] = temp_v0->base.y;
                arg3[var_s5] = temp_v0->base.z;
                var_s5 = var_s5 + 1;
                arg1[var_s5] = 2.0f * (fsin16_precise((s16) (arg0->unk2C << 8)) * (f32) arg0->unk2E);
                arg2[var_s5] = 2.0f * (fsin16_precise((s16) (arg0->unk2D << 8)) * (f32) arg0->unk2E);
                arg3[var_s5] = 2.0f * (fcos16_precise((s16) (arg0->unk2C << 8)) * (f32) arg0->unk2E);
                var_s5 = var_s5 + 1;
                arg1[var_s5] = 2.0f * (fsin16_precise((s16) (temp_v0->unk2C << 8)) * (f32) temp_v0->unk2E);
                arg2[var_s5] = 2.0f * (fsin16_precise((s16) (temp_v0->unk2D << 8)) * (f32) temp_v0->unk2E);
                arg3[var_s5] = 2.0f * (fcos16_precise((s16) (temp_v0->unk2C << 8)) * (f32) temp_v0->unk2E);
                var_s5 = var_s5 + 1;
            }
            arg0 = temp_v0;
        }
    } else {
        while ((arg0 != NULL) && (dll_26_func_1880(arg0) == 0)) {
            temp_v0 = dll_26_func_39C(dll_26_func_590(arg0, 0));
            if (temp_v0 != NULL) {
                if (arg4 != NULL) {
                    arg4[var_s5 >> 2] = arg0->unk19;
                }
                arg1[var_s5] = arg0->base.x;
                arg2[var_s5] = arg0->base.y;
                arg3[var_s5] = arg0->base.z;
                var_s5 = var_s5 + 1;
                arg1[var_s5] = temp_v0->base.x;
                arg2[var_s5] = temp_v0->base.y;
                arg3[var_s5] = temp_v0->base.z;
                var_s5 = var_s5 + 1;
                arg1[var_s5] = 2.0f * (fsin16_precise((s16) (arg0->unk2C << 8)) * (f32) arg0->unk2E);
                arg2[var_s5] = 2.0f * (fsin16_precise((s16) (arg0->unk2D << 8)) * (f32) arg0->unk2E);
                arg3[var_s5] = 2.0f * (fcos16_precise((s16) (arg0->unk2C << 8)) * (f32) arg0->unk2E);
                var_s5 = var_s5 + 1;
                arg1[var_s5] = 2.0f * (fsin16_precise((s16) (temp_v0->unk2C << 8)) * (f32) temp_v0->unk2E);
                arg2[var_s5] = 2.0f * (fsin16_precise((s16) (temp_v0->unk2D << 8)) * (f32) temp_v0->unk2E);
                arg3[var_s5] = 2.0f * (fcos16_precise((s16) (temp_v0->unk2C << 8)) * (f32) temp_v0->unk2E);
                var_s5 = var_s5 + 1;
            }
            arg0 = temp_v0;
        }
    }
    return var_s5;
}

// offset: 0x1420 | func: 15
CurveSetup* dll_26_func_1420(s32 curveUID, s32* arg1) {
    s32 temp_t6;
    s32 max;
    s32 min;

    *arg1 = -1;

    if (curveUID < 0) {
        return NULL;
    }

    max = _bss_28A8 - 1;
    min = 0;

    while (min <= max) {
        temp_t6 = (max + min) >> 1;

        if (_bss_8[temp_t6].uID < (u32)curveUID) {
            min = temp_t6 + 1;
        } else if (_bss_8[temp_t6].uID > (u32)curveUID) {
            max = temp_t6 - 1;
        } else {
            *arg1 = temp_t6;
            return _bss_8[temp_t6].setup;
        }
    }

    return NULL;
}

// offset: 0x14C8 | func: 16 | export: 10
void dll_26_func_14C8(CurveSetup** arg0, CurveSetup** arg1) {
    *arg0 = _bss_0;
    *arg1 = _bss_4;
}

// offset: 0x14F4 | func: 17 | export: 7
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_14F4.s")
#else
// needs dll_26_func_648 to be static
f32 dll_26_func_14F4(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32* arg5, f32* arg6, f32* arg7) {
    f32 var_fs0;
    s32 var_s1;
    s32 spAC;
    CurveSetup* temp_s2;
    CurveSetup* temp_v0;
    f32 temp_fv0;
    Vec3f sp7C[3];
    f32 var_fa0;
    f32 var_fv1;

    *arg7 = 0.0f;
    *arg6 = 0.0f;
    *arg5 = 0.0f;
    var_fs0 = 5000000.0f;
    for (spAC = 0; spAC < _bss_28A8; spAC++) {
        temp_s2 = _bss_8[spAC].setup;
        if ((arg1 == temp_s2->unk18) && (arg0 == temp_s2->unk19)) {
            sp7C[0].x = temp_s2->base.x;
            sp7C[0].y = temp_s2->base.y;
            sp7C[0].z = temp_s2->base.z;
            for (var_s1 = 0; var_s1 < 4; var_s1++) {
                if (!(temp_s2->unk1B & (1 << var_s1))) {
                    temp_v0 = dll_26_func_39C(temp_s2->unk1C[var_s1]);
                    if (temp_v0 != NULL) {
                        sp7C[1].x = temp_v0->base.x;
                        sp7C[1].y = temp_v0->base.y;
                        sp7C[1].z = temp_v0->base.z;
                        temp_fv0 = dll_26_func_648(arg2, arg3, arg4, sp7C);
                        if (temp_fv0 < 0) {
                            var_fa0 = -temp_fv0;
                        } else {
                            var_fa0 = temp_fv0;
                        }
                        if (var_fs0 < 0) {
                            var_fv1 = -var_fs0;
                        } else {
                            var_fv1 = var_fs0;
                        }
                        if (var_fa0 < var_fv1) {
                            _bss_0 = temp_s2;
                            _bss_4 = temp_v0;
                            var_fs0 = temp_fv0;
                            *arg5 = sp7C[2].x;
                            *arg6 = sp7C[2].y;
                            *arg7 = sp7C[2].z;
                        }
                    }
                }
            }
        }
    }
    return var_fs0;
}
#endif

// offset: 0x174C | func: 18 | export: 8
f32 dll_26_func_174C(Object* arg0, s32 arg1) {
    CurveSetup* temp_v0;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_v0 = dll_26_func_39C(arg1);
    if ((temp_v0 != NULL) && (arg0 != NULL)) {
        temp_fv0 = temp_v0->base.x - arg0->srt.transl.x;
        temp_fv1 = temp_v0->base.y - arg0->srt.transl.y;
        temp_fa1 = temp_v0->base.z - arg0->srt.transl.z;
        return sqrtf((temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1) + (temp_fa1 * temp_fa1));
    }
    return -1.0f;
}

// offset: 0x17F4 | func: 19 | export: 9
f32 dll_26_func_17F4(f32 arg0, f32 arg1, s32 arg2) {
    CurveSetup* temp_v0;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_v0 = dll_26_func_39C(arg2);
    if (temp_v0 != NULL) {
        temp_fv0 = temp_v0->base.x - arg0;
        temp_fv1 = temp_v0->base.z - arg1;
        return sqrtf((temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1));
    }
    return -1.0f;
}

// offset: 0x1880 | func: 20 | export: 11
s32 dll_26_func_1880(CurveSetup* arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if ((arg0->unk1C[i] != -1) && (arg0->unk1B & (1 << i))) {
            return 0;
        }
    }
    
    return 1;
}

// offset: 0x1924 | func: 21 | export: 12
s32 dll_26_func_1924(CurveSetup* arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if ((arg0->unk1C[i] != -1) && !(arg0->unk1B & (1 << i))) {
            return 0;
        }
    }
    
    return 1;
}

// offset: 0x19C8 | func: 22 | export: 13
s32 dll_26_func_19C8(CurveSetup* arg0, s32 *arg1, s32 arg2, s32* arg3) {
    CurveSetup* temp_v0;
    s32 temp_a0_2;
    s32 var_s0;
    s32 var_s2;
    s32 sp50[4];
    s32 var_s6;

    if (arg0 == NULL) {
        return -1;
    }
    
    var_s2 = 0;
    
    for (var_s6 = 0; var_s6 < 4; var_s6++) {
        if (arg0->unk1C[var_s6] >= 0) {
            temp_v0 = dll_26_func_39C(arg0->unk1C[var_s6]);
            for (var_s0 = 0; var_s0 < arg2; var_s0++) {
                if (temp_v0->unk19 == arg1[var_s0]) {
                    sp50[var_s2] = arg0->unk1C[var_s6];
                    var_s2 += 1;
                    var_s0 = arg2;
                }
            }
        }
    }
    
    if (var_s2 == 0) {
        return -1;
    }
    if (var_s2 == 1) {
        *arg3 = arg0->base.uID;
        return sp50[0];
    }
    if (var_s2 >= 2) {
        for (var_s6 = 0; var_s6 < var_s2; var_s6++) {
            temp_a0_2 = var_s2 - 1;
            if (sp50[var_s6] == *arg3) {
                var_s2 = temp_a0_2;
                while (var_s6 < temp_a0_2) {
                    sp50[var_s6] = sp50[var_s6 + 1];
                    var_s6++;
                }
            }
        }
        *arg3 = arg0->base.uID;
        var_s2 = rand_next(0, var_s2 - 1);
        return sp50[var_s2];
    }

    return -1;
}

// offset: 0x1B90 | func: 23 | export: 14
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_1B90.s")
#else
// https://decomp.me/scratch/AOhwW
s32 dll_26_func_1B90(CurveSetup* arg0, s32 arg1, s32 arg2, s32* arg3) {
    s32 temp;
    s32 sp720;
    s32 var_v0_2;
    f32 temp_fa1;
    f32 temp_fs0;
    s32 temp_a0;
    s32 sp70C;
    s32 var_s2;
    s32 sp6F8[4];
    s32 sp6F4;
    s32 sp6F0;
    f32 sp6D8[4];
    CurveSetup* temp_s4;
    CurveSetup* temp_v0;
    s32 temp_a1;
    s32 var_a0;
    s8 sp1BC[1300];
    s32 sp11C[40];
    f32 sp7C[40];
    s32 var_a2;
    s32 sp70;
    s32 var_a3;
    s32 var_s0;

    if (arg0 == NULL) {
        return -1;
    }
    dll_26_func_1420(arg0->base.uID, &sp6F0);
    sp720 = 0;
    for (sp70 = 0; sp70 < 4; sp70++) {
        if (arg0->unk1C[sp70] >= 0) {
            for (var_v0_2 = 0; var_v0_2 < 1300; var_v0_2++) { sp1BC[var_v0_2] = 0; }
            var_s0 = 1;
            sp1BC[sp6F0] = 1;
            temp_v0 = dll_26_func_1420(arg0->unk1C[sp70], &sp6F4);
            sp7C[0] = SQ(temp_v0->base.x - arg0->base.x) + SQ(temp_v0->base.y - arg0->base.y) + SQ(temp_v0->base.z - arg0->base.z);
            sp11C[0] = sp6F4;
            sp1BC[sp6F4] = 1;
            sp70C = 0;
            while (sp70C == 0) {
                if (var_s0 > 0) {
                    var_s0--;
                    sp6F4 = sp11C[var_s0];
                    temp_s4 = _bss_8[sp6F4].setup;
                    temp_fs0 = sp7C[var_s0];
                    if (arg1 == temp_s4->unk19 && (arg2 == -1 || arg2 == temp_s4->unk18)) {
                        sp70C = 1;
                        sp6D8[sp720] = temp_fs0;
                        sp6F8[sp720] = arg0->unk1C[sp70];
                        sp720 += 1;
                        continue;
                    }

                    for (var_s2 = 0; var_s2 < 4; var_s2 += 1) {
                        if (temp_s4->unk1C[var_s2] >= 0) {
                            temp_v0 = dll_26_func_1420(temp_s4->unk1C[var_s2], &sp6F4);
                            if (temp_v0 != NULL && sp1BC[sp6F4] == 0 && var_s0 < 40) {
                                temp_fa1 = SQ(temp_s4->base.x - temp_v0->base.x) + temp_fs0 + SQ(temp_s4->base.y - temp_v0->base.y) + SQ(temp_s4->base.z - temp_v0->base.z);
                                var_a2 = 0;
                                while (var_a2 < var_s0 && temp_fa1 < sp7C[var_a2]) {
                                    var_a2++;
                                }
                                var_a0 = var_s0;
                                while (var_a2 < var_a0) {
                                    sp11C[var_a0] = sp11C[var_a0 - 1];
                                    sp7C[var_a0] = sp7C[var_a0 - 1];
                                    var_a0--;
                                }
                                sp7C[var_a2] = temp_fa1;
                                sp11C[var_a2] = sp6F4;
                                sp1BC[sp6F4] = 1;
                                var_s0 += 1;
                            }
                        }
                    }
                } else {
                    sp70C = 1;
                }
            }
        }
    }

    if (sp720 == 0) {
        return -1;
    }
    if (sp720 == 1) {
        *arg3 = arg0->base.uID;
        return sp6F8[0];
    }
    if (sp720 >= 2) {
        for (var_a3 = 0, temp = 0; var_a3 < sp720; var_a3++) {
            temp_a1 = sp720 - 1;
            if (sp6F8[var_a3] == *arg3) {
                while (var_a3 < temp_a1) {
                    sp6F8[var_a3] = sp6F8[var_a3 + 1];
                    sp6D8[var_a3] = sp6D8[var_a3 + 1];
                    var_a3++;
                }
                sp720--;
            }
        }
        *arg3 = arg0->base.uID;
        for (var_a3 = 0; var_a3 < sp720; var_a3++) {
            if (sp6D8[var_a3] < sp6D8[temp]) {
                temp = var_a3;
            }            
        }
        return sp6F8[temp];
    }
    return -1;
}
#endif

// offset: 0x218C | func: 24 | export: 15
s32 dll_26_func_218C(s32 arg0) {
    CurveSetup* temp_a0;
    s32 i;

    for (i = 0; i < _bss_28A8; i++) {
        temp_a0 = _bss_8[i].setup;
        if ((temp_a0->unk19 == 0x15) && (arg0 == temp_a0->type15.unk34)) {
            return temp_a0->base.uID;
        }
    }
    return -1;
}

// offset: 0x21FC | func: 25 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_21FC.s")

// offset: 0x277C | func: 26 | export: 17
s32 dll_26_func_277C(f32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    f32 var_fs0;
    f32 var_fs1;
    s32 i;
    s32 sp90;
    s32 objCount;
    Object* obj;
    Object** objs;
    CurveSetup* curveSetup;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;
  
    objs = get_world_objects(&sp90, &objCount);
    var_fs1 = 0.0f;
    var_fs0 = -1.0f;
    for (i = 0; i < objCount; i++) {
        obj = objs[i];
        if ((obj->group == 0x2E) && (arg3 != obj->mapID)) {
            curveSetup = (CurveSetup*)obj->setup;
            if ((curveSetup != NULL) && (curveSetup->unk19 == 0x16)) {
                temp_fv0 = obj->positionMirror.x - arg0, 
                temp_fv1 = obj->positionMirror.y - arg1, 
                temp_fa1 = obj->positionMirror.z - arg2, 
                temp_fv0_2 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));

                if (var_fs0 == -1.0f || temp_fv0_2 < var_fs1) {
                    var_fs1 = temp_fv0_2;
                    var_fs0 = (f32) (u32) curveSetup->base.uID;
                }
            }
        }
    }
    return (s32) var_fs0;
}

// offset: 0x291C | func: 27 | export: 18
s32 dll_26_func_291C(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4) {
    CurveSetup* temp_v0;
    CurveSetup* var_s1;
    s32 sp64;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 var_s0;
    s32 var_s5;
    s32 var_v0;

    sp64 = arg0;
    var_s1 = dll_26_func_39C(sp64);
    var_s5 = 0;
    arg4[0] = 200000.0f;

    do {
        var_s0 = -1;
        for (var_v0 = 0; var_v0 < 4 && var_s0 == -1; var_v0++) {
            if (var_s1->unk1B & (1 << var_v0)) {
                continue;
            }

            var_s0 = var_s1->unk1C[var_v0];
        }

        if (var_s0 != -1) {
            temp_v0 = dll_26_func_39C(var_s0);
            if (dll_26_func_2AE8(arg1, arg2, arg3, var_s1, temp_v0, 1000.0f) != 0) {
                temp_fv0 = var_s1->base.x - arg1;
                temp_fv1 = var_s1->base.y - arg2;
                temp_fa1 = var_s1->base.z - arg3;
                temp_fv0 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));
                if (temp_fv0 < arg4[0]) {
                    arg4[0] = temp_fv0;
                }
                var_s5++;
            }

            sp64 = var_s0;
            var_s1 = temp_v0;
        }

        if (sp64 == arg0) {
            break;
        }
    } while (var_s0 != -1);
    
    return var_s5 & 1;
}

// offset: 0x2AE8 | func: 28
static s32 dll_26_func_2AE8(f32 a0, f32 a1, f32 a2, CurveSetup* a3, CurveSetup* a4, f32 a5) {
    f32 temp_fa1;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_ft1;
    f32 temp_fv1;

    temp_fv0 = a3->base.x;
    temp_fv1 = a3->base.z;
    temp_fa1 = a4->base.x;
    temp_ft4 = a4->base.z;
    temp_ft5 = (temp_fa1 * temp_fv1) - (temp_fv0 * temp_ft4);
    temp_ft1 = ((temp_ft4 - temp_fv1) * a0) + ((temp_fv0 - temp_fa1) * a2) + temp_ft5;
    if ((temp_ft1 <= 0.0f && temp_ft5 >= 0.0f) || (temp_ft1 >= 0.0f && temp_ft5 < 0.0f)) {
        if (((((-a2 * temp_fv0) + (a0 * temp_fv1)) <= 0.0f) && (((-a2 * temp_fa1) + (a0 * temp_ft4)) >= 0.0f)) || 
            ((((-a2 * temp_fv0) + (a0 * temp_fv1)) >= 0.0f) && (((-a2 * temp_fa1) + (a0 * temp_ft4)) < 0.0f))) {
            return 1;
        }
    }
    return 0;
}

// offset: 0x2C20 | func: 29 | export: 30
s32 dll_26_func_2C20(s32*, f32, f32, f32, f32*, f32*, f32*);
// https://decomp.me/scratch/229KB
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_2C20.s")

// offset: 0x3018 | func: 30 | export: 31
CurveSetup* dll_26_func_3018(CurveSetup* arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4) {
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    s32 sp74[4];

    while (!dll_26_func_1924(arg0)) {
        dll_26_func_A34(arg0, sp74);
        if ((dll_26_func_2C20(sp74, arg1, arg2, arg3, &sp8C, &sp88, &sp84) != 0) && (sp8C > -300.0f) && (sp8C < 300.0f) && (sp88 > -100.0f) && (sp88 < 100.0f)) {
            *arg4 = sp84;
            return arg0;
        }
        arg0 = dll_26_func_39C(dll_26_func_438(arg0, 0));
    }
    *arg4 = 0.0f;
    return arg0;
}

// offset: 0x31E0 | func: 31 | export: 32
void dll_26_func_31E0(CurveSetup *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4) {
    CurveSetup *temp_v0;
    f32 xDiff, yDiff, zDiff;

    temp_v0 = dll_26_func_39C(dll_26_func_438(arg0, 0));
    if (temp_v0 == NULL) {
        *arg1 = arg0->base.x;
        *arg2 = arg0->base.y;
        *arg3 = arg0->base.z;
        return;
    }

    xDiff = temp_v0->base.x - arg0->base.x;
    yDiff = temp_v0->base.y - arg0->base.y;
    zDiff = temp_v0->base.z - arg0->base.z;
    
    *arg1 = arg0->base.x + xDiff * arg4;
    *arg2 = arg0->base.y + yDiff * arg4;
    *arg3 = arg0->base.z + zDiff * arg4;
}

// offset: 0x32D8 | func: 32 | export: 33
f32 dll_26_func_32D8(CurveSetup *arg0, CurveSetup *arg1, f32 *arg2, f32 *arg3, f32 arg4, f32 arg5) {
    CurveSetup *temp_v0_4;
    CurveSetup *var_s1;
    f32 *temp_v0_2;
    f32 zDiff;
    f32 xDiff;
    f32 yDiff;
    f32 magnitude;
    s32 var_s2;
    s32 var_s5;

    if (arg0 == arg1) {
        xDiff = arg3[0] - arg2[0];
        yDiff = arg3[1] - arg2[1];
        zDiff = arg3[2] - arg2[2];
        magnitude = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
        if (arg5 < arg4) {
            magnitude = -magnitude;
        }
    } else {
        var_s5 = 0;
        var_s2 = 0;
        var_s1 = arg0;
        do {
            if (dll_26_func_1924(var_s1) != 0) {
                var_s2 = 1;
                var_s5 = 0;
            } else {
                var_s1 = dll_26_func_39C(dll_26_func_438(var_s1, 0));
                if (var_s1 == arg1) {
                    var_s2 = 1;
                    var_s5 = 1;
                    if ((!arg0) && (!arg0)){} // @fake
                }
            }
        } while (var_s2 == 0);
        var_s2 = 0;
        if (var_s5 == 0) {
            temp_v0_2 = arg2;
            var_s1 = arg0;
            arg0 = arg1;
            arg2 = arg3;
            arg1 = var_s1;
            arg3 = temp_v0_2;
        }
        arg0 = dll_26_func_39C(dll_26_func_438(arg0, 0));
        xDiff = arg0->base.x - arg2[0];
        yDiff = arg0->base.y - arg2[1];
        zDiff = arg0->base.z - arg2[2];
        magnitude = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
        do {
            if (arg0 == arg1) {
                xDiff = arg3[0] - arg0->base.x;
                yDiff = arg3[1] - arg0->base.y;
                zDiff = arg3[2] - arg0->base.z;
                var_s2 = 1;
                magnitude += sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
            } else {
                temp_v0_4 = dll_26_func_39C(dll_26_func_438(arg0, 0));
                xDiff = temp_v0_4->base.x - arg0->base.x;
                yDiff = temp_v0_4->base.y - arg0->base.y;
                zDiff = temp_v0_4->base.z - arg0->base.z;
                magnitude += sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
                arg0 = temp_v0_4;
            }
        } while (var_s2 == 0);
        if (var_s5 == 0) {
            magnitude = -magnitude;
        }
    }
    return magnitude;
}

// offset: 0x35AC | func: 33 | export: 19
s32 dll_26_func_35AC(f32 arg0, f32 arg1, f32 arg2) {
    CurveSetup* temp_v1;
    s32 sp6C[20];
    s32 var_s1;
    s8 temp_s2;
    s32 var_v0;
    f32 sp5C;

    var_s1 = 0;
    for (var_v0 = 0; var_v0 < _bss_28A8 && var_s1 < 0x14; var_v0++) {
        temp_v1 = _bss_8[var_v0].setup;
        if (temp_v1->unk19 == 0x17) {
            sp6C[var_s1] = temp_v1->base.uID;
            var_s1 += 1;
        }
    }
   
    while (var_s1 != 0) {
        if (dll_26_func_291C(sp6C[0], arg0, arg1, arg2, &sp5C) != 0) {
            return sp6C[0];
        }
        temp_s2 = dll_26_func_39C(sp6C[0])->unk18;
        var_v0 = 0;
        while (var_v0 < var_s1) {
            if (temp_s2 == dll_26_func_39C(sp6C[var_v0])->unk18) {
                var_s1 -= 1;
                sp6C[var_v0] = sp6C[var_s1];
            } else {
                var_v0++;
            }
        }
    }
    return -1;
}

// offset: 0x374C | func: 34 | export: 24
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_374C.s")
#else
// needs dll_26_func_648 to be static
s32 dll_26_func_374C(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    CurveSetup* temp_v0;
    s32 spAC[2];
    f32 temp_fa0;
    f32 temp_fv0;
    f32 sp9C[2];
    f32 temp_fv1;
    s32 s3;
    s32 temp_s2;
    Vec3f sp6C[3];
    s32 var_s1;

    spAC[0] = spAC[1] = -1;
    sp9C[0] = sp9C[1] = 5000000.0f;
    sp6C[0].x = arg0->base.x;
    sp6C[0].y = arg0->base.y;
    sp6C[0].z = arg0->base.z;
    for (var_s1 = 0, s3 = 1; var_s1 < 4; var_s1++, s3 <<= 1) {
        if (arg0->unk1C[var_s1] >= 0) {
            temp_v0 = dll_26_func_39C(arg0->unk1C[var_s1]);
            if (temp_v0 != NULL) {
                temp_s2 = arg1 == arg0->unk1C[var_s1];
                sp6C[1].x = temp_v0->base.x;
                sp6C[1].y = temp_v0->base.y;
                sp6C[1].z = temp_v0->base.z;
                dll_26_func_648(arg2, arg3, arg4, sp6C);
                temp_fv0 = sp6C[2].x - arg2;
                temp_fv1 = sp6C[2].y - arg3;
                temp_fa0 = sp6C[2].z - arg4;
                temp_fv0 = SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa0);
                if (temp_fv0 < sp9C[temp_s2]) {
                    sp9C[temp_s2] = temp_fv0;
                    spAC[temp_s2] = arg0->unk1C[var_s1];
                }
            }
        }
    }
    if (spAC[0] != -1) {
        return spAC[0];
    }
    if (spAC[1] != -1) {
        return spAC[1];
    }
    return -1;
}
#endif

// offset: 0x3930 | func: 35 | export: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_3930.s")

// offset: 0x3F00 | func: 36 | export: 5
s32 dll_26_func_3F00(Object* arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 temp_t5;
    s32 var_v0_2;
    f32 temp_fs0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fs2;
    s32 sp104;
    f32 var_fs3;
    s32 spFC;
    Vec3f spF0;
    s32 spEC;
    CurveSetup* temp_s0;
    u8 _pad2;
    u8 spE6;
    s32 _pad;
    CurveSetup* spDC;
    CurveSetup* spD8;
    Func_80059C40_Struct sp84;

    spDC = NULL;
    spD8 = NULL;
    var_fs2 = 5000000.0f;
    var_fs3 = 5000000.0f;
    spF0.x = arg0->srt.transl.f[0];
    spF0.y = arg0->srt.transl.f[1] + 10.0f;
    spF0.z = arg0->srt.transl.f[2];
    func_80007EE0(&spF0, &spFC);
    for (spEC = 0; spEC < _bss_28A8; spEC++) {
        temp_s0 = _bss_8[spEC].setup;
        var_v0_2 = 0;
        do {
            if ((temp_s0->unk19 == arg1[var_v0_2]) || (arg2 <= 0)) {
                temp_fs0 = temp_s0->base.x - arg0->srt.transl.f[0];
                temp_fv0 = temp_s0->base.y - arg0->srt.transl.f[1];
                temp_fv1 = temp_s0->base.z - arg0->srt.transl.f[2];
                temp_fs0 = sqrtf((temp_fs0 * temp_fs0) + (temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1));
                if ((temp_fs0 < var_fs2)) {
                    spF0.x = temp_s0->base.x;
                    spF0.y = temp_s0->base.y + 10.0f;
                    spF0.z = temp_s0->base.z;
                    func_80007EE0(&spF0, &sp104);
                    temp_t5 = func_80008048(&sp104, &spFC, NULL, &spE6, 0);
    
                    if (((spE6 == 1) || (temp_t5 != 0)) && (func_80059C40(&arg0->srt.transl, &spF0, 1.0f, 0, &sp84, arg0, (s8) arg4, -1, 0U, 0) == 0)) {
                        var_fs2 = temp_fs0;
                        spDC = temp_s0;
                    }
                }
                if ((arg3 == temp_s0->unk18) && (temp_fs0 < var_fs3)) {
                    spF0.x = temp_s0->base.x;
                    spF0.y = temp_s0->base.y + 10.0f;
                    spF0.z = temp_s0->base.z;
                    func_80007EE0(&spF0, &sp104);
                    temp_t5 = func_80008048(&sp104, &spFC, NULL, &spE6, 0);
    
                    if (((spE6 == 1) || (temp_t5 != 0)) && (func_80059C40(&arg0->srt.transl, &spF0, 1.0f, 0, &sp84, arg0, (s8) arg4, -1, 0U, 0) == 0)) {
                        var_fs3 = temp_fs0;
                        spD8 = temp_s0;
                    }
                }
                var_v0_2 = arg2;
            }
            var_v0_2++;
        } while (var_v0_2 < arg2);
    }

    if (spD8 != NULL) {
        spDC = spD8;
    }
    if (spDC != NULL) {
        return spDC->base.uID;
    } else {
        return -1;
    }
}

// offset: 0x4288 | func: 37 | export: 34
s32 dll_26_func_4288(UnkCurvesStruct* arg0, Object* arg1, f32 arg2, s32 *arg3, s32 arg4) {
    f32 zDiff;
    f32 xDiff;
    f32 yDiff;
    s32 var_a0;
    s32 var_v0;

    if (arg0 == NULL) {
        return 1;
    }
    
    var_a0 = dll_26_func_3F00(arg1, arg3, 1, arg4, 0xC);
    if (var_a0 != -1) {
        if (arg0->unk0x80 != 0) {
            arg0->unk0xA0 = dll_26_func_39C(var_a0);
            var_a0 = dll_26_func_438(arg0->unk0xA0, -1);
            if (var_a0 == -1) {
                return 1;
            }
        }
        
        arg0->unk0xA0 = dll_26_func_39C(var_a0);
        if (arg0->unk0xA0 == NULL) {
            arg0->unk0xA0 = NULL;
            return 1;
        }
        if (arg0->unk0x80 != 0) {
            var_v0 = dll_26_func_590(arg0->unk0xA0, -1);
        } else {
            var_v0 = dll_26_func_438(arg0->unk0xA0, -1);
        }
        if (var_v0 == -1) {
            return 1;
        }
        
        arg0->unk0xA4 = dll_26_func_39C(var_v0);
        if (arg0->unk0xA4 == NULL) {
            arg0->unk0xA4 = NULL;
            return 1;
        }
        if (arg0->unk0x80 != 0) {
            xDiff = arg0->unk0xA4->base.x - arg1->srt.transl.x;
            yDiff = arg0->unk0xA4->base.y - arg1->srt.transl.y;
            zDiff = arg0->unk0xA4->base.z - arg1->srt.transl.z;
        } else {
            xDiff = arg0->unk0xA0->base.x - arg1->srt.transl.x;
            yDiff = arg0->unk0xA0->base.y - arg1->srt.transl.y;
            zDiff = arg0->unk0xA0->base.z - arg1->srt.transl.z;
        }
        
        if (arg2 < sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff))) {
            return 1;
        }
        
        arg0->unk0xB8[0] = arg0->unk0xA0->base.x;
        arg0->unk0xB8[1] = arg0->unk0xA4->base.x;
        arg0->unk0xB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
        arg0->unk0xB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
        
        arg0->unk0xD8[0] = arg0->unk0xA0->base.y;
        arg0->unk0xD8[1] = arg0->unk0xA4->base.y;
        arg0->unk0xD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2D << 8)) * (f32) arg0->unk0xA0->unk2E);
        arg0->unk0xD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2D << 8)) * (f32) arg0->unk0xA4->unk2E);
        
        arg0->unk0xF8[0] = arg0->unk0xA0->base.z;
        arg0->unk0xF8[1] = arg0->unk0xA4->base.z;
        arg0->unk0xF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
        arg0->unk0xF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
        
        if (dll_26_func_4704(arg0) != 0) {
            return 1;
        }
        
        arg0->unk0x94 = func_80004C5C;
        arg0->unk0x98 = (unk_curve_func_2)func_80004CE8;
        arg0->unk0x84 = arg0->unk0xA8;
        arg0->unk0x88 = arg0->unk0xC8;
        arg0->unk0x8C = arg0->unk0xE8;
        arg0->unk0x90 = 8;
        
        func_80005094(arg0);
        
        return 0;
    }

    return 1;
}

// offset: 0x4704 | func: 38 | export: 35
s32 dll_26_func_4704(UnkCurvesStruct* arg0) {
    s32 var_v0;

    if (arg0 == NULL) {
        return 1;
    }
    if ((arg0->unk0xA0 == NULL) || (arg0->unk0xA4 == NULL)) {
        return 1;
    }
    arg0->unk0x9C = arg0->unk0xA0;
    arg0->unk0xA0 = arg0->unk0xA4;
    memcpy(arg0->unk0xA8, arg0->unk0xB8, 0x10U);
    memcpy(arg0->unk0xC8, arg0->unk0xD8, 0x10U);
    memcpy(arg0->unk0xE8, arg0->unk0xF8, 0x10U);
    if (arg0->unk0x80 != 0) {
        var_v0 = dll_26_func_590(arg0->unk0xA0, -1);
    } else {
        var_v0 = dll_26_func_438(arg0->unk0xA0, -1);
    }
    if (var_v0 != -1) {
        arg0->unk0xA4 = dll_26_func_39C(var_v0);
        if (arg0->unk0xA4 != NULL) {
            if (arg0->unk0x80 != 0) {
                arg0->unk0xB8[0] = arg0->unk0xA0->base.x;
                arg0->unk0xB8[1] = arg0->unk0x9C->base.x;
                arg0->unk0xB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0x9C->unk2C << 8)) * (f32) arg0->unk0x9C->unk2E);
                
                arg0->unk0xD8[0] = arg0->unk0xA0->base.y;
                arg0->unk0xD8[1] = arg0->unk0x9C->base.y;
                arg0->unk0xD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2D << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0x9C->unk2D << 8)) * (f32) arg0->unk0x9C->unk2E);
                
                arg0->unk0xF8[0] = arg0->unk0xA0->base.z;
                arg0->unk0xF8[1] = arg0->unk0x9C->base.z;
                arg0->unk0xF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk0x9C->unk2C << 8)) * (f32) arg0->unk0x9C->unk2E);
            } else {
                arg0->unk0xB8[0] = arg0->unk0xA0->base.x;
                arg0->unk0xB8[1] = arg0->unk0xA4->base.x;
                arg0->unk0xB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
                
                arg0->unk0xD8[0] = arg0->unk0xA0->base.y;
                arg0->unk0xD8[1] = arg0->unk0xA4->base.y;
                arg0->unk0xD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2D << 8)) * (f32)  arg0->unk0xA0->unk2E);
                arg0->unk0xD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2D << 8)) * (f32) arg0->unk0xA4->unk2E);
                
                arg0->unk0xF8[0] = arg0->unk0xA0->base.z;
                arg0->unk0xF8[1] = arg0->unk0xA4->base.z;
                arg0->unk0xF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
            }
            if (arg0->unk0x90 != 0) {
                func_8000523C(arg0);
            }
            if (arg0->unk0x80 != 0) {
                func_800053B0(arg0, -1.0f);
                return 0;
            }
            func_800053B0(arg0, 1.0f);
            return 0;
        }
    } else {
        arg0->unk0xA4 = NULL;
    }
    
    return 1;
}

// offset: 0x4CB4 | func: 39 | export: 36
s32 dll_26_func_4CB4(UnkCurvesStruct* arg0, s32 arg1) {
    f32 sp24;
    CurveSetup* temp_t7;

    if (arg1 == arg0->unk0x80) {
        return 0;
    }

    if ((arg0->unk0xA0 == NULL) || (arg0->unk0x9C == NULL)) {
        return 1;
    }
    
    sp24 = arg0->unk0x0;

    arg0->unk0x80 = arg1;
    temp_t7 = arg0->unk0x9C;
    arg0->unk0x9C = arg0->unk0xA4;
    arg0->unk0xA4 = temp_t7;
    
    arg0->unk0xB8[0] = arg0->unk0xA0->base.x;
    arg0->unk0xB8[1] = arg0->unk0xA4->base.x;
    arg0->unk0xB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
    arg0->unk0xB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
    
    arg0->unk0xD8[0] = arg0->unk0xA0->base.y;
    arg0->unk0xD8[1] = arg0->unk0xA4->base.y;
    arg0->unk0xD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2D << 8)) * (f32) arg0->unk0xA0->unk2E);
    arg0->unk0xD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2D << 8)) * (f32) arg0->unk0xA4->unk2E);
    
    arg0->unk0xF8[0] = arg0->unk0xA0->base.z;
    arg0->unk0xF8[1] = arg0->unk0xA4->base.z;
    arg0->unk0xF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
    arg0->unk0xF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
    
    if (dll_26_func_4704(arg0) != 0) {
        return 1;
    }
    
    arg0->unk0x94 = func_80004C5C;
    arg0->unk0x98 = (unk_curve_func_2)func_80004CE8;
    arg0->unk0x84 = arg0->unk0xA8;
    arg0->unk0x88 = arg0->unk0xC8;
    arg0->unk0x8C = arg0->unk0xE8;
    arg0->unk0x90 = 8;
    
    func_80005094(arg0);
    arg0->unk0x0 = sp24;
    return 0;
}

// offset: 0x4FA8 | func: 40 | export: 37
void dll_26_func_4FA8(UnkCurvesStruct* arg0, CurveSetup* arg1) {
    if ((arg1 != NULL) && (arg1 != arg0->unk0xA4)) {
        arg0->unk0xA4 = arg1;
        
        arg0->unk0xB8[1] = arg1->base.x;
        arg0->unk0xB8[3] = 2.0f * (fsin16_precise((s16) (arg1->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
        
        arg0->unk0xD8[1] = arg0->unk0xA4->base.y;
        arg0->unk0xD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2D << 8)) * (f32) arg0->unk0xA4->unk2E);
        
        arg0->unk0xF8[1] = arg0->unk0xA4->base.z;
        arg0->unk0xF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
    }
}

// offset: 0x50E4 | func: 41 | export: 38
s32 dll_26_func_50E4(UnkCurvesStruct* arg0, s32 arg1) {
    s32 var_v0;

    if (arg0 == NULL) {
        return 1;
    }

    if ((arg0->unk0xA0 == NULL) || ((arg0->unk0xA4 == NULL))) {
        return 1;
    }
    arg0->unk0x9C = arg0->unk0xA0;
    arg0->unk0xA0 = arg0->unk0xA4;
    memcpy(arg0->unk0xA8, arg0->unk0xB8, sizeof(f32)*4);
    memcpy(arg0->unk0xC8, arg0->unk0xD8, sizeof(f32)*4);
    memcpy(arg0->unk0xE8, arg0->unk0xF8, sizeof(f32)*4);
    if (arg0->unk0x80 != 0) {
        var_v0 = dll_26_func_577C(arg0->unk0xA0, -1, arg1);
    } else {
        var_v0 = dll_26_func_5698(arg0->unk0xA0, -1, arg1);
    }
    if (var_v0 != -1) {
        arg0->unk0xA4 = dll_26_func_39C(var_v0);
        if (arg0->unk0xA4 != NULL) {
            if (arg0->unk0x80 != 0) {
                arg0->unk0xB8[0] = arg0->unk0xA0->base.x;
                arg0->unk0xB8[1] = arg0->unk0x9C->base.x;
                arg0->unk0xB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0x9C->unk2C << 8)) * (f32) arg0->unk0x9C->unk2E);
                
                arg0->unk0xD8[0] = arg0->unk0xA0->base.y;
                arg0->unk0xD8[1] = arg0->unk0x9C->base.y;
                arg0->unk0xD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2D << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0x9C->unk2D << 8)) * (f32) arg0->unk0x9C->unk2E);
                
                arg0->unk0xF8[0] = arg0->unk0xA0->base.z;
                arg0->unk0xF8[1] = arg0->unk0x9C->base.z;
                arg0->unk0xF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk0x9C->unk2C << 8)) * (f32) arg0->unk0x9C->unk2E);
            } else {
                arg0->unk0xB8[0] = arg0->unk0xA0->base.x;
                arg0->unk0xB8[1] = arg0->unk0xA4->base.x;
                arg0->unk0xB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
                
                arg0->unk0xD8[0] = arg0->unk0xA0->base.y;
                arg0->unk0xD8[1] = arg0->unk0xA4->base.y;
                arg0->unk0xD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2D << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2D << 8)) * (f32) arg0->unk0xA4->unk2E);
                
                arg0->unk0xF8[0] = arg0->unk0xA0->base.z;
                arg0->unk0xF8[1] = arg0->unk0xA4->base.z;
                arg0->unk0xF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
                arg0->unk0xF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
            }
            if (arg0->unk0x90 != 0) {
                func_8000523C(arg0);
            }
            if (arg0->unk0x80 != 0) {
                func_800053B0(arg0, -1.0f);
                return 0;
            }
            func_800053B0(arg0, 1.0f);
            return 0;
        }
    } else {
        arg0->unk0xA4 = NULL;
    }
    
    return 1;
}

// offset: 0x5698 | func: 42 | export: 39
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_5698.s")
#else
// regalloc
s32 dll_26_func_5698(CurveSetup* arg0, s32 arg1, s32 arg2) {
    s32 sp40[4];
    s32 var_a3;
    s32 var_t1;
    s32 var_v1;

    var_t1 = 0;
    var_v1 = 1;
    var_a3 = 0;
    while (var_a3 < 4) {
        if ((arg0->unk1C[var_a3] >= 0) && !(arg0->unk1B & var_v1) && (arg1 != arg0->unk1C[var_a3])) {
            sp40[var_t1] = arg0->unk1C[var_a3];
            var_t1 += 1;
        }
        var_a3++;
        var_v1 <<= 1;
    }
    if (var_t1 != 0) {
        if (arg2 > (var_t1 - 1)) {
            arg2 = var_t1 - 1;
        }
        if (arg2 == -1) {
            arg2 = rand_next(0, var_t1 - 1);
        }
        return sp40[arg2];
    }
    if (var_t1 && var_t1){}
    return -1;
}
#endif

// offset: 0x577C | func: 43 | export: 40
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_577C.s")
#else
// regalloc
s32 dll_26_func_577C(CurveSetup* arg0, s32 arg1, s32 arg2) {
    s32 sp40[4];
    s32 var_a3;
    s32 var_t1;
    s32 var_v1;

    var_t1 = 0;
    var_v1 = 1;
    var_a3 = 0;
    while (var_a3 < 4) {
        if ((arg0->unk1C[var_a3] >= 0) && (arg0->unk1B & var_v1) && (arg1 != arg0->unk1C[var_a3])) {
            sp40[var_t1] = arg0->unk1C[var_a3];
            var_t1 += 1;
        }
        var_a3++;
        var_v1 <<= 1;
    }
    if (var_t1 != 0) {
        if (arg2 > (var_t1 - 1)) {
            arg2 = var_t1 - 1;
        }
        if (arg2 == -1) {
            arg2 = rand_next(0, var_t1 - 1);
        }
        return sp40[arg2];
    }
    if (var_t1 && var_t1){}
    return -1;
}
#endif

// offset: 0x5860 | func: 44 | export: 41
s32 dll_26_func_5860(UnkCurvesStruct* arg0, s32 arg1, s32 arg2) {
    s32 var_v0_2;

    if (arg0 == NULL) {
        return 1;
    }
    
    if (arg2 != -1) {
        if (arg0->unk0x80 != 0) {
            arg0->unk0xA0 = dll_26_func_39C(arg2);
            arg2 = dll_26_func_438(arg0->unk0xA0, -1);
            if (arg2 == -1) {
                return 1;
            }
        }
        arg0->unk0xA0 = dll_26_func_39C(arg2);
        if (arg0->unk0xA0 == NULL) {
            arg0->unk0xA0 = NULL;
            return 1;
        }
        if (arg0->unk0x80 != 0) {
            var_v0_2 = dll_26_func_590(arg0->unk0xA0, -1);
        } else {
            var_v0_2 = dll_26_func_438(arg0->unk0xA0, -1);
        }
        if (var_v0_2 == -1) {
            return 1;
        }
        arg0->unk0xA4 = dll_26_func_39C(var_v0_2);
        if (arg0->unk0xA4 == NULL) {
            arg0->unk0xA4 = NULL;
            return 1;
        }
        
        arg0->unk0xB8[0] = arg0->unk0xA0->base.x;
        arg0->unk0xB8[1] = arg0->unk0xA4->base.x;
        arg0->unk0xB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
        arg0->unk0xB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
        
        arg0->unk0xD8[0] = arg0->unk0xA0->base.y;
        arg0->unk0xD8[1] = arg0->unk0xA4->base.y;
        arg0->unk0xD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA0->unk2D << 8)) * (f32) arg0->unk0xA0->unk2E);
        arg0->unk0xD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk0xA4->unk2D << 8)) * (f32) arg0->unk0xA4->unk2E);
        
        arg0->unk0xF8[0] = arg0->unk0xA0->base.z;
        arg0->unk0xF8[1] = arg0->unk0xA4->base.z;
        arg0->unk0xF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA0->unk2C << 8)) * (f32) arg0->unk0xA0->unk2E);
        arg0->unk0xF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk0xA4->unk2C << 8)) * (f32) arg0->unk0xA4->unk2E);
        
        if (dll_26_func_4704(arg0) != 0) {
            return 1;
        
        }

        arg0->unk0x94 = func_80004C5C;
        arg0->unk0x98 = (unk_curve_func_2)func_80004CE8;
        arg0->unk0x84 = arg0->unk0xA8;
        arg0->unk0x88 = arg0->unk0xC8;
        arg0->unk0x8C = arg0->unk0xE8;
        arg0->unk0x90 = 8;
        
        func_80005094(arg0);

        return 0;
    }
    return 1;
}

/*0x0*/ static const char str_0[] = "curves.c: MAX_ROMCURVES exceeded!!\n";
/*0x24*/ static const char str_24[] = "addCurveDef: max curves reached %d/%d\n";
/*0x4C*/ static const char str_4C[] = " Curve Id %i ";
/*0x5C*/ static const char str_5C[] = " CI %i ";
/*0x64*/ static const char str_64[] = "Search\n";
/*0x6C*/ static const char str_6C[] = "link:(%d) %d\n";
/*0x7C*/ static const char str_7C[] = "Found: %d\n";
/*0x88*/ static const char str_88[] = "%d\n";
/*0x8C*/ static const char str_8C[] = " list Branch No %i ";
/*0xA0*/ static const char str_A0[] = " list Branch %i Max %i Branch No %i ";
