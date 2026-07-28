.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMTent_ctor
.dword DIMTent_dtor

# export table
/*0*/ .dword DIMTent_obj_Setup
/*1*/ .dword DIMTent_obj_Control
/*2*/ .dword DIMTent_obj_Update
/*3*/ .dword DIMTent_obj_Print
/*4*/ .dword DIMTent_obj_Free
/*5*/ .dword DIMTent_obj_GetModelFlags
/*6*/ .dword DIMTent_obj_GetDataSize
