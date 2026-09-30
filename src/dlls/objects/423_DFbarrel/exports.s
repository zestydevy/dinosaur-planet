.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFbarrel_ctor
.dword DFbarrel_dtor

# export table
/*0*/ .dword DFbarrel_obj_Setup
/*1*/ .dword DFbarrel_obj_Control
/*2*/ .dword DFbarrel_obj_Update
/*3*/ .dword DFbarrel_obj_Print
/*4*/ .dword DFbarrel_obj_Free
/*5*/ .dword DFbarrel_obj_GetModelFlags
/*6*/ .dword DFbarrel_obj_GetDataSize
