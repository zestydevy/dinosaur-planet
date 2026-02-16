.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_240_ctor
.dword dll_240_dtor

# export table
/*0*/ .dword dll_240_setup
/*1*/ .dword dll_240_control
/*2*/ .dword dll_240_update
/*3*/ .dword dll_240_print
/*4*/ .dword dll_240_free
/*5*/ .dword dll_240_get_model_flags
/*6*/ .dword dll_240_get_data_size
