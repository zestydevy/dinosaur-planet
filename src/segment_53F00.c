#include "common.h"

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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005509C.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_53F00/func_8005B5B8.s")
