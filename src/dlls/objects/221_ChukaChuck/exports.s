.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_221_ctor
.dword dll_221_dtor

# export table
/*0*/ .dword dll_221_setup
/*1*/ .dword dll_221_control
/*2*/ .dword dll_221_update
/*3*/ .dword dll_221_print
/*4*/ .dword dll_221_free
/*5*/ .dword dll_221_get_model_flags
/*6*/ .dword dll_221_get_data_size
