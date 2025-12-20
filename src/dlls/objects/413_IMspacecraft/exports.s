.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_413_ctor
.dword dll_413_dtor

# export table
/*0*/ .dword dll_413_setup
/*1*/ .dword dll_413_control
/*2*/ .dword dll_413_update
/*3*/ .dword dll_413_print
/*4*/ .dword dll_413_free
/*5*/ .dword dll_413_get_model_flags
/*6*/ .dword dll_413_get_data_size
/*7*/ .dword dll_413_func_470
/*8*/ .dword dll_413_func_4A4
/*9*/ .dword dll_413_func_4BC
