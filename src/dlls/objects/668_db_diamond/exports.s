.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBdiamond_ctor
.dword DBdiamond_dtor

# export table
.dword DBdiamond_setup
.dword DBdiamond_control
.dword DBdiamond_update
.dword DBdiamond_print
.dword DBdiamond_free
.dword DBdiamond_get_model_flags
.dword DBdiamond_get_data_size
