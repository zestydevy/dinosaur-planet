.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BlownUpTarget_ctor
.dword BlownUpTarget_dtor

# export table
/*0*/ .dword BlownUpTarget_obj_Setup
/*1*/ .dword BlownUpTarget_obj_Control
/*2*/ .dword BlownUpTarget_obj_Update
/*3*/ .dword BlownUpTarget_obj_Print
/*4*/ .dword BlownUpTarget_obj_Free
/*5*/ .dword BlownUpTarget_obj_GetModelFlags
/*6*/ .dword BlownUpTarget_obj_GetDataSize
