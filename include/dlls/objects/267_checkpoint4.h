#ifndef _DLLS_267_H
#define _DLLS_267_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    u8 unk18[0x28 - 0x18];
    s8 unk28;
    u8 yaw;
    u8 scale;
} checkpoint4_Setup;

DLL_INTERFACE(DLL_267_Checkpoint4) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*func7)(Object *self);
};

#endif // _DLLS_267_H
