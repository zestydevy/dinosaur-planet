#ifndef _DLLS_222_H
#define _DLLS_222_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_222_SnowWormSmall) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ s32 (*get_anim_state)(Object* self, s32 unused);
    /*08*/ u8 (*receive_message)(Object* self, u8 message, s32 unused);
};

#endif //_DLLS_222_H
