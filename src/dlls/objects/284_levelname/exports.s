.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword levelname_ctor
.dword levelname_dtor

# export table
.dword levelname_setup
.dword levelname_control
.dword levelname_update
.dword levelname_print
.dword levelname_free
.dword levelname_get_model_flags
.dword levelname_get_data_size
