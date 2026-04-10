#ifndef _DLLS_COMMON_SIDEKICK_H
#define _DLLS_COMMON_SIDEKICK_H

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
	/*13*/ s32 (*func13)(Object*);
	/*14*/ void (*func14)(Object* self, s32 gamebitID); //show a sidekick command in the inventory
	/*15*/ s32 (*get_blue_food_count)(Object *); // how much blue food they currently have
	/*16*/ s32 (*get_red_food_count)(Object *); // how much red food they currently have
	/*17*/ UnknownDLLFunc func17;
	/*18*/ void (*func18)(Object* self, s32);
	/*19*/ UnknownDLLFunc func19;
	/*20*/ UnknownDLLFunc func20;
	/*21*/ void (*func21)(Object *, s32, s32);
	/*22*/ void (*func22)(Object *, Object *);
	/*23*/ void (*func23)(Object *);
	/*24*/ s32 (*func24)(Object *);
	/*25*/ s32 (*func25)(Object*);
 	/*26*/ s32 (*func26)(Object*, s32*);
};

#endif //_DLLS_COMMON_SIDEKICK_H
