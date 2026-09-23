.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BlastedObject_ctor
.dword BlastedObject_dtor

# export table
/*0*/ .dword BlastedObject_obj_Setup
/*1*/ .dword BlastedObject_obj_Control
/*2*/ .dword BlastedObject_obj_Update
/*3*/ .dword BlastedObject_obj_Print
/*4*/ .dword BlastedObject_obj_Free
/*5*/ .dword BlastedObject_obj_GetModelFlags
/*6*/ .dword BlastedObject_obj_GetDataSize
