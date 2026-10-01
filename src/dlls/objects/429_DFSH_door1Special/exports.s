.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFSH_Door1Special_ctor
.dword DFSH_Door1Special_dtor

# export table
/*0*/ .dword DFSH_Door1Special_obj_Setup
/*1*/ .dword DFSH_Door1Special_obj_Control
/*2*/ .dword DFSH_Door1Special_obj_Update
/*3*/ .dword DFSH_Door1Special_obj_Print
/*4*/ .dword DFSH_Door1Special_obj_Free
/*5*/ .dword DFSH_Door1Special_obj_GetModelFlags
/*6*/ .dword DFSH_Door1Special_obj_GetDataSize
