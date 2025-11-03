#ifndef _DLLS_723_H
#define _DLLS_723_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_723_GPbonfire) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*start_burning)(Object* self, s32 skipSequence);
    /*8*/ s32 (*func8)(Object* self, s32 arg1, s32 arg2);
    /*9*/ s32 (*func9)(Object* self, s32 arg1, s32 arg2);
    /*10*/ s32 (*func10)(void);
};

#endif // _DLLS_723_H

