.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_503_ctor
.dword dll_503_dtor

# export table
/*0*/ .dword dll_503_setup
/*1*/ .dword dll_503_control
/*2*/ .dword dll_503_update
/*3*/ .dword dll_503_print
/*4*/ .dword dll_503_free
/*5*/ .dword dll_503_get_model_flags
/*6*/ .dword dll_503_get_data_size
