.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCgasventControl_ctor
.dword CCgasventControl_dtor

# export table
.dword CCgasventControl_setup
.dword CCgasventControl_control
.dword CCgasventControl_update
.dword CCgasventControl_print
.dword CCgasventControl_free
.dword CCgasventControl_get_model_flags
.dword CCgasventControl_get_data_size
