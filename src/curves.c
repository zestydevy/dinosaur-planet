#include "PR/gu.h"
#include "sys/curves.h"
#include "sys/main.h"

// .data

s32 D_8008C7D0 = -1;
s32 D_8008C7D4 = -1;

// .bss

f32 D_800A7C30[4];
f32 D_800A7C40;
f32 D_800A7C48[7];
f32 D_800A7C64;


// // // 


static const char str_800983b0[] = "curvesMove: There must be at least four control points\n";
static const char str_800983e8[] = "curvesMove: There must be a multiple of four control points for bezier or hermite curves\n";
static const char str_80098444[] = "curvesSetupMoveNetworkCurve: There must be at least four control points\n";
static const char str_80098490[] = "curvesSetupMoveNetworkCurve: There must be a multiple of four control points for bezier or hermite curves\n";

#define ONE_OVER_SIX (1.0f / 6.0f)

void func_80004B78(Vec4f *a0, Vec4f *a1);

void func_80004DFC(Vec4f *in, Vec4f *out);
void func_800065C0(UnkCurvesStruct *, s32);

// // // bezier?

f32 func_80004A60(Vec4f *a0, f32 a1, f32 *a2) {
    Vec4f v;
    
    v.x = a0->w + (((3.0f * a0->y) - a0->x) + (-3.0f * a0->z));
    v.y = (3.0f * a0->z) + ((3.0f * a0->x) + (-6.0f * a0->y));
    v.z = (3.0f * a0->z) + (-3.0f * a0->x);
    v.w = a0->z + (a0->x + (4.0f * a0->y));

    v = v; // from permuter
    
    if (a2 != NULL) {
        *a2 = ((v.z * 1.0f) + ((((3.0f * v.x) * a1) + (v.y + v.y)) * a1)) * ONE_OVER_SIX;
    }

    return ((v.w * 1.0f) + ((((v.x * a1) + v.y) * a1) + v.z) * a1) * ONE_OVER_SIX;
}

void func_80004B78(Vec4f *a0, Vec4f *a1) {
    a1->x = a0->w + (((3.0f * a0->y) - a0->x) + (-3.0f * a0->z));
    a1->y = (3.0f * a0->z) + ((3.0f * a0->x) + (-6.0f * a0->y));
    a1->z = (3.0f * a0->z) + (-3.0f * a0->x);
    a1->w = a0->z + (a0->x + (4.0f * a0->y));
    
    a1->x *= ONE_OVER_SIX;
    a1->y *= ONE_OVER_SIX;
    a1->z *= ONE_OVER_SIX;
    a1->w *= ONE_OVER_SIX;
}

/** Returns rate of change of curve at tValue along its path? */
f32 func_80004C5C(Vec4f *a0, f32 tValue, f32 *a2) {
    Vec4f v;

    func_80004CE8(a0, &v);

    if (a2 != NULL) {
        *a2 = (v.z * 1.0f) + (((3.0f * v.x * tValue) + (v.y + v.y)) * tValue);
    }

    return (v.w * 1.0f) + ((((v.x * tValue + v.y) * tValue) + v.z) * tValue);
}

void func_80004CE8(Vec4f *in, Vec4f *out) {
    out->x = in->w + ((in->x + in->x) + (-2.0f * in->y) + in->z);
    out->y = -in->w + ((-3.0f * in->x) + (3.0f * in->y) + (-2.0f * in->z));
    out->z = in->z;
    out->w = in->x;
}

// // // hermite?

f32 func_80004D70(Vec4f *a0, f32 a1, f32 *a2) {
    Vec4f v;

    func_80004DFC(a0, &v);

    if (a2 != NULL) {
        *a2 = (v.z * 1.0f) + (((3.0f * v.x * a1) + (v.y + v.y)) * a1);
    }

    return (v.w * 1.0f) + ((((v.x * a1 + v.y) * a1) + v.z) * a1);
}

void func_80004DFC(Vec4f *in, Vec4f *out) {
    out->x = in->w + (((3.0f * in->y) - in->x) + (-3.0f * in->z));
    out->y = (3.0f * in->z) + ((3.0f * in->x) + (-6.0f * in->y));
    out->z = (3.0f * in->y) + (-3.0f * in->x);
    out->w = in->x;
}

f32 func_80004E90(Vec4f *a0, f32 a1, f32 *a2) {
    Vec4f v;
    f32 temp;
    
    v.x = a0->w + (((3.0f * a0->y) - a0->x) + (-3.0f * a0->z));
    v.y = -a0->w + (((a0->x + a0->x) + (-5.0f * a0->y)) + (4.0f * a0->z));
    v.z = a0->z - a0->x;
    temp = a0->y;
    v.w = temp + temp;

    v = v; // from permuter
    
    if (a2 != NULL) {
        *a2 = (v.z * 1.0f) + ((((3.0f * v.x) * a1) + (v.y + v.y)) * a1);
    }

    return ((v.w * 1.0f) + (((((v.x * a1) + v.y) * a1) + v.z) * a1)) * 0.5f;
}

void func_80004F90(Vec4f *a0, Vec4f *a1) {
    f32 temp;
    a1->x = a0->w + (((3.0f * a0->y) - a0->x) + (-3.0f * a0->z));
    a1->y = -a0->w + (((a0->x + a0->x) + (-5.0f * a0->y)) + (4.0f * a0->z));
    a1->z = a0->z - a0->x;
    temp = a0->y;
    a1->w = temp + temp;
    
    a1->x *= 0.5f;
    a1->y *= 0.5f;
    a1->z *= 0.5f;
    a1->w *= 0.5f;
}

// // //

f32 linear_interp(f32 values[2], f32 time, void *a2) {
    return values[0] + (values[1] - values[0]) * time;
}

void func_80005084(void *a0, void *a1) {

}

void func_80005094(UnkCurvesStruct *arg0) {
    if (arg0->unk90) {

    }
    if ((arg0->unk94 == &func_80004D70) || (arg0->unk94 == &func_80004C5C)) {

    }

    arg0->unk10 = 0;
    arg0->unkC = 0.0f;

    while (arg0->unk10 < (arg0->unk90 - 3)) {
        func_800065C0(arg0, 5);
        
        arg0->unkC += arg0->unk14[0];
        
        if ((arg0->unk94 == &func_80004D70) || (arg0->unk94 == &func_80004C5C)) {
            arg0->unk10 += 4;
        } else {
            arg0->unk10 += 1;
        }
    }

    if (arg0->unk80 != 0) {
        arg0->unk10 = arg0->unk90 - 4;
    } else {
        arg0->unk10 = 0;
    }

    func_800065C0(arg0, 0x14);

    if (arg0->unk80 != 0) {
        arg0->unk0 = 1.0f;
        arg0->unk4 = arg0->unk64;
        arg0->unk8 = arg0->unkC;
    } else {
        arg0->unk0 = 0.0f;
        arg0->unk4 = 0.0f;
        arg0->unk8 = 0.0f;
    }

    if (arg0->unk84 != NULL) {
        arg0->unk68 = arg0->unk94(arg0->unk84, arg0->unk0, &arg0->unk74);
    }
    if (arg0->unk88 != NULL) {
        arg0->unk6C = arg0->unk94(arg0->unk88, arg0->unk0, &arg0->unk78);
    }
    if (arg0->unk8C != NULL) {
        arg0->unk70 = arg0->unk94(arg0->unk8C, arg0->unk0, &arg0->unk7C);
    }
}

void func_8000523C(UnkCurvesStruct *arg0) {
    if (arg0->unk90) {

    }
    if ((arg0->unk94 == &func_80004D70) || (arg0->unk94 == &func_80004C5C)) {

    }

    arg0->unk10 = 0;
    arg0->unkC = 0.0f;
    
    while (arg0->unk10 < (arg0->unk90 - 3)) {
        func_800065C0(arg0, 5);

        arg0->unkC += arg0->unk14[0];

        if ((arg0->unk94 == &func_80004D70) || (arg0->unk94 == &func_80004C5C)) {
            arg0->unk10 += 4;
        } else {
            arg0->unk10 += 1;
        }
    }

    if (arg0->unk80 != 0) {
        arg0->unk10 = arg0->unk90 - 4;
    } else {
        arg0->unk10 = 0;
    }

    func_800065C0(arg0, 0x14);
    
    if (arg0->unk80 != 0) {
        arg0->unk8 = arg0->unkC - arg0->unk4;
    } else {
        arg0->unk8 = arg0->unk4;
    }
}

void func_8000535C(UnkCurvesStruct *arg0) {
    if (arg0->unk80 != 0) {
        arg0->unk0 = 1.0f;
        arg0->unk4 = arg0->unk64;
        arg0->unk8 = arg0->unkC;
        arg0->unk10 = arg0->unk90 - 4;
    } else {
        arg0->unk0 = 0.0f;
        arg0->unk4 = 0.0f;
        arg0->unk8 = 0.0f;
        arg0->unk10 = 0;
    }
}

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_800053B0.s")
#else
s32 func_800053B0(UnkCurvesStruct* arg0, f32 arg1) {
    f32 temp_fs1;
    s32 var_t0;
    f32 temp_fv0;
    f32 var_fs0;
    s32 var_v1;
    f32* var_v0;
    f32 *s1;
    s32 pad[2];

    var_fs0 = arg1 * gUpdateRateF;
    if (var_fs0 > 0.0f) {
        var_v1 = (s32) (arg0->unk0 * 20.0f);
        if (var_v1 == 20) {
            var_v1 -= 1;
        }
        if (arg0->unk80 != 0) {
            arg0->unk4 = arg0->unk14[1 + var_v1] + arg0->unk4;
        } else if (arg0->unk0 >= 1.0f) {
            return 1;
        }
        arg0->unk8 += var_fs0;
        var_fs0 += arg0->unk4;
        var_v0 = &arg0->unk14[var_v1];
        s1 = var_v0;
        while (var_fs0 > 0.0f) {
            var_fs0 -= var_v0[1];
            if (var_fs0 > 0.0f) {
                var_v1 += 1;
                var_v0 += 1;
                if (var_v1 >= 20) {
                    var_t0 = arg0->unk10;
                    if ((arg0->unk94 == func_80004D70) || (arg0->unk94 == func_80004C5C)) {
                        arg0->unk10 += 3;
                    }
                    arg0->unk10 += 1;
                    if ((arg0->unk90 - 4) < arg0->unk10) {
                        if (arg0->unk84 != NULL) {
                            arg0->unk68 = arg0->unk94((Vec4f *) &arg0->unk84[var_t0], 1.0f, &arg0->unk74);
                        }
                        if (arg0->unk88 != NULL) {
                            arg0->unk6C = arg0->unk94((Vec4f *) &arg0->unk88[var_t0], 1.0f, &arg0->unk78);
                        }
                        if (arg0->unk8C != NULL) {
                            arg0->unk70 = arg0->unk94((Vec4f *) &arg0->unk8C[var_t0], 1.0f, &arg0->unk7C);
                        }
                        arg0->unk4 = 0.0f;
                        arg0->unk10 = arg0->unk90 - 4;
                        arg0->unk0 = 1.0f;
                        arg0->unk8 = arg0->unkC;
                        return 1;
                    }
                    func_800065C0(arg0, 20);
                    var_v1 = 0;
                    var_v0 = s1;
                }
            }
        }
        var_fs0 += var_v0[1];
        temp_fs1 = ((var_fs0 / var_v0[1]) * (((var_v1 + 1) / 20.0f) - (var_v1 / 20.0f))) + (var_v1 / 20.0f);
        if (arg0->unk84 != NULL) {
            arg0->unk68 = arg0->unk94((Vec4f* ) &arg0->unk84[arg0->unk10], temp_fs1, &arg0->unk74);
        }
        if (arg0->unk88 != NULL) {
            arg0->unk6C = arg0->unk94((Vec4f* ) &arg0->unk88[arg0->unk10], temp_fs1, &arg0->unk78);
        }
        if (arg0->unk8C != NULL) {
            arg0->unk70 = arg0->unk94((Vec4f* ) &arg0->unk8C[arg0->unk10], temp_fs1, &arg0->unk7C);
        }
        arg0->unk0 = temp_fs1;
        arg0->unk4 = var_fs0;
        arg0->unk80 = 0;
    } else if (var_fs0 < 0.0f) {
        var_v1 = (s32) (arg0->unk0 * 20.0f);
        if (var_v1 == 20) {
            var_v1 -= 1;
        }
        if (arg0->unk80 == 0) {
            arg0->unk4 = arg0->unk14[ 1 + var_v1] - arg0->unk4;
        } else if (arg0->unk0 <= 0.0f) {
            return 1;
        }
        var_v0 = &arg0->unk14[var_v1];
        s1 = var_v0;
        arg0->unk8 += var_fs0;
        var_fs0 += arg0->unk4;
        while (var_fs0 < 0.0f) {
            var_fs0 += var_v0[1];
            if (var_fs0 < 0.0f) {
                var_v1 -= 1;
                var_v0 -= 1;
                if (var_v1 < 0) {
                    var_t0 = arg0->unk10;
                    if ((arg0->unk94 == func_80004D70) || (arg0->unk94 == func_80004C5C)) {
                        arg0->unk10 -= 3;
                    }
                    arg0->unk10 -= 1;
                    if (arg0->unk10 < 0) {
                        if (arg0->unk84 != NULL) {
                            arg0->unk68 = arg0->unk94((Vec4f* ) &arg0->unk84[var_t0], 0.0f, &arg0->unk74);
                        }
                        if (arg0->unk88 != NULL) {
                            arg0->unk6C = arg0->unk94((Vec4f* ) &arg0->unk88[var_t0], 0.0f, &arg0->unk78);
                        }
                        if (arg0->unk8C != NULL) {
                            arg0->unk70 = arg0->unk94((Vec4f* ) &arg0->unk8C[var_t0], 0.0f, &arg0->unk7C);
                        }
                        arg0->unk0 = 0.0f;
                        arg0->unk4 = -s1[1];
                        arg0->unk8 = 0.0f;
                        arg0->unk10 = 0;
                        return 1;
                    }
                    func_800065C0(arg0, 20);
                    var_v1 = 19;
                    var_v0 = s1;
                }
            }
        }
        temp_fs1 = ((var_fs0 / var_v0[1]) * (((var_v1 + 1) / 20.0f) - (var_v1 / 20.0f))) + (var_v1 / 20.0f);
        if (arg0->unk84 != NULL) {
            arg0->unk68 = arg0->unk94((Vec4f* ) &arg0->unk84[arg0->unk10], temp_fs1, &arg0->unk74);
        }
        if (arg0->unk88 != NULL) {
            arg0->unk6C = arg0->unk94((Vec4f* ) &arg0->unk88[arg0->unk10], temp_fs1, &arg0->unk78);
        }
        if (arg0->unk8C != NULL) {
            arg0->unk70 = arg0->unk94((Vec4f* ) &arg0->unk8C[arg0->unk10], temp_fs1, &arg0->unk7C);
        }
        arg0->unk0 = temp_fs1;
        arg0->unk4 = var_fs0 - var_v0[1];
        arg0->unk80 = 1;
    }
    return 0;
}
#endif

void func_8000598C(f32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4, f32* arg5, s32 arg6, unk_curve_func_2 arg7) {
    f32 spC0[4];
    f32 spB0[4];
    f32 spA0[4];
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    s32 i;

    if (arg6 != D_8008C7D0) {
        D_800A7C30[0] = SQ(1.0f / arg6);
        D_800A7C30[1] = D_800A7C30[0] * 2;
        D_800A7C30[2] = D_800A7C30[0] * (1.0f / arg6);
        D_800A7C30[3] = D_800A7C30[2] * 6.0f;
        D_8008C7D0 = arg6;
        D_800A7C40 = 1.0f / arg6;
    }

    if (arg0 != NULL) {
        arg7(arg0, spC0);
        sp9C = spC0[3];
        sp98 = (D_800A7C30[2] * spC0[0]) + (D_800A7C30[0] * spC0[1]) + (D_800A7C40 * spC0[2]);
        sp94 = (D_800A7C30[3] * spC0[0]) + (D_800A7C30[1] * spC0[1]);
        sp90 = D_800A7C30[3] * spC0[0];
    }

    if (arg1 != NULL) {
        arg7(arg1, spB0);
        sp8C = spB0[3];
        sp88 = (D_800A7C30[2] * spB0[0]) + (D_800A7C30[0] * spB0[1]) + (D_800A7C40 * spB0[2]);
        sp84 = (D_800A7C30[3] * spB0[0]) + (D_800A7C30[1] * spB0[1]);
        sp80 = D_800A7C30[3] * spB0[0];
    }

    if (arg2 != NULL) {
        arg7(arg2, spA0);
        sp7C = spA0[3];
        sp78 = (D_800A7C30[2] * spA0[0]) + (D_800A7C30[0] * spA0[1]) + (D_800A7C40 * spA0[2]);
        sp74 = (D_800A7C30[3] * spA0[0]) + (D_800A7C30[1] * spA0[1]);
        sp70 = (D_800A7C30[3] * spA0[0]);
    }

    // @fake
    D_8008C7D0 = D_8008C7D0;

    for (i = 0; i <= arg6; i++) {
        if (arg0 != NULL) {
            arg3[i] = sp9C;
            sp9C += sp98;
            sp98 += sp94;
            sp94 += sp90;
        }
        if (arg1 != NULL) {
            arg4[i] = sp8C;
            sp8C += sp88;
            sp88 += sp84;
            sp84 += sp80;
        }
        if (arg2 != NULL) {
            arg5[i] = sp7C;
            sp7C += sp78;
            sp78 += sp74;
            sp74 += sp70;
        }
    }
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_80005E60.s")
#else
void func_80005E60(f32 *arg0, f32 *arg1, f32* arg2, f32* arg3, f32 *arg4, f32 *arg5, f32 *arg6, f32 *arg7, f32 *arg8, s32 arg9, void (*arg10)(f32*, f32*, f32*, f32*)) {
    f32 spC0[4];
    f32 spB0[4];
    f32 spA0[4];
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 temp_fv1;
    s32 i;

    if (arg9 != D_8008C7D4) {
        temp_fv1 = 1.0f / arg9;
        D_800A7C48[0] = temp_fv1 * temp_fv1;
        D_800A7C48[1] = D_800A7C48[0] * 2;
        D_800A7C48[2] = D_800A7C48[0] * temp_fv1;
        D_800A7C48[3] = D_800A7C48[2] * 6.0f;
        D_800A7C48[4] = D_800A7C48[0] * 3.0f;
        D_800A7C48[5] = D_800A7C48[0] * 6.0f;
        D_800A7C48[6] = 2 * temp_fv1;
        // @fake
        do { } while (0);
        D_8008C7D4 = arg9;
        D_800A7C64 = temp_fv1;
    }
    if (arg0 != NULL) {
        arg10(arg0, spC0, D_800A7C48, arg3);
        sp9C = spC0[3];
        sp98 = (D_800A7C48[2] * spC0[0]) + (D_800A7C48[0] * spC0[1]) + (D_800A7C64 * spC0[2]);
        sp94 = (D_800A7C48[3] * spC0[0]) + (D_800A7C48[1] * spC0[1]);
        sp90 = D_800A7C48[3] * spC0[0];
    }
    if (arg1 != NULL) {
        arg10(arg1, spB0, D_800A7C48, arg3);
        sp8C = spB0[3];
        sp88 = (D_800A7C48[2] * spB0[0]) + (D_800A7C48[0] * spB0[1]) + (D_800A7C64 * spB0[2]);
        sp84 = (D_800A7C48[3] * spB0[0]) + (D_800A7C48[1] * spB0[1]);
        sp80 = D_800A7C48[3] * spB0[0];
    }
    if (arg2 != NULL) {
        arg10(arg2, spA0, D_800A7C48, arg3);
        sp7C = spA0[3];
        sp78 = (D_800A7C48[2] * spA0[0]) + (D_800A7C48[0] * spA0[1]) + (D_800A7C64 * spA0[2]);
        sp74 = (D_800A7C48[3] * spA0[0]) + (D_800A7C48[1] * spA0[1]);
        sp70 = (D_800A7C48[3] * spA0[0]);
    }
    for (i = 0; i <= arg9; i++) {
        if (arg0 != NULL) {
            arg3[i] = sp9C;
            sp9C += sp98;
            sp98 += sp94;
            sp94 += sp90;
        }
        if (arg1 != NULL) {
            arg4[i] = sp8C;
            sp8C += sp88;
            sp88 += sp84;
            sp84 += sp80;
        }
        if (arg2 != NULL) {
            arg5[i] = sp7C;
            sp7C += sp78;
            sp78 += sp74;
            sp74 += sp70;
        }
    }

    if (arg0 != NULL) {
        sp9C = spC0[2];
        sp98 = (D_800A7C48[4] * spC0[0]) + (D_800A7C48[6] * spC0[1]);
        sp94 = D_800A7C48[5] * spC0[0];
    }
    if (arg1 != NULL) {
        sp8C = spB0[2];
        sp88 = (D_800A7C48[4] * spB0[0]) + (D_800A7C48[6] * spB0[1]);
        sp84 = D_800A7C48[5] * spB0[0];
        // @fake
        D_8008C7D4 = D_8008C7D4;
    }
    if (arg2 != NULL) {
        sp7C = spA0[2];
        sp78 = (D_800A7C48[4] * spA0[0]) + (D_800A7C48[6] * spA0[1]);
        sp74 = D_800A7C48[5] * spA0[0];
    }

    for (i = 0; i <= arg9; i++) {
        if (arg0 != NULL) {
            arg6[i] = sp9C;
            sp9C += sp98;
            sp98 += sp94;
        }
        if (arg1 != NULL) {
            arg7[i] = sp8C;
            sp8C += sp88;
            sp88 += sp84;
        }
        if (arg2 != NULL) {
            arg8[i] = sp7C;
            sp7C += sp78;
            sp78 += sp74;
        }
    }
}
#endif

void func_800065C0(UnkCurvesStruct *arg0, s32 arg1) {
    f32 *phi_s4;
    f32 *phi_s5;
    f32 *phi_s6;
    f32 xCoords[21];
    f32 yCoords[21];
    f32 zCoords[21];
    f32 xDist;
    f32 yDist;
    f32 zDist;
    s32 i;

    phi_s4 = 0;
    phi_s5 = 0;
    phi_s6 = 0;

    if (arg0->unk84 != 0) {
        phi_s4 = &arg0->unk84[arg0->unk10];
    }
    if (arg0->unk88 != 0) {
        phi_s5 = &arg0->unk88[arg0->unk10];
    }
    if (arg0->unk8C != 0) {
        phi_s6 = &arg0->unk8C[arg0->unk10];
    }

    if (arg0->unk98 != 0) {
        func_8000598C(phi_s4, phi_s5, phi_s6, xCoords, yCoords, zCoords, arg1, arg0->unk98);
    } else {
        
    }

    arg0->unk14[0] = 0.0f;

    for (i = 0; i < arg1; ++i) {
        // Find distance between current and next coord
        if (phi_s4 != 0) {
            xDist = xCoords[i + 1] - xCoords[i];
        } else {
            xDist = 0.0f;
        }

        if (phi_s5 != 0) {
            yDist = yCoords[i + 1] - yCoords[i];
        } else {
            yDist = 0.0f;
        }

        if (phi_s6 != 0) {
            zDist = zCoords[i + 1] - zCoords[i];
        } else {
            zDist = 0.0f;
        }

        // Compute distance between current and next coord
        arg0->unk14[i + 1] = sqrtf((xDist * xDist) + (yDist * yDist) + (zDist * zDist));
        // Sum distances
        arg0->unk14[0] += arg0->unk14[i + 1];
    }
}

f32 func_80006784(Unk80006784* arg0, s32 arg1, f32 arg2, f32 arg3) {
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;
    f32 var_fs0;
    f32 var_fs1;
    f32 var_fs2;
    Unk80006784* temp_s0;
    s32 var_v0;

    var_v0 = 0;
    while (var_v0 < arg1 && arg0[var_v0].unk0[0] < arg2) {
        var_v0++;
    }
    if (var_v0 == 0) {
        return 0.0f;
    }
    if (var_v0 >= arg1) {
        return 1.0f;
    }
    var_v0--;
    var_fs1 = 0.0f;
    var_fs2 = 1.0f;
    temp_s0 = &arg0[var_v0];
    var_fs0 = (arg2 - temp_s0->unk0[0]) * arg3;
    do {
        temp_fv0 = (var_fs1 + var_fs2) * 0.5f;
        temp_fv0_2 = func_80006CFC(temp_s0, var_fs1, temp_fv0);
        temp_fv1 = temp_fv0_2 + 0.0001f;
        if (var_fs0 < temp_fv1) {
            var_fs2 = temp_fv0;
        } else {
            var_fs1 = temp_fv0;
            var_fs0 -= temp_fv0_2;
        }
    } while (((temp_fv1 < var_fs0) || (var_fs0 < (temp_fv0_2 - 0.0001f))) && (var_fs1 != var_fs2));
    return temp_fv0;
}

f32 func_80006908(Unk80006908* arg0, s32 arg1, f32 arg2) {
    f32 temp_fa0;
    f32 temp_fv0;
    f32 var_ft4;
    f32 var_ft5;
    f32 var_fv1;
    s32 var_v0;
    s32 pad[1];
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    Vec4f temp;
    Vec4f temp2;

    if (arg1 == 0) {
        return 0.0f;
    }
    var_v0 = 0;
    while ((arg1 > var_v0) && (arg0[var_v0].unk20 < arg2)) {
        var_v0 += 1;
    }
    if (var_v0 >= arg1) {
        var_fv1 = 1.0f;
    } else if (var_v0 == 0) {
        var_fv1 = 0.0f;
    } else {
        var_ft4 = 0.0f;
        var_ft5 = 1.0f;
        var_v0--;
        sp48 = arg0[var_v0].unk0.x;
        sp44 = arg0[var_v0].unk0.y;
        sp40 = arg0[var_v0].unk0.z;
        sp3C = arg0[var_v0].unk0.w;
        temp.x = arg0[var_v0].unk10.x;
        temp.y = arg0[var_v0].unk10.y;
        temp.z = arg0[var_v0].unk10.z;
        temp.w = arg0[var_v0].unk10.w;
        do {
            temp_fv0 = (var_ft4 + var_ft5) * 0.5f;
            temp_fa0 = (((((temp.x * temp_fv0) + temp.y) * temp_fv0) + temp.z) * temp_fv0) + temp.w;
            if (arg2 < (temp_fa0 + 0.0001f)) {
                var_ft5 = temp_fv0;
            } else {
                var_ft4 = temp_fv0;
            }
        } while(((temp_fa0 + 0.0001f) < arg2 || arg2 < (temp_fa0 - 0.0001f)) && var_ft4 != var_ft5);
        var_fv1 = (((((sp48 * temp_fv0) + sp44) * temp_fv0) + sp40) * temp_fv0) + sp3C;
    }
    if (var_fv1 > 1.0f) {
        var_fv1 = 1.0f;
    } else if (var_fv1 < 0.0f) {
        var_fv1 = 0.0f;
    }
    return var_fv1;
}

void func_80006B28(Vec2f* arg0, s32 arg1, Unk80006908* arg2) {
    f32 sp88[4];
    f32 sp78[4];
    f32 sp68[4];
    s32 var_a1;
    s32 var_s3;
    s32 i;

    for (var_s3 = 0; var_s3 < arg1 - 1; var_s3++) {
        var_a1 = var_s3 - 1;
        for (i = 0; i < 4; i++) {
            if (var_a1 < 0) {
                sp88[i] = arg0[var_a1 + 1].x + (arg0[var_a1 + 1].x - arg0[var_a1 + 2].x);
                sp78[i] = arg0[var_a1 + 1].y + (arg0[var_a1 + 1].y - arg0[var_a1 + 2].y);
            } else if (var_a1 >= arg1) {
                sp88[i] = arg0[var_a1 + -1].x + (arg0[var_a1 + -1].x - arg0[var_a1 + -2].x);
                sp78[i] = arg0[var_a1 + -1].y + (arg0[var_a1 + -1].y - arg0[var_a1 + -2].y);
            } else {
                sp88[i] = arg0[var_a1 + 0].x;
                sp78[i] = arg0[var_a1 + 0].y;
            }
            var_a1++;
        }
        func_80004B78((Vec4f *)sp88, (Vec4f *)sp68);
        arg2[var_s3].unk0.x = sp68[0];
        arg2[var_s3].unk0.y = sp68[1];
        arg2[var_s3].unk0.z = sp68[2];
        arg2[var_s3].unk0.w = sp68[3];
        func_80004B78((Vec4f *)sp78, (Vec4f *)sp68);
        arg2[var_s3].unk10.x = sp68[0];
        arg2[var_s3].unk10.y = sp68[1];
        arg2[var_s3].unk10.z = sp68[2];
        arg2[var_s3].unk10.w = sp68[3];
        arg2[var_s3].unk20 = func_80004A60((Vec4f *)sp78, 0, 0);
    }
    arg2->unk20 = 0;
    arg2[arg1 - 1].unk20 = 1.0f;
}

f32 func_80006CFC(Unk80006784 *arg0, f32 arg1, f32 arg2) {
    f32 var_fs0;
    f32 sp48;
    f32 var_fs1;
    s32 i;

    var_fs1 = 0.0f;
    sp48 = (arg2 - arg1) / 25.0f;
    var_fs0 = arg1 + sp48;
    for (i = 2; i < 0x1A; i++) {
        if (!(i & 1)) {
            var_fs1 += 4.0f * func_80006E04(arg0, var_fs0);
        } else {
            var_fs1 += 2.0f * func_80006E04(arg0, var_fs0);
        }
        var_fs0 += sp48;
    }
    var_fs0 = func_80006E04(arg0, arg1);
    return ((func_80006E04(arg0, arg2) + (var_fs0 + var_fs1)) * sp48) / 3.0f;
}

f32 func_80006E04(Unk80006784* arg0, f32 arg1) {
    return sqrtf(arg0->unk0[5] +
        (arg0->unk0[4] +
            (arg0->unk0[3] +
                (arg0->unk0[2] +
                     (arg0->unk0[1] * arg1)
                ) * arg1
            ) * arg1
        ) * arg1
    );
}

void func_80006E58(UnkCurvesStruct* arg0, f32 arg1, f32 arg2, s32 arg3) {
    f32 temp_fv0;
    f32 var_fa0;
    f32 var_fa1;
    f32 var_ft4;
    f32 var_ft5;
    s32 var_v0;

    arg0->unk0 = 0.0f;
    var_ft5 = 0.0f;
    var_fa0 = 0.0f;
    var_fa1 = 0.0f;
    var_ft4 = 0.0f;
    for (var_v0 = 0; var_v0 < arg0->unk90; var_v0 += 4) {
        if (arg0->unk84 != NULL) {
            var_ft4 = arg1 - arg0->unk84[var_v0];
        }
        if (arg0->unk88 != NULL) {
            var_fa1 = arg2 - arg0->unk88[var_v0];
        }
        if (arg0->unk8C != NULL) {
            var_fa0 = arg1 - arg0->unk8C[var_v0];
        }
        temp_fv0 = (var_ft4 * var_ft4) + (var_fa1 * var_fa1) + (var_fa0 * var_fa0);
        if ((var_v0 == 0) || (temp_fv0 < var_ft5)) {
            arg0->unk10 = var_v0;
            var_ft5 = temp_fv0;
        }
    }
    func_800065C0(arg0, 0x14);
}
