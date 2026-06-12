.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_304_ctor
.dword dll_304_dtor

# export table
/*0*/ .dword dll_304_setup
/*1*/ .dword dll_304_control
/*2*/ .dword dll_304_update
/*3*/ .dword dll_304_print
/*4*/ .dword dll_304_free
/*5*/ .dword dll_304_get_model_flags
/*6*/ .dword dll_304_get_data_size
/*7*/ .dword dll_304_func_BE0
/*8*/ .dword dll_304_func_C3C
/*9*/ .dword dll_304_func_E54
