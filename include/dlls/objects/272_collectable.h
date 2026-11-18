#ifndef _DLLS_272_H
#define _DLLS_272_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_272_Collectable) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*IsCollected)(Object* self);
    /*8*/ void (*SetPauseState)(Object* self, s32 state);
    /*9*/ s32 (*GetAreaValue)(Object* self);
    /*10*/ void (*SetVelocity)(Object* self, f32 Vx, f32 Vy, f32 Vz);
    /*11*/ void (*SetVisibility)(Object* self, s32 visibility);
    /*12*/ u8 (*GetVisibility)(Object* self);
    /*13*/ void (*SavePosition)(Object* self, f32 x, f32 y, f32 z);
};

#define dll_collectable(obj) (((DLL_272_Collectable*)obj->dll)->vtbl)

#endif // _DLLS_272_H
