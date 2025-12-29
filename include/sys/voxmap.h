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

typedef struct {
    s32 pad0;
    s32 unk4;
    s32 pad8;
    s32 unkC;
    s32 pad10;
    u8 *unk14;
    s32 pad18;
    u8 *unk1C;
    s32 pad20;
    u8 *unk24;
} VoxmapSlotData;

typedef struct {
    s32 unk0; // worldX?
    s32 unk4; // worldZ?
    s32 unk8; // scaledX?
    s32 unkC; // scaledZ?
    VoxmapSlotData *data;
} VoxmapSlot;

// size: 0xE
typedef struct {
    Vec3s16 unk0;
    u16 unk6;
    u16 unk8;
    u8 unkA;
    u8 unkB;
    UNK_TYPE_8 unkC;
    u8 padD;
} Unk80008E40Unk0;

typedef struct {
    u16 unk0;
    u16 unk2;
} Unk80008E40Unk4;


typedef struct {
    Unk80008E40Unk0 *unk0;
    Unk80008E40Unk4 *unk4;
    Vec3f *unk8;
    Vec3s16 unkC;
    Vec3s16 unk12;
    UNK_TYPE_32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
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

void voxmap_update_cache_timers(void);

u8 *voxmap_load_slot(s32, s32, s32, s32);
u8 *func_80007620(s32, s32);

void func_80007E2C(Vec3f*, Vec3s16*);
void func_80007EE0(Vec3f*, Vec3s16*);
s32 func_80008048(Vec3s16 *, Vec3s16 *, Vec3s16 *, u8*, u8);

s32 func_800099D0(Vec3s16 *, Vec3s16 *, Vec3s16*);
void func_8000A650(Unk80008E40*);
void func_8000A80C(Unk80008E40Unk4 *, s16*, u16, u16);
Unk80008E40Unk0* func_8000A9AC(Unk80008E40*, Vec3s16*, u16, u16);
void func_80009F5C(Unk80008E40*, Unk80008E40Unk0*, s32);
s32 func_8000A934(Unk80008E40Unk4 *, s16* );
s32 func_80008E40(Unk80008E40*, Vec3f*, Vec3f*);
s32 func_80008F34(Unk80008E40*, s32);
s32 func_8000921C(Unk80008E40*, s32);
void func_8000A078(Unk80008E40*, Unk80008E40Unk0*, s32, u16, Vec3s16*);
s32 func_8000A640(Vec3s16*);
void func_8000A884(Unk80008E40Unk4* arg0, s32 arg1, u16 arg2, u16 arg3);
s32 func_8000AB18(Unk80008E40*, s32, s32, s32*);

VoxmapSlot* voxmap_find_closest_object_slot(Vec3s16 *position);

void func_80008DC0(SomeVoxmapAllocStruct*); // init/alloc struct
void func_80008E08(SomeVoxmapAllocStruct*); // free struct

#endif
