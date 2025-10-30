#ifndef _DLLS_13_EXPGFX_H__
#define _DLLS_13_EXPGFX_H__

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    Object *unk0;
    s32 unk4;
    s32 unk8;
    SRT transform;
    Vec3f unk24; // local offset?
    Vec3f unk30; // world position?
    f32 unk3C; // scale
    s16 unk40;
    s16 unk42; // textureID
    s32 unk44;
    s32 unk48;
    s32 unk4C[3];
    u16 unk58[3];
    s8 unk5E;
    u8 unk5F;
    u8 unk60;
    s8 unk61;
    s8 unk62;
} D74Stack;

DLL_INTERFACE(DLL_13_expgfx) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*func0)(void);
    /*1*/ s16 (*func1)(D74Stack *arg0, s32 arg1);
    /*2*/ void (*func2)(s32 arg0, u8 updateRate, s32 arg2, s32 arg3);
    /*3*/ UnknownDLLFunc func3;
    /*4*/ UnknownDLLFunc func4;
    /*5*/ void (*func5)(Object *arg0);
    /*6*/ void (*func6)(Object *arg0, Gfx **gdl, Mtx **mtxs, Vertex **vertices, s32 arg4, s32 arg5, s32 arg6);
    /*7*/ UnknownDLLFunc func7;
    /*8*/ UnknownDLLFunc func8;
    /*9*/ void (*func9)(Object *arg0);
    /*10*/ s8 (*func10)(Object *obj);
};

#endif //_DLLS_13_EXPGFX_H__
