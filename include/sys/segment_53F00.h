#ifndef _SYS_SEGMENT_53F00_H
#define _SYS_SEGMENT_53F00_H

#include "PR/ultratypes.h"
#include "sys/map.h"
#include "sys/math.h"

void func_80058F3C(void);
void fit_aabb_around_cubes(AABBs32 *aabb, Vec3f *posArray1, Vec3f *posArray2, f32 *cubeRadiusArray, s32 arrayLength);
void func_800533D8(s32*, UnkFunc80051D68Arg3**);
void func_80053408(Vec3s32 **);
void func_80053750(Object*, AABBs32*, u8);
Unk8005341C *func_8005341C(s32*);


#endif
