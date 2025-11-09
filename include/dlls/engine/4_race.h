#ifndef _DLL_4_H
#define _DLL_4_H

#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

// Note: This is a race checkpoint-specific version of the base ObjSetup struct, with additions
typedef struct {
    s16 objId;
    u8 quarterSize;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    Vec3f pos;
    s32 uID;
    s32 unk18[2]; // prev checkpoints
    s32 unk20[2]; // next checkpoints
    s8 unk28;
    u8 unk29;
    u8 unk2A;
    s8 unk2B[2];
    s8 unk2D[4];
    s8 unk31[4];
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 unk3F;
} RaceCheckpointSetup;

typedef struct {
    u32 uID;
    RaceCheckpointSetup *setup;
} RaceCheckpoint;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8; // 0-1 progress to ? checkpoint
    f32 unkC; // 0-1 progress to next checkpoint
    s32 unk10; // target checkpoint uID? only used by AI?
    s32 unk14; // target checkpoint uID? only used by AI?
    s32 unk18; // last checkpoint uID hit
    s32 unk1C; // checkpoint index
    s16 unk20;
} RaceStruct;

DLL_INTERFACE(DLL_4_race) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ void (*func1)(RaceCheckpointSetup* arg0);
/*2*/ void (*func2)(RaceCheckpointSetup* arg0);
/*3*/ void (*func3)(Object* arg0, RaceStruct* arg1, s32 arg2);
/*4*/ s32 (*func4)(Object* arg0, RaceStruct* arg1);
/*5*/ s32 (*func5)(SRT* arg0, RaceStruct* arg1, f32 arg2, s32 arg3, u8 arg4, u8 arg5);
/*6*/ s32 (*func6)(UNK_TYPE_32 a0, UNK_TYPE_32 a1, UNK_TYPE_32 a2, UNK_TYPE_32 a3);
/*7*/ void (*func7)(s32 arg0, RaceStruct* arg1, s8* arg2);
/*8*/ RaceCheckpointSetup *(*func8)(s32 checkpointUID, s32 *arg1);
/*9*/ void (*func9)(RaceStruct* arg0);
/*10*/ void (*func10)(RaceStruct* arg0);
/*11*/ RaceStruct *(*func11)(s32* arg0);
/*12*/ s32 (*func12)(RaceStruct* arg0);
/*12*/ RaceStruct *(*func13)(s32 arg0);
/*13*/ void (*func14)(void);
};

#endif // _DLL_4_H
