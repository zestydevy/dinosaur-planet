.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword sideload_ctor
.dword sideload_dtor

# export table
/*0*/ .dword sideload_obj_Setup
/*1*/ .dword sideload_obj_Control
/*2*/ .dword sideload_obj_Update
/*3*/ .dword sideload_obj_Print
/*4*/ .dword sideload_obj_Free
/*5*/ .dword sideload_obj_GetModelFlags
/*6*/ .dword sideload_obj_GetDataSize
