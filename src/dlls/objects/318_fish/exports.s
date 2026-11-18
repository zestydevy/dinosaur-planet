.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword fish_ctor
.dword fish_dtor

# export table
/*0*/ .dword fish_obj_Setup
/*1*/ .dword fish_obj_Control
/*2*/ .dword fish_obj_Update
/*3*/ .dword fish_obj_Print
/*4*/ .dword fish_obj_Free
/*5*/ .dword fish_obj_GetModelFlags
/*6*/ .dword fish_obj_GetDataSize
