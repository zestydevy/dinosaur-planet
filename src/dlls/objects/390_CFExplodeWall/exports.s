.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFExplodeWall_ctor
.dword CFExplodeWall_dtor

# export table
/*0*/ .dword CFExplodeWall_obj_Setup
/*1*/ .dword CFExplodeWall_obj_Control
/*2*/ .dword CFExplodeWall_obj_Update
/*3*/ .dword CFExplodeWall_obj_Print
/*4*/ .dword CFExplodeWall_obj_Free
/*5*/ .dword CFExplodeWall_obj_GetModelFlags
/*6*/ .dword CFExplodeWall_obj_GetDataSize
