.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFlog_ctor
.dword DFlog_dtor

# export table
/*0*/ .dword DFlog_obj_Setup
/*1*/ .dword DFlog_obj_Control
/*2*/ .dword DFlog_obj_Update
/*3*/ .dword DFlog_obj_Print
/*4*/ .dword DFlog_obj_Free
/*5*/ .dword DFlog_obj_GetModelFlags
/*6*/ .dword DFlog_obj_GetDataSize
/*7*/ .dword DFlog_vehicle_CanMount
/*8*/ .dword DFlog_vehicle_GetMountSide
/*9*/ .dword DFlog_vehicle_GetRiderPosition
/*10*/ .dword DFlog_vehicle_CanDismount
/*11*/ .dword DFlog_vehicle_GetDismountSide
/*12*/ .dword DFlog_vehicle_GetCameraPosition
/*13*/ .dword DFlog_vehicle_GetMountState
/*14*/ .dword DFlog_vehicle_SetMountState
/*15*/ .dword DFlog_vehicle_GetPlayerAnim
/*16*/ .dword DFlog_vehicle_Func16
/*17*/ .dword DFlog_vehicle_GetRacePosition
/*18*/ .dword DFlog_vehicle_Func18
/*19*/ .dword DFlog_vehicle_HandleRiderScale
/*20*/ .dword DFlog_vehicle_Func20
