.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword medium_crate_ctor
.dword medium_crate_dtor

# export table
.dword medium_crate_setup
.dword medium_crate_control
.dword medium_crate_update
.dword medium_crate_print
.dword medium_crate_free
.dword medium_crate_get_model_flags
.dword medium_crate_get_data_size
