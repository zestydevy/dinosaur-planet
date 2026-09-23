.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFTreasRobo_ctor
.dword CFTreasRobo_dtor

# export table
/*0*/ .dword CFTreasRobo_obj_Setup
/*1*/ .dword CFTreasRobo_obj_Control
/*2*/ .dword CFTreasRobo_obj_Update
/*3*/ .dword CFTreasRobo_obj_Print
/*4*/ .dword CFTreasRobo_obj_Free
/*5*/ .dword CFTreasRobo_obj_GetModelFlags
/*6*/ .dword CFTreasRobo_obj_GetDataSize
