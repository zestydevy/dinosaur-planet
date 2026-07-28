.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WM_MovePlatform_ctor
.dword WM_MovePlatform_dtor

# export table
/*0*/ .dword WM_MovePlatform_obj_Setup
/*1*/ .dword WM_MovePlatform_obj_Control
/*2*/ .dword WM_MovePlatform_obj_Update
/*3*/ .dword WM_MovePlatform_obj_Print
/*4*/ .dword WM_MovePlatform_obj_Free
/*5*/ .dword WM_MovePlatform_obj_GetModelFlags
/*6*/ .dword WM_MovePlatform_obj_GetDataSize
