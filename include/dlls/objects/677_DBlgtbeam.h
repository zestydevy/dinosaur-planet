#ifndef _DLLS_677_H
#define _DLLS_677_H

#include "PR/ultratypes.h"
#include "types.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_677_DBlgtbeam) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*func7)(Object* self, s32 stepNum);
    /*8*/ UnknownDLLFunc func8;
    /*9*/ s32 (*func9)(Object* self, s32 stepNum);
    //TODO: find out how many other exports this DLL has
};

#endif // _DLLS_677_H
