.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFdockpoint_ctor
.dword DFdockpoint_dtor

# export table
.dword DFdockpoint_setup
.dword DFdockpoint_control
.dword DFdockpoint_update
.dword DFdockpoint_print
.dword DFdockpoint_free
.dword DFdockpoint_get_model_flags
.dword DFdockpoint_get_data_size
