.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMIceWall_ctor
.dword DIMIceWall_dtor

# export table
/*0*/ .dword DIMIceWall_obj_Setup
/*1*/ .dword DIMIceWall_obj_Control
/*2*/ .dword DIMIceWall_obj_Update
/*3*/ .dword DIMIceWall_obj_Print
/*4*/ .dword DIMIceWall_obj_Free
/*5*/ .dword DIMIceWall_obj_GetModelFlags
/*6*/ .dword DIMIceWall_obj_GetDataSize
/*7*/ .dword DIMIceWall_TickFlame
