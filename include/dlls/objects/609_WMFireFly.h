#ifndef _DLLS_609_H
#define _DLLS_609_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_609_WMFireFly) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*set_next_spline_coord)(Object* self, Vec3f* coords);
};

#endif // _DLLS_609_H

