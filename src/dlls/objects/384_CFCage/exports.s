.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFCage_ctor
.dword CFCage_dtor

# export table
/*0*/ .dword CFCage_obj_Setup
/*1*/ .dword CFCage_obj_Control
/*2*/ .dword CFCage_obj_Update
/*3*/ .dword CFCage_obj_Print
/*4*/ .dword CFCage_obj_Free
/*5*/ .dword CFCage_obj_GetModelFlags
/*6*/ .dword CFCage_obj_GetDataSize
