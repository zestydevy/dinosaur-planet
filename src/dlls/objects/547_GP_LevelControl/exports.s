.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GP_LevelControl_ctor
.dword GP_LevelControl_dtor

# export table
.dword GP_LevelControl_create
.dword GP_LevelControl_update
.dword GP_LevelControl_func_434
.dword GP_LevelControl_draw
.dword GP_LevelControl_destroy
.dword GP_LevelControl_get_setup_flags
.dword GP_LevelControl_get_state_size
