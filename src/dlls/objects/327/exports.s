.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DLL327_ctor
.dword DLL327_dtor

# export table
/*0*/ .dword DLL327_obj_Setup
/*1*/ .dword DLL327_obj_Control
/*2*/ .dword DLL327_obj_Update
/*3*/ .dword DLL327_obj_Print
/*4*/ .dword DLL327_obj_Free
/*5*/ .dword DLL327_obj_GetModelFlags
/*6*/ .dword DLL327_obj_GetDataSize
