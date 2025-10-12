.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Duster_ctor
.dword Duster_dtor

# export table
.dword Duster_setup
.dword Duster_control
.dword Duster_update
.dword Duster_print
.dword Duster_free
.dword Duster_get_model_flags
.dword Duster_get_data_size
