.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DF_Lantern_ctor
.dword DF_Lantern_dtor

# export table
/*0*/ .dword DF_Lantern_obj_Setup
/*1*/ .dword DF_Lantern_obj_Control
/*2*/ .dword DF_Lantern_obj_Update
/*3*/ .dword DF_Lantern_obj_Print
/*4*/ .dword DF_Lantern_obj_Free
/*5*/ .dword DF_Lantern_obj_GetModelFlags
/*6*/ .dword DF_Lantern_obj_GetDataSize
