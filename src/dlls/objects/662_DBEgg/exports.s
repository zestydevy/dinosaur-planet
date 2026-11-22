.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBEgg_ctor
.dword DBEgg_dtor

# export table
/*0*/ .dword DBEgg_obj_Setup
/*1*/ .dword DBEgg_obj_Control
/*2*/ .dword DBEgg_obj_Update
/*3*/ .dword DBEgg_obj_Print
/*4*/ .dword DBEgg_obj_Free
/*5*/ .dword DBEgg_obj_GetModelFlags
/*6*/ .dword DBEgg_obj_GetDataSize
