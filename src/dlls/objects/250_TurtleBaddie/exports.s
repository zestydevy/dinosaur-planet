.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_250_ctor
.dword dll_250_dtor

# export table
/*0*/ .dword dll_250_setup
/*1*/ .dword dll_250_control
/*2*/ .dword dll_250_update
/*3*/ .dword dll_250_print
/*4*/ .dword dll_250_free
/*5*/ .dword dll_250_get_model_flags
/*6*/ .dword dll_250_get_data_size
