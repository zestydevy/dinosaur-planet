.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DLL307_ctor
.dword DLL307_dtor

# export table
/*0*/ .dword DLL307_obj_Setup
/*1*/ .dword DLL307_obj_Control
/*2*/ .dword DLL307_obj_Update
/*3*/ .dword DLL307_obj_Print
/*4*/ .dword DLL307_obj_Free
/*5*/ .dword DLL307_obj_GetModelFlags
/*6*/ .dword DLL307_obj_GetDataSize
