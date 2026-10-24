#ifndef _DLLS_304_H
#define _DLLS_304_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_304_LanternFireFly) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*set_next_spline_coord)(Object* self, Vec3f* coords);
    /*8*/ void (*send)(Object *self);
    /*9*/ void (*set_home)(Object* self, f32 x, f32 y, f32 z);
};

#endif // _DLLS_304_H

