#ifndef _DLLS_24_H
#define _DLLS_24_H

#include "PR/ultratypes.h"
#include "sys/gfx/texture.h"
#include "sys/memory.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "prevent_bss_reordering.h"
#include "functions.h"

// size: 0x20
typedef struct StructBss4
{
    u8 pad0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    s32 pad4;
    s32 pad8;
    s32 padC;
    u8 pad10;
    s8 unk11;
    s8 unk12;
    s8 unk13;
    s32 pad14;
    s32 pad18;
    s32 pad1C;
} StructBss4;

// size: 0x10
typedef struct StructBss8
{
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
} StructBss8;

// size: 0x10
typedef struct StructBssC
{
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    s32 pad4;
    s32 pad8;
    s32 padC;
} StructBssC;

// size 0x1A (0x1C?)
typedef struct StructBss24
{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
} StructBss24;

// size: 0x5C
typedef struct StructBss2C
{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8 pad10[0x24 - 0x10];
    f32 unk24;
    u8 pad28[0x3C - 0x28];
    f32 unk3C;
    u8 pad40[0x54 - 0x40];
    s16 unk54;
    s16 pad56;
    s8 unk58;
} StructBss2C;

// size: 0x1C
typedef struct StructBss34
{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s8 unk18;
} StructBss34;

// size: 0x14
typedef struct StructBss3C
{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    s8 unk12;
} StructBss3C;

typedef struct Func564Arg0 {
    s16 unk0;
    s16 pad2;
    s32 pad4;
    s32 pad8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} Func564Arg0;

typedef struct Func564Arg3 {
    u8 pad0[0x1B0];
    f32 unk1B0;
} Func564Arg3;

void dll_24_ctor(s32 arg0);
void dll_24_dtor(s32 arg0);
void dll_24_func_24C(void);
void dll_24_func_564(Func564Arg0 *arg0, u16 arg1, Vec3f *arg2, Func564Arg3 *arg3, f32 arg4);
void dll_24_func_6E8(void);
void dll_24_func_174C(f32, f32, f32, f32);
void dll_24_func_1B28(f32, f32, f32, s16, f32);
void dll_24_func_1CC8(f32, f32, f32, s16, f32, s32);

#endif //_DLLS_24_H