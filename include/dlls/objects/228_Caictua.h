#ifndef _DLLS_258_H
#define _DLLS_258_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_228_Caictua) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*GetAnimState)(Object* self, s32 unused);
    /*8*/ void (*ReceiveMessage)(Object* self, u8 message, s32 unused);
};

#define dll_Caictua(obj) (((DLL_228_Caictua*)obj->dll)->vtbl)

#endif // _DLLS_258_H

