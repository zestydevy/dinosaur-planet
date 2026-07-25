.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword IMSpaceRing_ctor
.dword IMSpaceRing_dtor

# export table
/*0*/ .dword IMSpaceRing_obj_Setup
/*1*/ .dword IMSpaceRing_obj_Control
/*2*/ .dword IMSpaceRing_obj_Update
/*3*/ .dword IMSpaceRing_obj_Print
/*4*/ .dword IMSpaceRing_obj_Free
/*5*/ .dword IMSpaceRing_obj_GetModelFlags
/*6*/ .dword IMSpaceRing_obj_GetDataSize
