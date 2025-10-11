.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GP_LevelControl_ctor
.dword GP_LevelControl_dtor

# export table
.dword GP_LevelControl_setup
.dword GP_LevelControl_control
.dword GP_LevelControl_update
.dword GP_LevelControl_print
.dword GP_LevelControl_free
.dword GP_LevelControl_get_model_flags
.dword GP_LevelControl_get_state_size
