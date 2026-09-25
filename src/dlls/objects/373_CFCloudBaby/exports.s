.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFCloudBaby_ctor
.dword CFCloudBaby_dtor

# export table
/*0*/ .dword CFCloudBaby_obj_Setup
/*1*/ .dword CFCloudBaby_obj_Control
/*2*/ .dword CFCloudBaby_obj_Update
/*3*/ .dword CFCloudBaby_obj_Print
/*4*/ .dword CFCloudBaby_obj_Free
/*5*/ .dword CFCloudBaby_obj_GetModelFlags
/*6*/ .dword CFCloudBaby_obj_GetDataSize
/*7*/ .dword CFCloudBaby_Func_12A4
/*8*/ .dword CFCloudBaby_Func_12BC
