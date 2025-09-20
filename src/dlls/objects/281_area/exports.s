.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword area_ctor
.dword area_dtor

# export table
.dword area_create
.dword area_update
.dword area_func_3C
.dword area_draw
.dword area_destroy
.dword area_func_70
.dword area_get_state_size
