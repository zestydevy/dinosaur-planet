.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_481_ctor
.dword dll_481_dtor

# export table
/*0*/ .dword dll_481_setup
/*1*/ .dword dll_481_control
/*2*/ .dword dll_481_update
/*3*/ .dword dll_481_print
/*4*/ .dword dll_481_free
/*5*/ .dword dll_481_get_model_flags
/*6*/ .dword dll_481_get_data_size
