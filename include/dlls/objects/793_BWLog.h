#ifndef _DLLS_793_H
#define _DLLS_793_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 _unk19[0x24-0x19];
} BWLog_Setup;

// This might share an interface with DLL 417 or 712
DLL_INTERFACE(DLL_793_BWLog) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*func7)(Object *self, Object *a1);
    /*8*/ s32 (*func8)(Object *self);
    /*9*/ void (*func9)(Object *self, f32 *a1, f32 *a2, f32 *a3);
    /*10*/ s32 (*func10)(Object *self, Object *a1);
    /*11*/ UNK_TYPE_32 (*func11)(Object *self);
    /*12*/ void (*func12)(Object *self, f32 *a1, f32 *a2, f32 *a3);
    /*13*/ UNK_TYPE_8 (*func13)(Object *self);
    /*14*/ void (*func14)(Object *self, s32 a1);
    /*15*/ void (*func15)(Object *self, f32 *a1, UNK_TYPE_32 *a2);
    /*16*/ f32 (*func16)(UNK_TYPE_32 a0, f32 *a1);
    /*17*/ s32 (*func17)(UNK_TYPE_32 a0);
    /*18*/ void (*func18)(UNK_TYPE_32 a0);
    /*19*/ void (*func19)(UNK_TYPE_32 a0, UNK_TYPE_32 a1);
    /*20*/ void (*func20)(UNK_TYPE_32 a0, UNK_TYPE_32 a1, UNK_TYPE_32 a2);
};

#endif // _DLLS_793_H
