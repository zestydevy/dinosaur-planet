.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword capy_ctor
.dword capy_dtor

# export table
/*0*/ .dword capy_obj_Setup
/*1*/ .dword capy_obj_Control
/*2*/ .dword capy_obj_Update
/*3*/ .dword capy_obj_Print
/*4*/ .dword capy_obj_Free
/*5*/ .dword capy_obj_GetModelFlags
/*6*/ .dword capy_obj_GetDataSize
