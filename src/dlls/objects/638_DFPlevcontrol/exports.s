.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFP_LevelControl_ctor
.dword DFP_LevelControl_dtor

# export table
.dword DFP_LevelControl_setup
.dword DFP_LevelControl_control
.dword DFP_LevelControl_update
.dword DFP_LevelControl_print
.dword DFP_LevelControl_free
.dword DFP_LevelControl_get_model_flags
.dword DFP_LevelControl_get_data_size
.dword DFP_LevelControl_export_safe_floor_tiles
