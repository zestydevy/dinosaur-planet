.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ProjBall_ctor
.dword ProjBall_dtor

# export table
/*0*/ .dword ProjBall_obj_Setup
/*1*/ .dword ProjBall_obj_Control
/*2*/ .dword ProjBall_obj_Update
/*3*/ .dword ProjBall_obj_Print
/*4*/ .dword ProjBall_obj_Free
/*5*/ .dword ProjBall_obj_GetModelFlags
/*6*/ .dword ProjBall_obj_GetDataSize
