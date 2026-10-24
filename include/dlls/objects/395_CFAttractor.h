#ifndef _DLLS_395_H
#define _DLLS_395_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_395_CFAttractor) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s16 (*func7)(Object* self);
    /*8*/ void (*func8)(Object* self, Object** outObj);
};

#endif // _DLLS_395_H
