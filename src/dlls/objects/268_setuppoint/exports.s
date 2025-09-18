.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword setuppoint_ctor
.dword setuppoint_dtor

# export table
.dword setuppoint_create
.dword setuppoint_update
.dword setuppoint_func_64
.dword setuppoint_draw
.dword setuppoint_destroy
.dword setuppoint_func_D4
.dword setuppoint_get_state_size
