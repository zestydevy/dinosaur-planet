#ifndef _DLLS_COMMON_VEHICLE_H
#define _DLLS_COMMON_VEHICLE_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_IVehicle) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ s32 (*func7)(Object *self, Object *rider);
	/*8*/ s32 (*func8)(Object *self);
	/*9*/ void (*func9)(Object *self, f32 *a1, f32 *a2, f32 *a3);
	/*10*/ s32 (*func10)(Object *self, Object *rider);
	/*11*/ s32 (*func11)(Object *self);
	/*12*/ void (*func12)(Object *self, f32 *a1, f32 *a2, f32 *a3);
	/*13*/ s32 (*func13)(Object *self);
	/*14*/ void (*func14)(Object *self, s32 arg1);
	/*15*/ void (*func15)(Object *self, f32 *arg1, s32 *arg2);
	/*16*/ f32 (*func16)(Object *self, f32 *arg1);
	/*17*/ s32 (*func17)(Object *self); // get_race_position?
	/*18*/ void (*func18)(Object *self);
	/*19*/ void (*func19)(Object *self, f32 scale);
	/*20*/ void (*func20)(s32 a0, s32 a1, s32 a2);
};

#endif //_DLLS_COMMON_VEHICLE_H
