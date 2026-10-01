.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBfire_ctor
.dword DBfire_dtor

# export table
/*0*/ .dword DBfire_obj_Setup
/*1*/ .dword DBfire_obj_Control
/*2*/ .dword DBfire_obj_Update
/*3*/ .dword DBfire_obj_Print
/*4*/ .dword DBfire_obj_Free
/*5*/ .dword DBfire_obj_GetModelFlags
/*6*/ .dword DBfire_obj_GetDataSize
