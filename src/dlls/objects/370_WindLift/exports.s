.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WindLift_ctor
.dword WindLift_dtor

# export table
/*0*/ .dword WindLift_obj_Setup
/*1*/ .dword WindLift_obj_Control
/*2*/ .dword WindLift_obj_Update
/*3*/ .dword WindLift_obj_Print
/*4*/ .dword WindLift_obj_Free
/*5*/ .dword WindLift_obj_GetModelFlags
/*6*/ .dword WindLift_obj_GetDataSize
