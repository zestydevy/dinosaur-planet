.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCgasvent_ctor
.dword CCgasvent_dtor

# export table
.dword CCgasvent_setup
.dword CCgasvent_control
.dword CCgasvent_update
.dword CCgasvent_print
.dword CCgasvent_free
.dword CCgasvent_get_model_flags
.dword CCgasvent_get_data_size
