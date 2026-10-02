#ifndef _DLLS_373_H
#define _DLLS_373_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_373_CFCloudBaby) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*IsRescuedTimerDone)(Object* self);
    /*8*/ s32 (*RescueFromChest)(Object* self);
};

#endif // _DLLS_373_H
