.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CloudBall_ctor
.dword CloudBall_dtor

# export table
/*0*/ .dword CloudBall_obj_Setup
/*1*/ .dword CloudBall_obj_Control
/*2*/ .dword CloudBall_obj_Update
/*3*/ .dword CloudBall_obj_Print
/*4*/ .dword CloudBall_obj_Free
/*5*/ .dword CloudBall_obj_GetModelFlags
/*6*/ .dword CloudBall_obj_GetDataSize
