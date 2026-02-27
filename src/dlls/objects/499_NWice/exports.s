.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_499_ctor
.dword dll_499_dtor

# export table
/*0*/ .dword dll_499_setup
/*1*/ .dword dll_499_control
/*2*/ .dword dll_499_update
/*3*/ .dword dll_499_print
/*4*/ .dword dll_499_free
/*5*/ .dword dll_499_get_model_flags
/*6*/ .dword dll_499_get_data_size
