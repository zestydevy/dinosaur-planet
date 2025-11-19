#ifndef _DLLS_572_H
#define _DLLS_572_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_572_SB_Galleon) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*func7)(Object *self);
    /*8*/ s8 (*func8)(Object *self);
};

#endif // _DLLS_572_H
