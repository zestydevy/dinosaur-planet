.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFP_Statue1_ctor
.dword DFP_Statue1_dtor

# export table
/*0*/ .dword DFP_Statue1_obj_Setup
/*1*/ .dword DFP_Statue1_obj_Control
/*2*/ .dword DFP_Statue1_obj_Update
/*3*/ .dword DFP_Statue1_obj_Print
/*4*/ .dword DFP_Statue1_obj_Free
/*5*/ .dword DFP_Statue1_obj_GetModelFlags
/*6*/ .dword DFP_Statue1_obj_GetDataSize
