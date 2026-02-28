.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_246_ctor
.dword dll_246_dtor

# export table
/*0*/ .dword dll_246_setup
/*1*/ .dword dll_246_control
/*2*/ .dword dll_246_update
/*3*/ .dword dll_246_print
/*4*/ .dword dll_246_free
/*5*/ .dword dll_246_get_model_flags
/*6*/ .dword dll_246_get_data_size
