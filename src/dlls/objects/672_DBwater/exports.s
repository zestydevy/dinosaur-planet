.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBwater_ctor
.dword DBwater_dtor

# export table
/*0*/ .dword DBwater_obj_Setup
/*1*/ .dword DBwater_obj_Control
/*2*/ .dword DBwater_obj_Update
/*3*/ .dword DBwater_obj_Print
/*4*/ .dword DBwater_obj_Free
/*5*/ .dword DBwater_obj_GetModelFlags
/*6*/ .dword DBwater_obj_GetDataSize
