#ifndef _DLLS_396_H
#define _DLLS_396_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dlls/objects/common/cf_can_unload.h"

DLL_INTERFACE(DLL_396_CFSupTreasureCh) {
    /*:*/ DLL_INTERFACE_BASE(DLL_ICFCanUnload);
    /*8*/ void (*Func8)(Object* self, Object* arg1);
    /*9*/ void (*Func9)(Object* self, Object* arg1, f32 arg2);
};

#endif
