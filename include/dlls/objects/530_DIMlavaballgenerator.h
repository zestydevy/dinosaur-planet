#ifndef _DLLS_530_H
#define _DLLS_530_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_530_DIMLavaBallGenerator) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*func7)(Object *self, Object *arg1);
    /*8*/ void (*func8)(Object *self, Object *arg1);
};

#endif // _DLLS_530_H
