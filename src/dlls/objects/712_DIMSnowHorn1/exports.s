.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMSnowHorn_ctor
.dword DIMSnowHorn_dtor

# export table
/*0*/ .dword DIMSnowHorn_obj_Setup
/*1*/ .dword DIMSnowHorn_obj_Control
/*2*/ .dword DIMSnowHorn_obj_Update
/*3*/ .dword DIMSnowHorn_obj_Print
/*4*/ .dword DIMSnowHorn_obj_Free
/*5*/ .dword DIMSnowHorn_obj_GetModelFlags
/*6*/ .dword DIMSnowHorn_obj_GetDataSize
/*7*/ .dword DIMSnowHorn_vehicle_CanMount
/*8*/ .dword DIMSnowHorn_vehicle_GetMountSide
/*9*/ .dword DIMSnowHorn_vehicle_GetRiderPosition
/*10*/ .dword DIMSnowHorn_vehicle_CanDismount
/*11*/ .dword DIMSnowHorn_vehicle_GetDismountSide
/*12*/ .dword DIMSnowHorn_vehicle_GetCameraPosition
/*13*/ .dword DIMSnowHorn_vehicle_GetMountState
/*14*/ .dword DIMSnowHorn_vehicle_SetMountState
/*15*/ .dword DIMSnowHorn_vehicle_GetPlayerAnim
/*16*/ .dword DIMSnowHorn_vehicle_Func16
/*17*/ .dword DIMSnowHorn_vehicle_GetRacePosition
/*18*/ .dword DIMSnowHorn_vehicle_Func18
/*19*/ .dword DIMSnowHorn_vehicle_HandleRiderScale
/*20*/ .dword DIMSnowHorn_vehicle_Func20
