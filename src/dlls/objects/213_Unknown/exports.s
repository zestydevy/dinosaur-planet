.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_213_ctor
.dword dll_213_dtor

# export table
/*0*/ .dword dll_213_setup
/*1*/ .dword dll_213_control
/*2*/ .dword dll_213_update
/*3*/ .dword dll_213_print
/*4*/ .dword dll_213_free
/*5*/ .dword dll_213_get_model_flags
/*6*/ .dword dll_213_get_data_size
