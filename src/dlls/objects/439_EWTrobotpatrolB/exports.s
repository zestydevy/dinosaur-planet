.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_439_ctor
.dword dll_439_dtor

# export table
/*0*/ .dword dll_439_obj_Setup
/*1*/ .dword dll_439_obj_Control
/*2*/ .dword dll_439_obj_Update
/*3*/ .dword dll_439_obj_Print
/*4*/ .dword dll_439_obj_Free
/*5*/ .dword dll_439_obj_GetModelFlags
/*6*/ .dword dll_439_obj_GetDataSize
