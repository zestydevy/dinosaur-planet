.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHthorntail_ctor
.dword SHthorntail_dtor

# export table
/*0*/ .dword SHthorntail_obj_Setup
/*1*/ .dword SHthorntail_obj_Control
/*2*/ .dword SHthorntail_obj_Update
/*3*/ .dword SHthorntail_obj_Print
/*4*/ .dword SHthorntail_obj_Free
/*5*/ .dword SHthorntail_obj_GetModelFlags
/*6*/ .dword SHthorntail_obj_GetDataSize
