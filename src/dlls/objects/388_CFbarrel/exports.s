.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFbarrel_ctor
.dword CFbarrel_dtor

# export table
/*0*/ .dword CFbarrel_obj_Setup
/*1*/ .dword CFbarrel_obj_Control
/*2*/ .dword CFbarrel_obj_Update
/*3*/ .dword CFbarrel_obj_Print
/*4*/ .dword CFbarrel_obj_Free
/*5*/ .dword CFbarrel_obj_GetModelFlags
/*6*/ .dword CFbarrel_obj_GetDataSize
/*7*/ .dword CFbarrel_Func_17F4
