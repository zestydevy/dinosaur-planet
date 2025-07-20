#include "sys/curves.h"

extern s32 D_8008C7D0;
extern f32 D_800A7C30[4];
extern f32 D_800A7C40;

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

f32 func_80004C5C(Vec4f *a0, f32 a1, f32 *a2) {
    Vec4f v;

    func_80004CE8(a0, &v);

    if (a2 != NULL) {
        *a2 = (v.z * 1.0f) + (((3.0f * v.x * a1) + (v.y + v.y)) * a1);
    }

    return (v.w * 1.0f) + ((((v.x * a1 + v.y) * a1) + v.z) * a1);
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
    if (arg0->unk0x90) {

    }
    if ((arg0->unk0x94 == &func_80004D70) || (arg0->unk0x94 == &func_80004C5C)) {

    }

    arg0->unk0x10 = 0;
    arg0->unk0xC = 0.0f;

    while (arg0->unk0x10 < (arg0->unk0x90 - 3)) {
        func_800065C0(arg0, 5);
        
        arg0->unk0xC += arg0->unk0x14;
        
        if ((arg0->unk0x94 == &func_80004D70) || (arg0->unk0x94 == &func_80004C5C)) {
            arg0->unk0x10 += 4;
        } else {
            arg0->unk0x10 += 1;
        }
    }

    if (arg0->unk0x80 != 0) {
        arg0->unk0x10 = arg0->unk0x90 - 4;
    } else {
        arg0->unk0x10 = 0;
    }

    func_800065C0(arg0, 0x14);

    if (arg0->unk0x80 != 0) {
        arg0->unk0x0 = 1.0f;
        arg0->unk0x4 = arg0->unk0x64;
        arg0->unk0x8 = arg0->unk0xC;
    } else {
        arg0->unk0x0 = 0.0f;
        arg0->unk0x4 = 0.0f;
        arg0->unk0x8 = 0.0f;
    }

    if (arg0->unk0x84 != NULL) {
        arg0->unk0x68 = arg0->unk0x94(arg0->unk0x84, arg0->unk0x0, &arg0->unk0x74);
    }
    if (arg0->unk0x88 != NULL) {
        arg0->unk0x6C = arg0->unk0x94(arg0->unk0x88, arg0->unk0x0, &arg0->unk0x78);
    }
    if (arg0->unk0x8C != NULL) {
        arg0->unk0x70 = arg0->unk0x94(arg0->unk0x8C, arg0->unk0x0, &arg0->unk0x7C);
    }
}

void func_8000523C(UnkCurvesStruct *arg0) {
    if (arg0->unk0x90) {

    }
    if ((arg0->unk0x94 == &func_80004D70) || (arg0->unk0x94 == &func_80004C5C)) {

    }

    arg0->unk0x10 = 0;
    arg0->unk0xC = 0.0f;
    
    while (arg0->unk0x10 < (arg0->unk0x90 - 3)) {
        func_800065C0(arg0, 5);

        arg0->unk0xC += arg0->unk0x14;

        if ((arg0->unk0x94 == &func_80004D70) || (arg0->unk0x94 == &func_80004C5C)) {
            arg0->unk0x10 += 4;
        } else {
            arg0->unk0x10 += 1;
        }
    }

    if (arg0->unk0x80 != 0) {
        arg0->unk0x10 = arg0->unk0x90 - 4;
    } else {
        arg0->unk0x10 = 0;
    }

    func_800065C0(arg0, 0x14);
    
    if (arg0->unk0x80 != 0) {
        arg0->unk0x8 = arg0->unk0xC - arg0->unk0x4;
    } else {
        arg0->unk0x8 = arg0->unk0x4;
    }
}

void func_8000535C(UnkCurvesStruct *arg0) {
    if (arg0->unk0x80 != 0) {
        arg0->unk0x0 = 1.0f;
        arg0->unk0x4 = arg0->unk0x64;
        arg0->unk0x8 = arg0->unk0xC;
        arg0->unk0x10 = arg0->unk0x90 - 4;
    } else {
        arg0->unk0x0 = 0.0f;
        arg0->unk0x4 = 0.0f;
        arg0->unk0x8 = 0.0f;
        arg0->unk0x10 = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_800053B0.s")

void func_8000598C(f32*,f32*,f32*,f32*,f32*,f32*,s32,s32);
#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_8000598C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_80005E60.s")

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

    if (arg0->unk0x84 != 0) {
        phi_s4 = &arg0->unk0x84[arg0->unk0x10];
    }
    if (arg0->unk0x88 != 0) {
        phi_s5 = &arg0->unk0x88[arg0->unk0x10];
    }
    if (arg0->unk0x8C != 0) {
        phi_s6 = &arg0->unk0x8C[arg0->unk0x10];
    }

    if (arg0->unk0x98 != 0) {
        func_8000598C(phi_s4, phi_s5, phi_s6, xCoords, yCoords, zCoords, arg1, arg0->unk0x98);
    } else {
        
    }

    arg0->unk0x14 = 0.0f;

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
        arg0->unk0x18[i] = sqrtf((xDist * xDist) + (yDist * yDist) + (zDist * zDist));
        // Sum distances
        arg0->unk0x14 += arg0->unk0x18[i];
    }
}

static const f32 f32_80098508 = 0.000099999997f; // TODO: make literal in func_80006784
#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_80006784.s")

static const f32 f32_8009850c = 0.000099999997f; // TODO: make literal in func_80006908
#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_80006908.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_80006B28.s")
#else
typedef struct {
    /* 0x0  */ Vec4f unk0x0;
    /* 0x10 */ Vec4f unk0x10;
    /* 0x20 */ f32 unk0x20;
} UnkCurveStruct1;

typedef struct {
    /* 0x0 */ f32 unk0x0;
    /* 0x4 */ f32 unk0x4;
} UnkCurveStruct2;

void func_80006B28(UnkCurveStruct2 *arg0, s32 arg1, UnkCurveStruct1 *arg2) {
    Vec4f sp88;
    Vec4f sp78;
    Vec4f sp68;
    UnkCurveStruct2 *phi_v0;
    f32 *phi_a0;
    f32 *phi_v1;
    s32 i;
    s32 end;

    end = arg1 - 1;
    i = 0;
    if (end > 0) {
        phi_v0 = arg0[i - 1];
        phi_a0 = &sp88;
        phi_v1 = &sp78;

        do {
            if ((i - 1) < 0) {
                *phi_a0 = (phi_v0->unk8 - phi_v0->unk10) + phi_v0->unk8; // v[i].z - v[i+1].x + v[i].z
                *phi_v1 = (phi_v0->unkC - phi_v0->unk14) + phi_v0->unkC; // v[i].w - v[i+1].y + v[i].z
            } else if ((i - 1) >= arg1) {
                *phi_a0 = (phi_v0->unk-8 - phi_v0->unk-10) + phi_v0->unk-8; // v[i-1].z - v[i-1].x + v[i-1].z
                *phi_v1 = (phi_v0->unk-4 - phi_v0->unk-C) + phi_v0->unk-4;  // v[i-1].w - v[i-1].y + v[i-1].w
            } else {
                *phi_a0 = phi_v0->unk0; // v[i].x
                *phi_v1 = phi_v0->unk4; // v[i].y
            }
            phi_a0 += 4;
            phi_v1 = phi_v1 + 4;
            if (phi_v1 != &sp88) {
                continue;
            }
            
            func_80004B78(&sp88, &sp68);
            arg2[i].unk0x0.x = sp68.x;
            arg2[i].unk0x0.y = sp68.y;
            arg2[i].unk0x0.z = sp68.z;
            arg2[i].unk0x0.w = sp68.w;
            func_80004B78(&sp78, &sp68);
            arg2[i].unk0x10.x = sp68.x;
            arg2[i].unk0x10.y = sp68.y;
            arg2[i].unk0x10.z = sp68.z;
            arg2[i].unk0x10.w = sp68.w;
            arg2[i].unk0x20 = func_80004A60(&sp78, 0, 0);
            
            i = i + 1;
        } while (i != end);
    }
    arg2[0].unk0x20 = 0.0f;
    arg2[arg1 - 1].unk0x20 = 1.0f;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_80006CFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_80006E04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/curves/func_80006E58.s")
