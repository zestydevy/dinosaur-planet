.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_722_ctor
.dword dll_722_dtor

# export table
/*0*/ .dword dll_722_setup
/*1*/ .dword dll_722_control
/*2*/ .dword dll_722_update
/*3*/ .dword dll_722_print
/*4*/ .dword dll_722_free
/*5*/ .dword dll_722_get_model_flags
/*6*/ .dword dll_722_get_data_size
/*7*/ .dword dll_722_func_BEC
/*8*/ .dword dll_722_func_D00
/*9*/ .dword dll_722_func_D18
/*10*/ .dword dll_722_func_D30
