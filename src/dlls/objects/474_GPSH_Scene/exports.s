.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GPSH_Scene_ctor
.dword GPSH_Scene_dtor

# export table
/*0*/ .dword GPSH_Scene_obj_Setup
/*1*/ .dword GPSH_Scene_obj_Control
/*2*/ .dword GPSH_Scene_obj_Update
/*3*/ .dword GPSH_Scene_obj_Print
/*4*/ .dword GPSH_Scene_obj_Free
/*5*/ .dword GPSH_Scene_obj_GetModelFlags
/*6*/ .dword GPSH_Scene_obj_GetDataSize
