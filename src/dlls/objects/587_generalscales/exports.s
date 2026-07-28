.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GeneralScales_ctor
.dword GeneralScales_dtor

# export table
/*0*/ .dword GeneralScales_obj_Setup
/*1*/ .dword GeneralScales_obj_Control
/*2*/ .dword GeneralScales_obj_Update
/*3*/ .dword GeneralScales_obj_Print
/*4*/ .dword GeneralScales_obj_Free
/*5*/ .dword GeneralScales_obj_GetModelFlags
/*6*/ .dword GeneralScales_obj_GetDataSize
