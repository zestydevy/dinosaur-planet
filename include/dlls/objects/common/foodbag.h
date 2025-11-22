#ifndef _DLLS_COMMON_FOODBAG_H
#define _DLLS_COMMON_FOODBAG_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_IFoodbag) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ UnknownDLLFunc func7;
	/*8*/ UnknownDLLFunc func8;
	/*9*/ UnknownDLLFunc func9;
	/*10*/ UnknownDLLFunc func10;
	/*11*/ UnknownDLLFunc func11;
	/*12*/ void (*func12)(Object *self, s16 a1);
};

#endif //_DLLS_COMMON_FOODBAG_H
