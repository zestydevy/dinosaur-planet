#ifndef _DLLS_283_H
#define _DLLS_283_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_283_KrazoaText) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*PrintText)(Object* self, Gfx** gfx);
};

#define dll_KrazoaText(obj) (((DLL_283_KrazoaText*)obj->dll)->vtbl)

#endif // _DLLS_283_H

