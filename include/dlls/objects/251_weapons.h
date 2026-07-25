#ifndef _DLLS_251_H
#define _DLLS_251_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_251_Weapons) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ void (*func7)(Object* self, f32 arg1);
    /*08*/ void (*func8)(Object* self);
    /*09*/ UnknownDLLFunc func9;
    /*10*/ UnknownDLLFunc func10;
    /*11*/ void (*func11)(Object* self);
    /*12*/ void (*func12)(Object* self, s32 arg1);
    /*13*/ void (*func13)(Object* self, s32 arg1);
    /*14*/ void (*func14)(Object* self, s32 arg1);
    /*15*/ void (*func15)(Object* self, s32 arg1);
    /*16*/ s16 (*func16)(Object* self);
    /*17*/ void (*func17)(Object* self, Vec3f* arg1, Vec3f* arg2);
    /*18*/ void (*func18)(Object* self, f32 arg1, f32 arg2);
    /*19*/ s8 (*func19)(Object* self);
};

#endif // _DLLS_251_H
