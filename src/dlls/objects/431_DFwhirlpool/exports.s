.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFwhirlpool_ctor
.dword DFwhirlpool_dtor

# export table
/*0*/ .dword DFwhirlpool_obj_Setup
/*1*/ .dword DFwhirlpool_obj_Control
/*2*/ .dword DFwhirlpool_obj_Update
/*3*/ .dword DFwhirlpool_obj_Print
/*4*/ .dword DFwhirlpool_obj_Free
/*5*/ .dword DFwhirlpool_obj_GetModelFlags
/*6*/ .dword DFwhirlpool_obj_GetDataSize
