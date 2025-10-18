.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBlevelControl_ctor
.dword DBlevelControl_dtor

# export table
.dword DBlevelControl_setup
.dword DBlevelControl_control
.dword DBlevelControl_update
.dword DBlevelControl_print
.dword DBlevelControl_free
.dword DBlevelControl_get_model_flags
.dword DBlevelControl_get_data_size
