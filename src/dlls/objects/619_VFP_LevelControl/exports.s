.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_LevelControl_ctor
.dword VFP_LevelControl_dtor

# export table
.dword VFP_LevelControl_setup
.dword VFP_LevelControl_control
.dword VFP_LevelControl_update
.dword VFP_LevelControl_print
.dword VFP_LevelControl_free
.dword VFP_LevelControl_get_model_flags
.dword VFP_LevelControl_get_data_size
