.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_509_ctor
.dword dll_509_dtor

# export table
/*0*/ .dword dll_509_setup
/*1*/ .dword dll_509_control
/*2*/ .dword dll_509_update
/*3*/ .dword dll_509_print
/*4*/ .dword dll_509_free
/*5*/ .dword dll_509_get_model_flags
/*6*/ .dword dll_509_get_data_size
