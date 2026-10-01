.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TexScroll2_ctor
.dword TexScroll2_dtor

# export table
/*0*/ .dword TexScroll2_obj_Setup
/*1*/ .dword TexScroll2_obj_Control
/*2*/ .dword TexScroll2_obj_Update
/*3*/ .dword TexScroll2_obj_Print
/*4*/ .dword TexScroll2_obj_Free
/*5*/ .dword TexScroll2_obj_GetModelFlags
/*6*/ .dword TexScroll2_obj_GetDataSize
/*7*/ .dword TexScroll2_changeScrollSpeed
