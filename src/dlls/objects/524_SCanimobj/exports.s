.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCAnimObj_ctor
.dword SCAnimObj_dtor

# export table
/*0*/ .dword SCAnimObj_obj_Setup
/*1*/ .dword SCAnimObj_obj_Control
/*2*/ .dword SCAnimObj_obj_Update
/*3*/ .dword SCAnimObj_obj_Print
/*4*/ .dword SCAnimObj_obj_Free
/*5*/ .dword SCAnimObj_obj_GetModelFlags
/*6*/ .dword SCAnimObj_obj_GetDataSize
