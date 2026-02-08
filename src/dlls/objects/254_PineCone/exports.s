.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_254_ctor
.dword dll_254_dtor

# export table
/*0*/ .dword dll_254_setup
/*1*/ .dword dll_254_control
/*2*/ .dword dll_254_update
/*3*/ .dword dll_254_print
/*4*/ .dword dll_254_free
/*5*/ .dword dll_254_get_model_flags
/*6*/ .dword dll_254_get_data_size
