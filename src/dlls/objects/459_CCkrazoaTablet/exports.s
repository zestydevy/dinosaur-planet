.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCkrazoaTablet_ctor
.dword CCkrazoaTablet_dtor

# export table
.dword CCkrazoaTablet_setup
.dword CCkrazoaTablet_control
.dword CCkrazoaTablet_update
.dword CCkrazoaTablet_print
.dword CCkrazoaTablet_free
.dword CCkrazoaTablet_get_model_flags
.dword CCkrazoaTablet_get_data_size
