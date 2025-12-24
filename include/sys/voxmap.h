#ifndef _SYS_VOXMAP_H
#define _SYS_VOXMAP_H

#include "sys/math.h"
#include "unktypes.h"

typedef struct {
    u8 pad[0xAF0];
} SomeVoxmapAllocStructUnk0;
typedef struct {
    u8 pad[0x320];
}  SomeVoxmapAllocStructUnk4;
typedef struct {
    u8 pad[0x78];
}  SomeVoxmapAllocStructUnk8;
typedef struct {
    SomeVoxmapAllocStructUnk0 *unk0;
    SomeVoxmapAllocStructUnk4 *unk4;
    SomeVoxmapAllocStructUnk8 *unk8;
} SomeVoxmapAllocStruct;

// size: 0xE
typedef struct {
    Vec3s16 unk0;
    u8 pad6[0xA-0x6];
    u8 unkA;
    u8 unkB;
    UNK_TYPE_8 unkC;
    u8 padD;
} Unk80008E40Unk0;

typedef struct {
    Unk80008E40Unk0 *unk0;
    UNK_TYPE_32 unk4;
    Vec3f *unk8;
    Vec3s16 unkC;
    Vec3s16 unk12;
    UNK_TYPE_32 unk18;
    s16 unk1C;
    UNK_TYPE_16 unk1E;
    s16 unk20;
    s16 unk22;
    UNK_TYPE_16 unk24;
} Unk80008E40;

typedef struct {
    Vec3f unk0;
    Vec3f unkC;
    Vec3f unk18;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
} Unk80009024;

void func_80007178(void);

u8 *func_80007468(s32, s32, s32, s32);
u8 *func_80007620(s32, s32);

void func_80007E2C(Vec3f*, Vec3s16*);
void func_80007EE0(Vec3f*, Vec3s16*);
s32 func_80008048(Vec3s16 *arg0, Vec3s16 *arg1, Vec3s16 *arg2, u8* arg3, u8 arg4);

s32 func_800099D0(Vec3s16 *, Vec3s16 *, Vec3s16*);
void func_8000A650(Unk80008E40*);
void func_8000A80C(s32, UNK_TYPE_16*, u16, u16);
u16* func_8000A9AC(Unk80008E40*, Vec3s16*, UNK_TYPE_32, UNK_TYPE_32);
void func_80009F5C(Unk80008E40*, Unk80008E40Unk0*, s32);
s32 func_8000A934(s32, UNK_TYPE_16 *);
s32 func_80008E40(Unk80008E40*, Vec3f*, Vec3f*);
s32 func_80008F34(Unk80008E40*, s32);
s32 func_8000921C(Unk80008E40*, s32);

void func_80008DC0(SomeVoxmapAllocStruct*); // init/alloc struct
void func_80008E08(SomeVoxmapAllocStruct*); // free struct

#endif
