.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_222_ctor
.dword dll_222_dtor

# export table
/*0*/ .dword dll_222_setup
/*1*/ .dword dll_222_control
/*2*/ .dword dll_222_update
/*3*/ .dword dll_222_print
/*4*/ .dword dll_222_free
/*5*/ .dword dll_222_get_model_flags
/*6*/ .dword dll_222_get_data_size
/*7*/ .dword dll_222_func_7B0
/*8*/ .dword dll_222_func_7C4
