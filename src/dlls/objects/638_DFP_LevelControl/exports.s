.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFP_LevelControl_ctor
.dword DFP_LevelControl_dtor

# export table
.dword DFP_LevelControl_obj_Setup
.dword DFP_LevelControl_obj_Control
.dword DFP_LevelControl_obj_Update
.dword DFP_LevelControl_obj_Print
.dword DFP_LevelControl_obj_Free
.dword DFP_LevelControl_obj_GetModelFlags
.dword DFP_LevelControl_obj_GetDataSize
.dword DFP_LevelControl_GetSafeFloorTiles
