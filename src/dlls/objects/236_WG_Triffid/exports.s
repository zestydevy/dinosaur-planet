.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_236_ctor
.dword dll_236_dtor

# export table
/*0*/ .dword dll_236_setup
/*1*/ .dword dll_236_control
/*2*/ .dword dll_236_update
/*3*/ .dword dll_236_print
/*4*/ .dword dll_236_free
/*5*/ .dword dll_236_get_model_flags
/*6*/ .dword dll_236_get_data_size
