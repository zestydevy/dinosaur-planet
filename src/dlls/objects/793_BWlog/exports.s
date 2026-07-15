.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BWlog_ctor
.dword BWlog_dtor

# export table
/*0*/ .dword BWlog_obj_Setup
/*1*/ .dword BWlog_obj_Control
/*2*/ .dword BWlog_obj_Update
/*3*/ .dword BWlog_obj_Print
/*4*/ .dword BWlog_obj_Free
/*5*/ .dword BWlog_obj_GetModelFlags
/*6*/ .dword BWlog_obj_GetDataSize
/*7*/ .dword BWlog_vehicle_CanMount
/*8*/ .dword BWlog_vehicle_GetMountSide
/*9*/ .dword BWlog_vehicle_GetRiderPosition
/*10*/ .dword BWlog_vehicle_CanDismount
/*11*/ .dword BWlog_vehicle_GetDismountSide
/*12*/ .dword BWlog_vehicle_GetCameraPosition
/*13*/ .dword BWlog_vehicle_GetMountState
/*14*/ .dword BWlog_vehicle_SetMountState
/*15*/ .dword BWlog_vehicle_GetPlayerAnim
/*16*/ .dword BWlog_vehicle_Func16
/*17*/ .dword BWlog_vehicle_GetRacePosition
/*18*/ .dword BWlog_vehicle_Func18
/*19*/ .dword BWlog_vehicle_HandleRiderScale
/*20*/ .dword BWlog_vehicle_Func20
