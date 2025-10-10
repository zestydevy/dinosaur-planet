.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword trigger_ctor
.dword trigger_dtor

# export table
.dword trigger_create
.dword trigger_update
.dword trigger_func_7D4
.dword trigger_draw
.dword trigger_destroy
.dword trigger_get_model_flags
.dword trigger_get_state_size
