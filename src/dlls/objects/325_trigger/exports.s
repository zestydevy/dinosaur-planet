.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword trigger_ctor
.dword trigger_dtor

# export table
.dword trigger_setup
.dword trigger_control
.dword trigger_update
.dword trigger_print
.dword trigger_free
.dword trigger_get_model_flags
.dword trigger_get_state_size
