.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFCradle_ctor
.dword DFCradle_dtor

# export table
/*0*/ .dword DFCradle_obj_Setup
/*1*/ .dword DFCradle_obj_Control
/*2*/ .dword DFCradle_obj_Update
/*3*/ .dword DFCradle_obj_Print
/*4*/ .dword DFCradle_obj_Free
/*5*/ .dword DFCradle_obj_GetModelFlags
/*6*/ .dword DFCradle_obj_GetDataSize
