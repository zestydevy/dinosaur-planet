#ifndef _DLLS_702_H
#define _DLLS_702_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_702_KTrex) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s16 (*get_fsa_anim_state)(Object* self);
    /*8*/ void (*func8)(Object *self, s32 arg1);
};

#endif // _DLLS_702_H
