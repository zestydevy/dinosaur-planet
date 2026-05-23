#ifndef _DLLS_COMMON_DINOCALLER_H
#define _DLLS_COMMON_DINOCALLER_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_IDinoCaller) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ s32 (*call)(Object *self);
};

#endif
