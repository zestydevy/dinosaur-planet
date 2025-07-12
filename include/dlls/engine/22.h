#ifndef _DLLS_22_H
#define _DLLS_22_H

#include "PR/ultratypes.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/gfx/gx.h"
#include "sys/rand.h"
#include "dll.h"
#include "functions.h"
#include "prevent_bss_reordering.h"
#include "dll_def.h"

typedef struct StructBss10 {
    Texture *unk0;
    s32 pad4;
    s32 pad8;
    Texture *unkC;
    u8 pad10[0x28 - 0x10];
} StructBss10;

typedef struct StructBss38 {
    u8 *unk0;
    u8 pad4[0x20 - 0x4];
    s32 unk20;
    u8 pad24[0xC8 - 0x24];
    Texture *unkC8[8 * 6];
    Texture *unk188[8 * 6];
    u8 pad248[0x268 - 0x248];
    u16 unk268;
    u16 unk26C;
} StructBss38;

typedef struct StructData1C {
    s32 unk0;
    s16 unk4;
} StructData1C;

DLL_INTERFACE_BEGIN(22)
/* 0*/ u32 (*dll_22_func_2D0)(u32 arg0);
DLL_INTERFACE_END()

void dll_22_func_448(void);
f32 dll_22_func_16A0(void);
void dll_22_func_8F4();

// TODO: Find a better place for StructBss10 (and func_80037F9C)
void func_80037F9C(Gfx**, StructBss10*, s32, s32, s32, s32, s32, s32);

#endif //_DLLS_22_H
