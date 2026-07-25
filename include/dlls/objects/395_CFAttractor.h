#ifndef _DLLS_395_H
#define _DLLS_395_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 mode;
    s16 unk1A;
    f32 unk1C;
    s8 unk20;
    s8 unk21;
    s8 unk22;
    s8 unk23;
} CFAttractor_Setup;

DLL_INTERFACE(DLL_395_CFAttractor) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s16 (*func7)(Object* self);
    /*8*/ void (*func8)(Object* self, Object** outObj);
};

#endif // _DLLS_395_H
