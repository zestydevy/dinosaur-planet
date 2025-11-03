#ifndef _DLLS_519_H
#define _DLLS_519_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_519_SC_Levelcontrol) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*func7)(Object *self, u8 arg1);
    /*8*/ u8 (*func8)(Object *self);
};

#endif // _DLLS_519_H
