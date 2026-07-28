.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SnowWormSmall_ctor
.dword SnowWormSmall_dtor

# export table
/*0*/ .dword SnowWormSmall_obj_Setup
/*1*/ .dword SnowWormSmall_obj_Control
/*2*/ .dword SnowWormSmall_obj_Update
/*3*/ .dword SnowWormSmall_obj_Print
/*4*/ .dword SnowWormSmall_obj_Free
/*5*/ .dword SnowWormSmall_obj_GetModelFlags
/*6*/ .dword SnowWormSmall_obj_GetDataSize
/*7*/ .dword SnowWormSmall_GetAnimState
/*8*/ .dword SnowWormSmall_ReceiveMessage
