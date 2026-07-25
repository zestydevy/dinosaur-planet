.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFPerch_ctor
.dword CFPerch_dtor

# export table
/*0*/ .dword CFPerch_obj_Setup
/*1*/ .dword CFPerch_obj_Control
/*2*/ .dword CFPerch_obj_Update
/*3*/ .dword CFPerch_obj_Print
/*4*/ .dword CFPerch_obj_Free
/*5*/ .dword CFPerch_obj_GetModelFlags
/*6*/ .dword CFPerch_obj_GetDataSize
