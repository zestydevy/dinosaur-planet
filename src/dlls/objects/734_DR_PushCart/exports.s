.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_734_ctor
.dword dll_734_dtor

# export table
/*0*/ .dword dll_734_setup
/*1*/ .dword dll_734_control
/*2*/ .dword dll_734_update
/*3*/ .dword dll_734_print
/*4*/ .dword dll_734_free
/*5*/ .dword dll_734_get_model_flags
/*6*/ .dword dll_734_get_data_size
/*7*/ .dword dll_734_func_C34
