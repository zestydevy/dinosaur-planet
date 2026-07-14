#ifndef _DLLS_216_H
#define _DLLS_216_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_216_SnowWorm) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ s32 (*get_anim_state)(Object* self);
    /*08*/ u8 (*receive_message)(Object* self, u8 message);
};

#endif //_DLLS_216_H
