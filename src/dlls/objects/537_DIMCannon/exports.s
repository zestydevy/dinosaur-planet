.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMCannon_ctor
.dword DIMCannon_dtor

# export table
/*0*/ .dword DIMCannon_obj_Setup
/*1*/ .dword DIMCannon_obj_Control
/*2*/ .dword DIMCannon_obj_Update
/*3*/ .dword DIMCannon_obj_Print
/*4*/ .dword DIMCannon_obj_Free
/*5*/ .dword DIMCannon_obj_GetModelFlags
/*6*/ .dword DIMCannon_obj_GetDataSize
