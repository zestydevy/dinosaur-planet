.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_LevelControl_ctor
.dword WL_LevelControl_dtor

# export table
.dword WL_LevelControl_setup
.dword WL_LevelControl_control
.dword WL_LevelControl_update
.dword WL_LevelControl_print
.dword WL_LevelControl_free
.dword WL_LevelControl_get_model_flags
.dword WL_LevelControl_get_data_size
