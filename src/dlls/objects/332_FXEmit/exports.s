.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_332_ctor
.dword dll_332_dtor

# export table
/*0*/ .dword dll_332_setup
/*1*/ .dword dll_332_control
/*2*/ .dword dll_332_update
/*3*/ .dword dll_332_print
/*4*/ .dword dll_332_free
/*5*/ .dword dll_332_get_model_flags
/*6*/ .dword dll_332_get_data_size
