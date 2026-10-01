.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFSH_ObjCreator_ctor
.dword DFSH_ObjCreator_dtor

# export table
/*0*/ .dword DFSH_ObjCreator_obj_Setup
/*1*/ .dword DFSH_ObjCreator_obj_Control
/*2*/ .dword DFSH_ObjCreator_obj_Update
/*3*/ .dword DFSH_ObjCreator_obj_Print
/*4*/ .dword DFSH_ObjCreator_obj_Free
/*5*/ .dword DFSH_ObjCreator_obj_GetModelFlags
/*6*/ .dword DFSH_ObjCreator_obj_GetDataSize
