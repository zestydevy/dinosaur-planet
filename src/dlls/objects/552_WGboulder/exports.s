.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WGboulder_ctor
.dword WGboulder_dtor

# export table
/*0*/ .dword WGboulder_setup
/*1*/ .dword WGboulder_control
/*2*/ .dword WGboulder_update
/*3*/ .dword WGboulder_print
/*4*/ .dword WGboulder_free
/*5*/ .dword WGboulder_get_model_flags
/*6*/ .dword WGboulder_get_data_size
