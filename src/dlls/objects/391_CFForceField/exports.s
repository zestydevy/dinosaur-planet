.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFForceField_ctor
.dword CFForceField_dtor

# export table
/*0*/ .dword CFForceField_obj_Setup
/*1*/ .dword CFForceField_obj_Control
/*2*/ .dword CFForceField_obj_Update
/*3*/ .dword CFForceField_obj_Print
/*4*/ .dword CFForceField_obj_Free
/*5*/ .dword CFForceField_obj_GetModelFlags
/*6*/ .dword CFForceField_obj_GetDataSize
