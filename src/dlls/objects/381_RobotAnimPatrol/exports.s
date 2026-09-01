.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword RobotAnimPatrol_ctor
.dword RobotAnimPatrol_dtor

# export table
/*0*/ .dword RobotAnimPatrol_obj_Setup
/*1*/ .dword RobotAnimPatrol_obj_Control
/*2*/ .dword RobotAnimPatrol_obj_Update
/*3*/ .dword RobotAnimPatrol_obj_Print
/*4*/ .dword RobotAnimPatrol_obj_Free
/*5*/ .dword RobotAnimPatrol_obj_GetModelFlags
/*6*/ .dword RobotAnimPatrol_obj_GetDataSize
