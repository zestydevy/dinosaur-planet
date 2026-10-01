.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WaterBaddie_ctor
.dword WaterBaddie_dtor

# export table
/*0*/ .dword WaterBaddie_obj_Setup
/*1*/ .dword WaterBaddie_obj_Control
/*2*/ .dword WaterBaddie_obj_Update
/*3*/ .dword WaterBaddie_obj_Print
/*4*/ .dword WaterBaddie_obj_Free
/*5*/ .dword WaterBaddie_obj_GetModelFlags
/*6*/ .dword WaterBaddie_obj_GetDataSize
/*7*/ .dword WaterBaddie_GetAnimState
/*8*/ .dword WaterBaddie_ReceiveMessage
