.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword animator_ctor
.dword animator_dtor

# export table
.dword animator_setup
.dword animator_control
.dword animator_update
.dword animator_print
.dword animator_free
.dword animator_get_model_flags
.dword animator_get_data_size
