#include "common.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s8 unk1A;
    s8 unk1B;
    s32 unk1C[4];
    s32 unk2C;
} CurveSetup;

typedef struct {
    u32 uID;
    CurveSetup *setup;
} CurveNode;

/*0x0*/ static s32 _bss_0;
/*0x4*/ static s32 _bss_4;
/*0x8*/ static CurveNode _bss_8[1300];
/*0x10*/ //static u8 _bss_10[0x2898];
/*0x28A8*/ static s32 _bss_28A8;

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
u32 dll_26_func_1E4(f32 x, f32 y, f32 z, s32 *arg3, s32 arg4, s32 arg5) {
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

        if (_bss_8[temp_t6].uID < curveUID) {
            min = temp_t6 + 1;
        } else if (_bss_8[temp_t6].uID > curveUID) {
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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_4F0.s")
#else
s32 dll_26_func_4F0(CurveSetup* arg0, s32 arg1, s32* arg2) {
    s32 var_v1;
    s32 i;

    var_v1 = 0;
    
    for (i = 0; i < 4; i++) {
        if ((arg0->unk1C[i] >= 0) && (arg1 != arg0->unk1C[i])) {
            arg2[var_v1] = arg0->unk1C[i];
            var_v1++;
        }
    }

    return var_v1;
}
#endif

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
#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_854.s")
#else
// logically sound, needs work
s32 dll_26_func_854(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 spAC[2];
    f32 sp9C[2];
    Vec3f sp6C[3];
    CurveSetup* temp_v0;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 temp_fv0_2;
    f32* temp_v1;
    s32 temp_s2;
    s32 var_s1;

    spAC[0] = -1;
    spAC[1] = -1;
    sp9C[1] = 0.0f;
    sp9C[0] = 0.0f;
    sp6C[0].x = arg0->base.x;
    sp6C[0].y = arg0->base.y;
    sp6C[0].z = arg0->base.z;
    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        if (arg0->unk1C[var_s1] >= 0) {
            temp_v0 = dll_26_func_39C(arg0->unk1C[var_s1]);
            if (temp_v0 != NULL) {
                temp_s2 = arg1 == arg0->unk1C[var_s1];
                sp6C[1].x = temp_v0->base.x;
                sp6C[1].y = temp_v0->base.y;
                sp6C[1].z = temp_v0->base.z;
                dll_26_func_648(arg2, arg3, arg4, sp6C);
                temp_v1 = &sp9C[temp_s2];
                temp_fv0 = sp6C[2].x - arg2;
                temp_fv1 = sp6C[2].y - arg3;
                temp_fa0 = sp6C[2].z - arg4;
                temp_fv0_2 = (temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0);
                if (*temp_v1 < temp_fv0_2) {
                    *temp_v1 = temp_fv0_2;
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_B8C.s")

// offset: 0xD8C | func: 13 | export: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_D8C.s")

// offset: 0xE40 | func: 14 | export: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_E40.s")

// offset: 0x1420 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_1420.s")

// offset: 0x14C8 | func: 16 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_14C8.s")

// offset: 0x14F4 | func: 17 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_14F4.s")

// offset: 0x174C | func: 18 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_174C.s")

// offset: 0x17F4 | func: 19 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_17F4.s")

// offset: 0x1880 | func: 20 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_1880.s")

// offset: 0x1924 | func: 21 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_1924.s")

// offset: 0x19C8 | func: 22 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_19C8.s")

// offset: 0x1B90 | func: 23 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_1B90.s")

// offset: 0x218C | func: 24 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_218C.s")

// offset: 0x21FC | func: 25 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_21FC.s")

// offset: 0x277C | func: 26 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_277C.s")

// offset: 0x291C | func: 27 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_291C.s")

// offset: 0x2AE8 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_2AE8.s")

// offset: 0x2C20 | func: 29 | export: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_2C20.s")

// offset: 0x3018 | func: 30 | export: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_3018.s")

// offset: 0x31E0 | func: 31 | export: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_31E0.s")

// offset: 0x32D8 | func: 32 | export: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_32D8.s")

// offset: 0x35AC | func: 33 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_35AC.s")

// offset: 0x374C | func: 34 | export: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_374C.s")

// offset: 0x3930 | func: 35 | export: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_3930.s")

// offset: 0x3F00 | func: 36 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_3F00.s")

// offset: 0x4288 | func: 37 | export: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_4288.s")

// offset: 0x4704 | func: 38 | export: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_4704.s")

// offset: 0x4CB4 | func: 39 | export: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_4CB4.s")

// offset: 0x4FA8 | func: 40 | export: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_4FA8.s")

// offset: 0x50E4 | func: 41 | export: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_50E4.s")

// offset: 0x5698 | func: 42 | export: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_5698.s")

// offset: 0x577C | func: 43 | export: 40
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_577C.s")

// offset: 0x5860 | func: 44 | export: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/26_curves/dll_26_func_5860.s")

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
