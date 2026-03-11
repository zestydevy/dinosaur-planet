.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_252_ctor
.dword dll_252_dtor

# export table
/*0*/ .dword dll_252_setup
/*1*/ .dword dll_252_control
/*2*/ .dword dll_252_update
/*3*/ .dword dll_252_print
/*4*/ .dword dll_252_free
/*5*/ .dword dll_252_get_model_flags
/*6*/ .dword dll_252_get_data_size
