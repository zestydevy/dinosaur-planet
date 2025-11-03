#ifndef _DLLS_417_H
#define _DLLS_417_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_417_DFlog) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*func7)(Object *self, Object *a1);
    /*8*/ s32 (*func8)(Object *self);
    /*9*/ void (*func9)(Object *self, f32 *a1, f32 *a2, f32 *a3);
    /*10*/ s32 (*func10)(Object* arg0, Object* arg1);
    /*11*/ UNK_TYPE_32 (*func11)(Object *self);
    /*12*/ void (*func12)(Object *self, f32 *a1, f32 *a2, f32 *a3);
    /*13*/ UNK_TYPE_8 (*func13)(Object *self);
    /*14*/ void (*func14)(Object *self, s32 a1);
    /*15*/ void (*func15)(Object *self, f32 *a1, u32 *a2);
    /*16*/ f32 (*func16)(Object *self, f32 *a1);
    /*17*/ s32 (*func17)(UNK_TYPE_32 a0);
    /*18*/ void (*func18)(UNK_TYPE_32 a0);
    /*19*/ void (*func19)(UNK_TYPE_32 a0, UNK_TYPE_32 a1);
    /*20*/ void (*func20)(UNK_TYPE_32 a0, UNK_TYPE_32 a1, UNK_TYPE_32 a2);
};

#endif // _DLLS_417_H
