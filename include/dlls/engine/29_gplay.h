#ifndef _DLLS_29_H
#define _DLLS_29_H

#include <PR/ultratypes.h>
#include "dll_def.h"

// size: 0x10
typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 unk0xC;
    u8 unk0xD;
    u8 unk0xE[2];
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
    u8 unk0x0;
    u8 unk0x1;
    u8 unk0x2;
    u8 _unk0x3[1];
    s16 unk0x4;
    s16 unk0x6;
    u8 _unk0x8[4];
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
    u8 _unk0x12D4[0x100];
} GplayStruct8;

// size: 0x15d4
typedef struct {
    GplayStruct8 unk0;
    u8 unk0x13d4[0x200];
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
    u8 unk0x15d4[0x100];
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
    GplayStruct4 unk0x17ac;
} GplayStruct7;

// DLL_INTERFACE_BEGIN(29_gplay)
    
// DLL_INTERFACE_END()

typedef struct DLLInst_Unknown DLLInst_29_gplay;

#endif //_DLLS_29_H
