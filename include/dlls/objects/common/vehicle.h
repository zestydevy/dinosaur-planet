#ifndef _DLLS_COMMON_VEHICLE_H
#define _DLLS_COMMON_VEHICLE_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

enum VehicleMountState {
	VEHICLE_NoRider = 0,
	VEHICLE_Mounting = 1,
	VEHICLE_Mounted = 2,
	VEHICLE_Dismounting = 3
};

DLL_INTERFACE(DLL_IVehicle) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ s32 (*CanMount)(Object *self, Object *rider);
	/*8*/ s32 (*GetMountSide)(Object *self); // 1 = left, 2 = right
	/*9*/ void (*GetRiderPosition)(Object *self, f32 *x, f32 *y, f32 *z);
	/*10*/ s32 (*CanDismount)(Object *self, Object *rider);
	/*11*/ s32 (*GetDismountSide)(Object *self); // 1 = left, 2 = right
	/*12*/ void (*GetCameraPosition)(Object *self, f32 *x, f32 *y, f32 *z);
	/*13*/ s32 (*GetMountState)(Object *self);
	/*14*/ void (*SetMountState)(Object *self, s32 state);
	/*15*/ void (*GetPlayerAnim)(Object *self, f32 *tValue, s32 *animIndex);
	/*16*/ f32 (*Func16)(Object *self, f32 *arg1); // anim tick delta related
	/*17*/ s32 (*GetRacePosition)(Object *self);
	/*18*/ void (*Func18)(Object *self);
	/*19*/ void (*HandleRiderScale)(Object *self, f32 scale);
	/*20*/ void (*Func20)(Object *self, s32 a1, s32 a2);
};

#define dll_vehicle(obj) (((DLL_IVehicle*)(obj)->dll)->vtbl)

#endif //_DLLS_COMMON_VEHICLE_H
