.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_336_ctor
.dword dll_336_dtor

# export table
/*0*/ .dword dll_336_setup
/*1*/ .dword dll_336_control
/*2*/ .dword dll_336_update
/*3*/ .dword dll_336_print
/*4*/ .dword dll_336_free
/*5*/ .dword dll_336_get_model_flags
/*6*/ .dword dll_336_get_data_size
