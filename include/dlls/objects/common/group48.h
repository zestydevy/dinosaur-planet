#ifndef _DLLS_COMMON_GROUP_48_H
#define _DLLS_COMMON_GROUP_48_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "PR/gbi.h"

DLL_INTERFACE(DLL_IGROUP_48) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ void (*func7)(Object *, f32);
	/*8*/ void (*func8)(Object *);
	/*9*/ UnknownDLLFunc func9;
	/*10*/ void (*func10)(Object *, Gfx** arg0, Mtx** arg1, Vertex** arg2, Triangle** arg3);
	/*11*/ void (*func11)(Object *);
	/*12*/ void (*func12)(Object *, s32);
	/*13*/ void (*func13)(Object *, u8);
	/*14*/ UnknownDLLFunc func14;
	/*15*/ UnknownDLLFunc func15;
	/*16*/ UnknownDLLFunc func16;
	/*17*/ UnknownDLLFunc func17;
	/*14*/ void (*func18)(Object *, f32, f32);
};

#endif //_DLLS_COMMON_GROUP_48_H
