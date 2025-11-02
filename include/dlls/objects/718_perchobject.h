#ifndef _DLLS_718_H
#define _DLLS_718_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_718_PerchObject) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*land_on_perch)(Object* self, s32 arg1);
    /*8*/ u32 (*func8)(Object *self, s32 arg1, s32 arg2);
    /*9*/ u32 (*approach_perch)(Object* self, s32 arg1, f32* deltaY);
    /*10*/ u32 (*func10)(Object *self);
};

#endif // _DLLS_718_H

