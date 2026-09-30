.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TexScroll_ctor
.dword TexScroll_dtor

# export table
/*0*/ .dword TexScroll_obj_Setup
/*1*/ .dword TexScroll_obj_Control
/*2*/ .dword TexScroll_obj_Update
/*3*/ .dword TexScroll_obj_Print
/*4*/ .dword TexScroll_obj_Free
/*5*/ .dword TexScroll_obj_GetModelFlags
/*6*/ .dword TexScroll_obj_GetDataSize
