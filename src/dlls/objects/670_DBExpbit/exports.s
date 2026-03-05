.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_670_ctor
.dword dll_670_dtor

# export table
/*0*/ .dword dll_670_setup
/*1*/ .dword dll_670_control
/*2*/ .dword dll_670_update
/*3*/ .dword dll_670_print
/*4*/ .dword dll_670_free
/*5*/ .dword dll_670_get_model_flags
/*6*/ .dword dll_670_get_data_size
