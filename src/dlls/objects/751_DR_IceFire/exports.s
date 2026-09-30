.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DRIceFire_ctor
.dword DRIceFire_dtor

# export table
/*0*/ .dword DRIceFire_obj_Setup
/*1*/ .dword DRIceFire_obj_Control
/*2*/ .dword DRIceFire_obj_Update
/*3*/ .dword DRIceFire_obj_Print
/*4*/ .dword DRIceFire_obj_Free
/*5*/ .dword DRIceFire_obj_GetModelFlags
/*6*/ .dword DRIceFire_obj_GetDataSize
