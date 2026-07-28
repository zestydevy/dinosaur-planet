.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFPCorePlat_ctor
.dword VFPCorePlat_dtor

# export table
/*0*/ .dword VFPCorePlat_obj_Setup
/*1*/ .dword VFPCorePlat_obj_Control
/*2*/ .dword VFPCorePlat_obj_Update
/*3*/ .dword VFPCorePlat_obj_Print
/*4*/ .dword VFPCorePlat_obj_Free
/*5*/ .dword VFPCorePlat_obj_GetModelFlags
/*6*/ .dword VFPCorePlat_obj_GetDataSize
