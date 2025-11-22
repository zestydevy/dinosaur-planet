#ifndef _DLLS_573_H
#define _DLLS_573_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_573_SB_Cloudrunner) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*func7)(Object *self, f32*, f32*, f32*);
    /*8*/ UnknownDLLFunc func8;
    /*9*/ UnknownDLLFunc func9;
    /*10*/ void (*func10)(Object *self, f32*, f32*);
};

#endif // _DLLS_573_H
