#ifndef _DLLS_29_H
#define _DLLS_29_H

#include "PR/ultratypes.h"
#include "sys/math.h"
#include "dll_def.h"
#include "unktypes.h"

#define CHARACTER_SABRE 0
#define CHARACTER_KRYSTAL 1

// size: 0x10
typedef struct {
    /*0x0*/ Vec3f vec;
    /*0xC*/ s8 unk0xC;
    /*0xD*/ s8 unk0xD;
    /*0xE*/ u8 unk0xE[2];
} GplayStruct5;

// size: 0x14
typedef struct {
    s16 unk0x0;
    s16 unk0x2;
    s16 unk0x4;
    s16 unk0x6;
    s16 unk0x8;
    s16 unk0xa;
    s16 unk0xc;
    s16 unk0xe;
    s16 unk0x10;
    s16 unk0x12;
} GplayStruct6;

// size: 0xB4
typedef struct {
    u8 unk0x0[0xB4];
} GplayStruct14;

// size: 0x8
typedef struct {
    s32 unk0x0;
    f32 unk0x4;
} GplayStruct17;

// size: 0x3
typedef struct {
    u8 unk0x0;
    u8 unk0x1;
    u8 _unk0x2[1];
} GplayStruct11;

// size: 0xc
typedef struct {
    /*0x0*/u8 unk0x0;
    /*0x1*/u8 hp;
    /*0x2*/u8 hpMax; //only shows up to 104, code caps it at 80.
    /*0x3*/u8 _unk0x3[1];
    /*0x4*/s16 mana; //only mods when byte at "state"0x8bb is set.
    /*0x6*/s16 manaMax; //capped at 100.
    /*0x8*/s16 scarabs; //capped at 999.
    /*0xA*/UNK_TYPE_8 unk0xA;
    /*0xB*/s8 dusters;
} GplayStruct10;

// size: 0x13d4
typedef struct {
    GplayStruct10 unk0x0[2];
    GplayStruct11 unk0x18[2];
    u8 _unk0x1E[0x2];
    GplayStruct14 unk0x20[2];
    GplayStruct14 unk0x188[2];
    u8 unk0x2F0[6];
    u8 unk0x2f6;
    /*0x2f7*/u8 character;
    u8 unk0x2f8;
    f32 unk0x2fc;
    s16 unk0x300;
    s16 unk0x302;
    u8 unk0x304;
    u8 _unk0x305[0x7CF];
    GplayStruct17 unk0xad4[256]; // unconfirmed length, but probably
    /*0x12D4*/u8 bitString[256];
} GplayStruct8;

// size: 0x15d4
typedef struct {
    GplayStruct8 unk0;
    /*0x13d4*/u8 bitString[512];
} GplayStruct9;

// size: 0x40
typedef struct {
    f32 unk0x0;
    s16 unk0x4;
    s16 unk0x6;
    s16 unk0x8;
    s16 unk0xa;
    s16 unk0xc;
    s16 unk0xe;
    u8 _unk0x10[0x2c];
    u8 unk0x3c;
    s8 unk0x3d;
    s8 unk0x3e;
    s8 unk0x3f;
} GplayStruct12;

// size: 0x8
typedef struct {
    s16 unk0x0;
    s16 unk0x2;
    s16 unk0x4;
    s16 unk0x6;
} GplayStruct13;

// size: 0x17ac
typedef struct {
    GplayStruct9 unk0;
    /*0x15d4*/u8 bitString[256];
    GplayStruct5 unk0x16d4[2];
    GplayStruct6 unk0x16F4[2];
    GplayStruct12 unk0x171C[2];
    GplayStruct13 unk0x179c[2];
} GplayStruct3;

// size: 0x80
typedef struct {
    u8 unk0x0;
    u8 unk0x1;
    u8 _unk0x2[6];
    u8 unk0x8;
    u8 unk0x9;
    u8 unk0xa;
    s8 unk0xb;
    s8 unk0xc;
    u32 unk0x10;
    u32 unk0x14;
    u32 unk0x18;
    u8 _unk0x1C[0x64];
} GplayStruct4;

// size: 0x182c
typedef struct {
    GplayStruct3 unk0;
    /*0x17a*/u8 bitString[128];
} GplayStruct7;

DLL_INTERFACE_BEGIN(29_gplay)
    /*0*/ void (*func_110)(s8 param1);
    /*1*/ void (*func_198)(s8 param1, u8 *param2);
    /*2*/ s32 (*func_3E4)(s8 param1, u8 param2);
    /*3*/ void (*func_638)(s8 param1, s8 param2);
    /*4*/ void (*func_6AC)();
    /*5*/ void (*func_94C)(s32 param1);
    /*6*/ void (*func_958)(Vec3f *param1, s16 param2, s32 param3, s32 param4);
    /*7*/ void (*func_AE0)();
    /*8*/ void (*func_B3C)(Vec3f *param1, s16 param2, s32 param3);
    /*9*/ void (*func_CBC)();
    /*10*/ void (*func_D20)();
    /*11*/ s32 (*func_D70)();
    /*12*/ void (*func_8D8)();
    /*13*/ u32 (*func_79C)();
    /*14*/ GplayStruct4 *(*func_930)();
    /*15*/ u8 (*func_143C)(s32 param1);
    /*16*/ void (*func_139C)(s32 param1, s32 param2);
    /*17*/ void (*func_1378)(s32 param1, s32 param2);
    /*18*/ u8 (*func_14F0)(s32 param1, s32 param2);
    /*19*/ void (*func_16C4)(s32 param1, s32 param2, s32 param3);
    /*20*/ u16 (*func_1590)(s32 param1);
    /*21*/ void (*func_15B8)(s32 param1);
    /*22*/ u32 (*func_163C)(s32 param1);
    /*23*/ void (*func_1680)(s32 param1);
    /*24*/ void (*func_1014)(u32 param1, f32 param2);
    /*25*/ s32 (*func_109C)(u32 param1);
    /*26*/ f32 (*func_10F4)(u32 param1);
    /*27*/ void (*func_115C)();
    /*28*/ u8 (*func_E90)();
    /*29*/ void (*func_EAC)(u8 character);
    /*30*/ void *(*func_1254)();
    /*31*/ s16 (*func_121C)();
    /*32*/ void (*func_1238)(s32 param1);
    /*33*/ GplayStruct7 *(*func_E74)();
    /*34*/ GplayStruct10 *(*func_ED4)();
    /*35*/ GplayStruct5 *(*func_F04)();
    /*36*/ GplayStruct11 *(*func_F30)();
    /*37*/ GplayStruct6 *(*func_F60)();
    /*38*/ GplayStruct12 *(*func_FA8)();
    /*39*/ GplayStruct13 *(*func_FE8)();
    /*40*/ GplayStruct14 *(*func_1974)();
    /*41*/ GplayStruct14 *(*func_19B8)();
    /*42*/ u32 (*func_1270)();
    /*43*/ u32 (*func_19FC)(u8 param1);
    /*44*/ void (*func_1A48)(u8 param1);
    /*45*/ s32 (*func_1A90)(u8 param1);
    /*46*/ void (*func_1AF8)(u8 param1, u8 param2);
    /*47*/ u32 (*func_1B78)(u8 param1);
    /*48*/ void (*func_1BC4)(u8 param1);
DLL_INTERFACE_END()

#endif //_DLLS_29_H
