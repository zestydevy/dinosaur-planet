.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_629_ctor
.dword dll_629_dtor

# export table
/*0*/ .dword dll_629_obj_Setup
/*1*/ .dword dll_629_obj_Control
/*2*/ .dword dll_629_obj_Update
/*3*/ .dword dll_629_obj_Print
/*4*/ .dword dll_629_obj_Free
/*5*/ .dword dll_629_obj_GetModelFlags
/*6*/ .dword dll_629_obj_GetDataSize
