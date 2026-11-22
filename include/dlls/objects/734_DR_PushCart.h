#ifndef _DLLS_734_H
#define _DLLS_734_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_734_DR_PushCart) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ f32 (*Func7)(Object* self, Object* arg1, f32 arg2);
};

#endif
