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
	/*7*/ s32 (*can_mount)(Object *self, Object *rider);
	/*8*/ s32 (*get_mount_side)(Object *self); // 1 = left, 2 = right
	/*9*/ void (*get_rider_position)(Object *self, f32 *x, f32 *y, f32 *z);
	/*10*/ s32 (*can_dismount)(Object *self, Object *rider);
	/*11*/ s32 (*get_dismount_side)(Object *self); // 1 = left, 2 = right
	/*12*/ void (*get_camera_position)(Object *self, f32 *x, f32 *y, f32 *z);
	/*13*/ s32 (*get_mount_state)(Object *self);
	/*14*/ void (*set_mount_state)(Object *self, s32 state);
	/*15*/ void (*get_player_anim)(Object *self, f32 *tValue, s32 *animIndex);
	/*16*/ f32 (*func16)(Object *self, f32 *arg1); // anim tick delta related
	/*17*/ s32 (*get_race_position)(Object *self);
	/*18*/ void (*func18)(Object *self);
	/*19*/ void (*handle_rider_scale)(Object *self, f32 scale);
	/*20*/ void (*func20)(Object *self, s32 a1, s32 a2);
};

#endif //_DLLS_COMMON_VEHICLE_H
