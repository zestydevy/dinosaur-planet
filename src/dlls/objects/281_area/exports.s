.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword area_ctor
.dword area_dtor

# export table
.dword area_setup
.dword area_control
.dword area_update
.dword area_print
.dword area_free
.dword area_get_model_flags
.dword area_get_state_size
