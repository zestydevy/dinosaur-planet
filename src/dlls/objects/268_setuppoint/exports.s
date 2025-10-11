.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword setuppoint_ctor
.dword setuppoint_dtor

# export table
.dword setuppoint_setup
.dword setuppoint_control
.dword setuppoint_update
.dword setuppoint_print
.dword setuppoint_free
.dword setuppoint_get_model_flags
.dword setuppoint_get_state_size
