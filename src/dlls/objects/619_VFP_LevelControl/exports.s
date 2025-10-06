.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_LevelControl_ctor
.dword VFP_LevelControl_dtor

# export table
.dword VFP_LevelControl_create
.dword VFP_LevelControl_update
.dword VFP_LevelControl_func_864
.dword VFP_LevelControl_draw
.dword VFP_LevelControl_destroy
.dword VFP_LevelControl_func_8C8
.dword VFP_LevelControl_get_state_size
