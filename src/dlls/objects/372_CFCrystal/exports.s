.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFCrystal_ctor
.dword CFCrystal_dtor

# export table
/*0*/ .dword CFCrystal_obj_Setup
/*1*/ .dword CFCrystal_obj_Control
/*2*/ .dword CFCrystal_obj_Update
/*3*/ .dword CFCrystal_obj_Print
/*4*/ .dword CFCrystal_obj_Free
/*5*/ .dword CFCrystal_obj_GetModelFlags
/*6*/ .dword CFCrystal_obj_GetDataSize
