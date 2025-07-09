#ifndef _DLLS_24_H
#define _DLLS_24_H

#include "PR/ultratypes.h"
#include "sys/gfx/map.h"
#include "sys/gfx/texture.h"
#include "sys/memory.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "sys/main.h"
#include "prevent_bss_reordering.h"
#include "functions.h"

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

typedef struct Dll24State {
/*0x00*/ StructBss8 *_bss_0; // 120 items
/*0x04*/ DLTri *_bss_4;
/*0x08*/ StructBss8 *_bss_8; // 10? 150? items
/*0x0C*/ DLTri *_bss_C;
/*0x10*/ StructBss8 *_bss_10; // 120 items
/*0x14*/ DLTri *_bss_14;
/*0x18*/ StructBss8 *_bss_18; // 120 items
/*0x1C*/ DLTri *_bss_1C;
/*0x20*/ s32 _bss_20; // some sort of counter
/*0x24*/ StructBss24 *_bss_24; // 30 items
/*0x28*/ s32 _bss_28; // counter or state?
/*0x2C*/ StructBss2C *_bss_2C; // 10 items
/*0x30*/ s32 _bss_30; // counter or state?
/*0x34*/ StructBss34 *_bss_34; // 30 items
/*0x38*/ s32 _bss_38; // some sort of counter
/*0x3C*/ StructBss3C *_bss_3C; // 30 items
/*0x40*/ Texture *_bss_40;
/*0x44*/ Texture *_bss_44;
/*0x48*/ Texture *_bss_48;
/*0x4C*/ Texture *_bss_4C;
/*0x50*/ f32 _bss_50;
} Dll24State;
void dll_24_ctor(s32 arg0);
void dll_24_dtor(s32 arg0);
void dll_24_func_24C(void);
void dll_24_func_564(Func564Arg0 *arg0, u16 arg1, Vec3f *arg2, Func564Arg3 *arg3, f32 arg4);
void dll_24_func_6E8(void);
void dll_24_func_174C(f32, f32, f32, f32);
void dll_24_func_1B28(f32, f32, f32, s16, f32);
void dll_24_func_1CC8(f32, f32, f32, s16, f32, s32);

#endif //_DLLS_24_H