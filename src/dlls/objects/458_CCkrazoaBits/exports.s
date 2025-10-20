.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCkrazoaBits_ctor
.dword CCkrazoaBits_dtor

# export table
.dword CCkrazoaBits_setup
.dword CCkrazoaBits_control
.dword CCkrazoaBits_update
.dword CCkrazoaBits_print
.dword CCkrazoaBits_free
.dword CCkrazoaBits_get_model_flags
.dword CCkrazoaBits_get_data_size
