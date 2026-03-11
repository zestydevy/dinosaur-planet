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
void func_80053300(void);
void func_800591EC();
void func_8005B5B8(Object*, Object*, s32);
void func_80059038(s32 animatorID, Object* parentObject, s32 enableLines);
s32 func_80057F1C(Object* arg0, f32 arg1, f32 arg2, f32 arg3, Func_80057F1C_Struct*** arg4, s32 arg5, s32 arg6);
s32 func_80059C40(Vec3f* arg0, Vec3f* arg1, f32 arg2, s32 arg3, Func_80059C40_Struct* arg4, Object* arg5, s8 arg6, s8 arg7, u8 arg8, s8 arg9);
s32 func_80058680(Object* arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4, u8 arg5);
s32 func_80056BCC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3, Vec3f* arg4, f32 arg5);
s32 func_80058B1C(Object* arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4, u8 arg5);

#endif
