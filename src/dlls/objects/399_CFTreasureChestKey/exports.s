.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFTreasureChestKey_ctor
.dword CFTreasureChestKey_dtor

# export table
/*0*/ .dword CFTreasureChestKey_obj_Setup
/*1*/ .dword CFTreasureChestKey_obj_Control
/*2*/ .dword CFTreasureChestKey_obj_Update
/*3*/ .dword CFTreasureChestKey_obj_Print
/*4*/ .dword CFTreasureChestKey_obj_Free
/*5*/ .dword CFTreasureChestKey_obj_GetModelFlags
/*6*/ .dword CFTreasureChestKey_obj_GetDataSize
