.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_437_ctor
.dword dll_437_dtor

# export table
/*0*/ .dword dll_437_setup
/*1*/ .dword dll_437_control
/*2*/ .dword dll_437_update
/*3*/ .dword dll_437_print
/*4*/ .dword dll_437_free
/*5*/ .dword dll_437_get_model_flags
/*6*/ .dword dll_437_get_data_size
/*7*/ .dword dll_437_func_3AE4
/*8*/ .dword dll_437_func_3AF8
