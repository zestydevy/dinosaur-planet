#ifndef _DLLS_XXX_SIDEKICK_H
#define _DLLS_XXX_SIDEKICK_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_ISidekick) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ UnknownDLLFunc func7;
	/*8*/ UnknownDLLFunc func8;
	/*9*/ UnknownDLLFunc func9;
	/*10*/ UnknownDLLFunc func10;
	/*11*/ UnknownDLLFunc func11;
	/*12*/ UnknownDLLFunc func12;
	/*13*/ UnknownDLLFunc func13;
	/*14*/ UnknownDLLFunc func14;
	/*15*/ UnknownDLLFunc func15;
	/*16*/ UnknownDLLFunc func16;
	/*17*/ UnknownDLLFunc func17;
	/*18*/ UnknownDLLFunc func18;
	/*19*/ UnknownDLLFunc func19;
	/*20*/ UnknownDLLFunc func20;
	/*21*/ void (*func21)(Object *, s32, s32);
	/*22*/ void (*func22)(Object *, Object *);
	/*23*/ void (*func23)(Object *);
};

#endif //_DLLS_210_H
