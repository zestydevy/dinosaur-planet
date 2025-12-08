.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_697_ctor
.dword dll_697_dtor

# export table
/*0*/ .dword dll_697_setup
/*1*/ .dword dll_697_control
/*2*/ .dword dll_697_update
/*3*/ .dword dll_697_print
/*4*/ .dword dll_697_free
/*5*/ .dword dll_697_get_model_flags
/*6*/ .dword dll_697_get_data_size
