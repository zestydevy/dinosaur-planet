.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_Column_ctor
.dword WL_Column_dtor

# export table
/*0*/ .dword WL_Column_obj_Setup
/*1*/ .dword WL_Column_obj_Control
/*2*/ .dword WL_Column_obj_Update
/*3*/ .dword WL_Column_obj_Print
/*4*/ .dword WL_Column_obj_Free
/*5*/ .dword WL_Column_obj_getModelFlags
/*6*/ .dword WL_Column_obj_getDataSize
