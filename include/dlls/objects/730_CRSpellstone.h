#ifndef _DLLS_730_H
#define _DLLS_730_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_730_CRSpellstone) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*SetState)(Object* self, s32 newValue);
    /*8*/ int (*IsNotRescued)(Object* self);
};

#endif // _DLLS_730_H
