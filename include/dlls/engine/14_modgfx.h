#ifndef _DLL_14_H
#define _DLL_14_H

#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"
#include "PR/gbi.h"

// size: 0x18
typedef struct {
    s32 unk0;
    Vec3f unk4;
    s16* unk10;
    s16 unk14;
    u8 unk16;
    u8 pad17;
} ModgfxStruct_0;

// size: 0x60
typedef struct {
    ModgfxStruct_0* unk0;
    Object* unk4;
    u8 pad8[0x20 - 0x8];
    f32 unk20;
    f32 unk24;
    f32 unk28;
    Vec3f unk2C;
    f32 unk38;
    s32 unk3C;
    s32 unk40;
    s16 unk44;
    s16 unk46[7];
    s32 unk54;
    u8 unk58;
    u8 unk59;
    u8 unk5A;
    u8 unk5B;
    s8 unk5C;
    s8 unk5D;
    u8 pad5E;
    u8 pad5F;
} ModgfxStruct;

DLL_INTERFACE(DLL_14_modgfx) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ s16 (*func1)(ModgfxStruct* arg0, s32 arg1, s32 arg2, s16* arg3, s32 arg4, s16* arg5, s32 arg6, Texture* arg7);
/*2*/ void (*func2)(s32 arg0, s32 arg1, s32 arg2);
/*3*/ void (*func3)(void);
/*4*/ void (*func4)(Object* arg0);
/*5*/ void (*func5)(Object* arg0);
/*6*/ s32 (*func6)(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, u8 arg3, Object* obj);
/*7*/ void (*func7)(s16* arg0);
/*8*/ void (*func8)(void);
/*9*/ void (*func9)(Object* arg0, u8 arg1);
/*10*/ void (*func10)(Object* arg0);
/*11*/ void (*func11)(s8* arg0);
/*12*/ void (*func12)(Object* arg0, u8 arg1, u8 arg2, s32 arg3, s32 arg4);
/*13*/ void (*func13)(void);
/*14*/ void (*func14)(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16* arg5);
/*15*/ void (*func15)(void);
/*16*/ void (*func16)(s16 arg0);
/*17*/ void (*func17)(s16 arg0);
/*18*/ void (*func18)(s16* arg0);
/*19*/ void (*func19)(Object* arg0, s16* arg1, s32 arg3, s16* arg4, s32 arg5, s32 arg6, Texture* arg7);
/*20*/ void (*func20)(s32 arg0);
/*21*/ s16 (*func21)(void);
};

#endif // _DLL_14_H
