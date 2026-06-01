.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_234_ctor
.dword dll_234_dtor

# export table
/*0*/ .dword dll_234_setup
/*1*/ .dword dll_234_control
/*2*/ .dword dll_234_update
/*3*/ .dword dll_234_print
/*4*/ .dword dll_234_free
/*5*/ .dword dll_234_get_model_flags
/*6*/ .dword dll_234_get_data_size
/*7*/ .dword dll_234_func_A8C
