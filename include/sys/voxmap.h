#ifndef _SYS_VOXMAP_H
#define _SYS_VOXMAP_H

#include "sys/math.h"
#include "unktypes.h"

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
} SomeVoxmapAllocStruct;

void func_80007178(void);

u8 *func_80007468(s32, s32, s32, s32);
u8 *func_80007620(s32, s32);

void func_80007E2C(Vec3f*, Vec3s16*);
void func_80007EE0(Vec3f*, Vec3s16*);
s32 func_80008048(Vec3s16*, Vec3s16*, Vec3s16*, u8*, s32);

void func_80008DC0(SomeVoxmapAllocStruct*);
void func_80008E08(SomeVoxmapAllocStruct*);

#endif
