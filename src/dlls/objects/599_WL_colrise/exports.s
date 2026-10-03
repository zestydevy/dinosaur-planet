.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_colrise_ctor
.dword WL_colrise_dtor

# export table
/*0*/ .dword WL_colrise_obj_Setup
/*1*/ .dword WL_colrise_obj_Control
/*2*/ .dword WL_colrise_obj_Update
/*3*/ .dword WL_colrise_obj_Print
/*4*/ .dword WL_colrise_obj_Free
/*5*/ .dword WL_colrise_obj_GetModelFlags
/*6*/ .dword WL_colrise_obj_GetDataSize
