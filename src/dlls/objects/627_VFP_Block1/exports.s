.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_Block1_ctor
.dword VFP_Block1_dtor

# export table
/*0*/ .dword VFP_Block1_obj_Setup
/*1*/ .dword VFP_Block1_obj_Control
/*2*/ .dword VFP_Block1_obj_Update
/*3*/ .dword VFP_Block1_obj_Print
/*4*/ .dword VFP_Block1_obj_Free
/*5*/ .dword VFP_Block1_obj_GetModelFlags
/*6*/ .dword VFP_Block1_obj_GetDataSize
