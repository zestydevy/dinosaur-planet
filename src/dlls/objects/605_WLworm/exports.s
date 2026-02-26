.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_605_ctor
.dword dll_605_dtor

# export table
/*0*/ .dword dll_605_setup
/*1*/ .dword dll_605_control
/*2*/ .dword dll_605_update
/*3*/ .dword dll_605_print
/*4*/ .dword dll_605_free
/*5*/ .dword dll_605_get_model_flags
/*6*/ .dword dll_605_get_data_size
