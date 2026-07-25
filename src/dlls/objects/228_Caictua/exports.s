.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_228_ctor
.dword dll_228_dtor

# export table
/*0*/ .dword dll_228_setup
/*1*/ .dword dll_228_control
/*2*/ .dword dll_228_update
/*3*/ .dword dll_228_print
/*4*/ .dword dll_228_free
/*5*/ .dword dll_228_get_model_flags
/*6*/ .dword dll_228_get_data_size
/*7*/ .dword dll_228_func_5D4
/*8*/ .dword dll_228_func_5E8
