#ifndef _DLLS_609_H
#define _DLLS_609_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s8 varianceY;
    s8 effectType;
    s16 fxRange;
    s16 varianceZ;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} WMFireFly_Setup;

DLL_INTERFACE(DLL_609_WMFireFly) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*set_next_spline_coord)(Object* self, Vec3f* coords);
};

#endif // _DLLS_609_H

