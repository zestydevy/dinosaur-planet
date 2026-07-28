.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SnowWorm_ctor
.dword SnowWorm_dtor

# export table
/*0*/ .dword SnowWorm_obj_Setup
/*1*/ .dword SnowWorm_obj_Control
/*2*/ .dword SnowWorm_obj_Update
/*3*/ .dword SnowWorm_obj_Print
/*4*/ .dword SnowWorm_obj_Free
/*5*/ .dword SnowWorm_obj_GetModelFlags
/*6*/ .dword SnowWorm_obj_GetDataSize
/*7*/ .dword SnowWorm_GetAnimState
/*8*/ .dword SnowWorm_ReceiveMessage
