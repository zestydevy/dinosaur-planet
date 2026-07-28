.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFTreasureDoor_ctor
.dword CFTreasureDoor_dtor

# export table
/*0*/ .dword CFTreasureDoor_obj_Setup
/*1*/ .dword CFTreasureDoor_obj_Control
/*2*/ .dword CFTreasureDoor_obj_Update
/*3*/ .dword CFTreasureDoor_obj_Print
/*4*/ .dword CFTreasureDoor_obj_Free
/*5*/ .dword CFTreasureDoor_obj_GetModelFlags
/*6*/ .dword CFTreasureDoor_obj_GetDataSize
