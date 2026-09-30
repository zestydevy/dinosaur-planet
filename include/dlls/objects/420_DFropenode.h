#ifndef _DLLS_420_H
#define _DLLS_420_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_420_DFRopeNode) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ void (*func7)(Object* self, Vec4f* spline); //Get rope spline
    /*08*/ void (*func8)(Object* self, f32 positionValue, f32* ox, f32* oy, f32* oz); //Get point on rope, using a position value (0.0 to 7.0)
    /*09*/ void (*func9)(Object* self, f32* arg1, f32 arg2);
    /*10*/ s16 (*func10)(Object* self, f32 arg1, f32 arg2);
    /*11*/ s32 (*func11)(Object* self, f32 x, f32 y, f32 z, f32* arg4, f32* arg5, s8* arg6); //TODO: verify
    /*12*/ s16 (*func12)(Object* self); //GetRopeYaw (angle between rope's ends, viewed from above)
    /*13*/ void (*func13)(Object* self, u32 arg1); //Set connection state?
    /*14*/ s16 (*func14)(Object* self); //Check if disconnected?
    /*15*/ void (*func15)(Object* self, f32 arg1);
    /*16*/ void (*func16)(Object* self); //clear pointer to other DFropenode object
};

#define dll_DFropenode(obj) (((DLL_420_DFRopeNode*)obj->dll)->vtbl)

#endif // _DLLS_420_H

