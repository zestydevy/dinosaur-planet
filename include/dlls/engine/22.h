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

// Size: 0x28
typedef struct StructBss10 {
    Texture *unk0;
    s32 pad4;
    s16 unk8;
    s16 unkA;
    Texture *unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
    s32 unk18;
    u8 pad1C[0x28 - 0x1C];
} StructBss10;

// Size: 0x18
typedef struct InnerBss38 {
    Texture *unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 pad10;
    s32 pad14;
} InnerBss38;

// Size: 0x20
typedef struct Bss38Unk0 {
    s32 unk0[8];
}Bss38Unk0;

// Size: 0x10
typedef struct Bss38Unk80 {
    u16 unk0[8];
}Bss38Unk80;

// Size: 0x26C
typedef struct StructBss38 {
    Bss38Unk0 unk0[3];
    u8 pad60[0x7E - 0x60];
    u16 unk7E;
    Bss38Unk80 unk80[2];
    Bss38Unk80 unkA0[2];
    s16 unkC0[4];
    InnerBss38 unkC8[8];
    InnerBss38 unk188[8];
    u16 unk248;
    u8 pad24C[0x268 - 0x24A];
    s16 unk268;
    s16 pad26A;
} StructBss38;

typedef struct StructData1C {
    s32 unk0;
    s16 unk4;
} StructData1C;

DLL_INTERFACE_BEGIN(22)
/* 0 */ u32 (*dll_22_func_2D0)(u32 arg0);
/* 1 */ void (*dll_22_func_2F4)(void);
/* 2 */ void (*dll_22_func_32C)(u8 arg0, u8 arg1);
/* 3 */ void (*dll_22_func_368)(u16 arg0);
/* 4 */ void (*dll_22_func_448)(u16 arg0);
/* 5 */ void (*dll_22_func_4C0)(void);
/* 6 */ void (*dll_22_func_578)(Gfx **gdl);
/* 7 */ s16 (*dll_22_func_214C)(s32 arg0);
/* 8 */ s32 (*dll_22_func_21C0)(s32 arg0, s16 arg1);
/* 9 */ s32 (*dll_22_func_2248)(u8 arg0);
DLL_INTERFACE_END()

// TODO: Find a better place for StructBss10 (and func_80037F9C)
void func_80037F9C(Gfx**, StructBss10*, s32, s32, s32, s32, s32, s32);

#endif //_DLLS_22_H
