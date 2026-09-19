.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFGuardian_ctor
.dword CFGuardian_dtor

# export table
/*0*/ .dword CFGuardian_obj_Setup
/*1*/ .dword CFGuardian_obj_Control
/*2*/ .dword CFGuardian_obj_Update
/*3*/ .dword CFGuardian_obj_Print
/*4*/ .dword CFGuardian_obj_Free
/*5*/ .dword CFGuardian_obj_GetModelFlags
/*6*/ .dword CFGuardian_obj_GetDataSize
/*7*/ .dword CFGuardian_Func_2778
