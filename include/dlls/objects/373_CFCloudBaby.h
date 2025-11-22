#ifndef _DLLS_373_H
#define _DLLS_373_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_373_CFCloudBaby) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*func7)(Object*); // returns whether a flag is unset
    /*8*/ s32 (*func8)(Object*);
};

#endif // _DLLS_373_H
