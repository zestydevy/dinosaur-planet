#ifndef _DLLS_220_H
#define _DLLS_220_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

DLL_INTERFACE(DLL_220_Chuka) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ u8 (*receive_message)(Object* self, u8 message);
};

#endif //_DLLS_220_H
