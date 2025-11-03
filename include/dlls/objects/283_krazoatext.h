#ifndef _DLLS_283_H
#define _DLLS_283_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_283_Krazoatext) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*func7)(Object* self, Gfx** gfx);
};

#endif // _DLLS_283_H
