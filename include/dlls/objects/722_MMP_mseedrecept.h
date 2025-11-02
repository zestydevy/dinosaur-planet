#ifndef _DLLS_722_H
#define _DLLS_722_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_722_MMPMseedrecept) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*func7)(Object* self, s32 arg1);
    /*8*/ u32 (*func8)(Object *self, u32 a1, u32 a2);
    /*9*/ u32 (*func9)(Object *self, u32 a1, u32 a2);
    /*10*/ u32 (*func10)(Object *self);
};

#endif // _DLLS_722_H

