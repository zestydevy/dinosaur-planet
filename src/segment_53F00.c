#include "common.h"

static const char str_8009aa70[] = "Sorry Background Block list has been exceeded\n";
static const char str_8009aaa0[] = "1: track/intersect.c: OVERFLOW error\n";
static const char str_8009aac8[] = "2: track/intersect.c: OVERFLOW error\n";
static const char str_8009aaf0[] = "3: track/intersect.c: OVERFLOW error\n";
static const char str_8009ab18[] = "TrackGetHeight()-Overflow!!!\n";
static const char str_8009ab38[] = "trackIntersect: FUNC OVERFLOW %d\n";
static const char str_8009ab5c[] = "intersectModLineBuild point list overflow, %d/%d\n";
static const char str_8009ab90[] = "trackIntersect: FUNC OVERFLOW %d\n";
static const char str_8009abb4[] = "insertPoint array overrun %d/%d\n";
static const char str_8009abd8[] = "NO FREE LAST LINE\n";

s32 func_80055458(Object*, s32, s32, f32*, f32 *, s32, s8*, s32);
s32 func_800564C8(s32, s32, f32 *, s32, s8*, s32);
extern s32 D_80092E70;
typedef struct Unk800BB268 {
    Object *unk0;
    s16 unk4;
    MtxF *unk8;
    MtxF *unkC;
} Unk800BB268;
extern Unk800BB268 D_800BB268[];
extern u8 D_800BB3A8;

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80053300.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800533D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80053408.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005341C.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/fit_aabb_around_cubes.s")
#else
// close
void _fit_aabb_around_cubes(AABBs32 *aabb, Vec3f *posArray1, Vec3f *posArray2, float *cubeRadiusArray, int arrayLength) {
    aabb->min.x = 1000000;
    aabb->max.x = -1000000;
    aabb->min.y = 1000000;
    aabb->max.y = -1000000;
    aabb->min.z = 1000000;
    aabb->max.z = -1000000;

    while (arrayLength-- != 0) {
        float left;
        float right;
        float bottom;
        float top;
        float back;
        float front;

        // vec1
        left = posArray1->x - *cubeRadiusArray;
        if (left < aabb->min.x) {
            aabb->min.x = left;
        }

        right = posArray1->x + *cubeRadiusArray;
        if (right > aabb->max.x) {
            aabb->max.x = right;
        }

        bottom = posArray1->y - *cubeRadiusArray;
        if (bottom < aabb->min.y) {
            aabb->min.y = bottom;
        }

        top = posArray1->y + *cubeRadiusArray;
        if (top > aabb->max.y) {
            aabb->max.y = top;
        }

        back = posArray1->z - *cubeRadiusArray;
        if (back < aabb->min.z) {
            aabb->min.z = back;
        }

        front = posArray1->z + *cubeRadiusArray;
        if (front > aabb->max.z) {
            aabb->max.z = front;
        }

        // vec2
        left = posArray2->x - *cubeRadiusArray;
        if (left < aabb->min.x) {
            aabb->min.x = left;
        }

        right = posArray2->x + *cubeRadiusArray;
        if (right > aabb->max.x) {
            aabb->max.x = right;
        }

        bottom = posArray2->y - *cubeRadiusArray;
        if (bottom < aabb->min.y) {
            aabb->min.y = bottom;
        }

        top = posArray2->y + *cubeRadiusArray;
        if (top > aabb->max.y) {
            aabb->max.y = top;
        }

        back = posArray2->z - *cubeRadiusArray;
        if (back < aabb->min.z) {
            aabb->min.z = back;
        }

        front = posArray2->z + *cubeRadiusArray;
        if (front > aabb->max.z) {
            aabb->max.z = front;
        }

        posArray1++;
        posArray2++;
        
        cubeRadiusArray++;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80053750.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80053B24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005471C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80054DF8.s")

u8 func_8005509C(Object *arg0, f32* arg1, f32* arg2, s32 arg3, Unk80027934* arg4, u8 arg5) {
    Unk800BB268* var_s3;
    Unk800BB268* target;
    f32 spA0[4 * 3];
    f32 sp70[4 * 3];
    u8 sp6F;
    s16 var_s1;
    s16 var_s2;
    u8 temp_v0_3;
    u8 var_s5;
    Object* sp64;

    sp6F = 0;
    arg4->unk68 = 0;
    var_s3 = D_800BB268;
    target = &D_800BB268[D_800BB3A8];
    if ((u32) target >= (u32) ((u8*)&D_800BB268 + 1)) {
        do {
            sp64 = var_s3->unk0;
            if (sp64 != NULL) {
                for (var_s1 = 0, var_s2 = 0; var_s2 < arg3;) {
                    vec3_transform(
                        var_s3->unk8,
                        arg1[var_s1 + 0], arg1[var_s1 + 1], arg1[var_s1 + 2],
                        &sp70[var_s1 + 0], &sp70[var_s1 + 1], &sp70[var_s1 + 2]
                    );
                    vec3_transform(
                        var_s3->unk8,
                        arg2[var_s1 + 0], arg2[var_s1 + 1], arg2[var_s1 + 2],
                        &spA0[var_s1 + 0], &spA0[var_s1 + 1], &spA0[var_s1 + 2]
                    );
                    var_s1 += 3;\
                    var_s2++;
                }
                if (arg5 & 2) {
                    sp6F |= func_800564C8((var_s3->unk4 * 0x34) + D_80092E70, (var_s3[1].unk4 * 0x34) + D_80092E70, &spA0[0], arg3, (s8* ) arg4, 1);
                } else {
                    temp_v0_3 = func_80055458(sp64, (var_s3->unk4 * 0x34) + D_80092E70, (var_s3[1].unk4 * 0x34) + D_80092E70, &sp70[0], &spA0[0], arg3, (s8* ) arg4, 1);
                    if (temp_v0_3) {
                        func_80026184(sp64, arg0);
                        sp6F |= temp_v0_3;
                    }
                }
                for (var_s1 = 0, var_s2 = 0, var_s5 = 1; var_s2 < arg3; var_s2++) {
                    if (sp6F & var_s5) {
                        vec3_transform(
                            var_s3->unkC,
                            spA0[var_s1 + 0], spA0[var_s1 + 1], spA0[var_s1 + 2],
                            &arg2[var_s1 + 0], &arg2[var_s1 + 1], &arg2[var_s1 + 2]
                        );
                    }
                    var_s5 <<= 1;
                    var_s1 += 3;
                }
            } else if (arg5 & 2) {
                sp6F |= func_800564C8((var_s3->unk4 * 0x34) + D_80092E70, (var_s3[1].unk4 * 0x34) + D_80092E70, arg2, arg3, (s8* ) arg4, 0);
            } else {
                sp6F |= func_80055458(sp64, (var_s3->unk4 * 0x34) + D_80092E70, (var_s3[1].unk4 * 0x34) + D_80092E70, arg1, arg2, arg3, (s8* ) arg4, 0);
            }
            var_s3 += 1;
        } while ((u32) var_s3 < (u32) target);
    }
    return sp6F;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80055458.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800564C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800567F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80056BCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80056E50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800573D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80057A30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80057F1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058144.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058680.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800588D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058B1C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058D54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058F3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058F50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058F7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058F8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80058FE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80059038.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800591EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_800596BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_80059C40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005A2BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005A3F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005B17C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005B204.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005B274.s")

void func_8005B5B8(Object*, Object*, s32);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005B5B8.s")
