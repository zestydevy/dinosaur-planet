// curves.c?

#include "common.h"

extern f32 D_800984FC; // 1.0f / 60.0f
extern f32 D_80098500; // 1.0f / 60.0f
extern f32 D_80098504; // 1.0f / 60.0f

void func_80004CE8(Vec4f *in, Vec4f *out);
void func_80004DFC(Vec4f *in, Vec4f *out);

// // // bezier?

f32 func_80004A60(Vec4f *a0, f32 a1, f32 *a2) {
    Vec4f v;
    
    v.x = a0->w + (((3.0f * a0->y) - a0->x) + (-3.0f * a0->z));
    v.y = (3.0f * a0->z) + ((3.0f * a0->x) + (-6.0f * a0->y));
    v.z = (3.0f * a0->z) + (-3.0f * a0->x);
    v.w = a0->z + (a0->x + (4.0f * a0->y));

    v = v; // from permuter
    
    if (a2 != NULL) {
        *a2 = ((v.z * 1.0f) + ((((3.0f * v.x) * a1) + (v.y + v.y)) * a1)) * D_800984FC;
    }

    return ((v.w * 1.0f) + ((((v.x * a1) + v.y) * a1) + v.z) * a1) * D_80098500;
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80004B78.s")
#else
// Functionally equivalent
void func_80004B78(Vec4f *a0, Vec4f *a1) {
    f32 temp = D_80098504;

    a1->x = a0->w + (((3.0f * a0->y) - a0->x) + (-3.0f * a0->z));
    a1->y = (3.0f * a0->z) + ((3.0f * a0->x) + (-6.0f * a0->y));
    a1->z = (3.0f * a0->z) + (-3.0f * a0->x);
    a1->w = a0->z + (a0->x + (4.0f * a0->y));
    
    a1->x *= temp;
    a1->y *= temp;
    a1->z *= temp;
    a1->w *= temp;
}
#endif

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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80005094.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_8000523C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_8000535C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_800053B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_8000598C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80005E60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_800065C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80006784.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80006908.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80006B28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80006CFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80006E04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_5660/func_80006E58.s")
