.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword StaticCamera_ctor
.dword StaticCamera_dtor

# export table
/*0*/ .dword StaticCamera_obj_Setup
/*1*/ .dword StaticCamera_obj_Control
/*2*/ .dword StaticCamera_obj_Update
/*3*/ .dword StaticCamera_obj_Print
/*4*/ .dword StaticCamera_obj_Free
/*5*/ .dword StaticCamera_obj_GetModelFlags
/*6*/ .dword StaticCamera_obj_GetDataSize
