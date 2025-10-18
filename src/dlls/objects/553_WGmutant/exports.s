.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WGmutant_ctor
.dword WGmutant_dtor

# export table
.dword WGmutant_setup
.dword WGmutant_control
.dword WGmutant_update
.dword WGmutant_print
.dword WGmutant_free
.dword WGmutant_get_model_flags
.dword WGmutant_get_data_size
