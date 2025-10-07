.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_LevelControl_ctor
.dword WL_LevelControl_dtor

# export table
.dword WL_LevelControl_create
.dword WL_LevelControl_update
.dword WL_LevelControl_func_394
.dword WL_LevelControl_draw
.dword WL_LevelControl_destroy
.dword WL_LevelControl_func_43C
.dword WL_LevelControl_get_state_size
