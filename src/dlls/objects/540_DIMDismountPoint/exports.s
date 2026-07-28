.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMDismountPoint_ctor
.dword DIMDismountPoint_dtor

# export table
/*0*/ .dword DIMDismountPoint_obj_Setup
/*1*/ .dword DIMDismountPoint_obj_Control
/*2*/ .dword DIMDismountPoint_obj_Update
/*3*/ .dword DIMDismountPoint_obj_Print
/*4*/ .dword DIMDismountPoint_obj_Free
/*5*/ .dword DIMDismountPoint_obj_GetModelFlags
/*6*/ .dword DIMDismountPoint_obj_GetDataSize
/*7*/ .dword DIMDismountPoint_Func_1CC
/*8*/ .dword DIMDismountPoint_Func_28C
