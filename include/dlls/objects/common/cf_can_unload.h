#ifndef _DLLS_COMMON_CF_CAN_UNLOAD_H
#define _DLLS_COMMON_CF_CAN_UNLOAD_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_ICFCanUnload) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ s32 (*CanUnload)(Object *self);
};

#endif
