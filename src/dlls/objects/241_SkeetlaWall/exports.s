.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_241_ctor
.dword dll_241_dtor

# export table
/*0*/ .dword dll_241_setup
/*1*/ .dword dll_241_control
/*2*/ .dword dll_241_update
/*3*/ .dword dll_241_print
/*4*/ .dword dll_241_free
/*5*/ .dword dll_241_get_model_flags
/*6*/ .dword dll_241_get_data_size
/*7*/ .dword dll_241_func_10C
