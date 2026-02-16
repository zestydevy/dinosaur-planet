.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_412_ctor
.dword dll_412_dtor

# export table
/*0*/ .dword dll_412_setup
/*1*/ .dword dll_412_control
/*2*/ .dword dll_412_update
/*3*/ .dword dll_412_print
/*4*/ .dword dll_412_free
/*5*/ .dword dll_412_get_model_flags
/*6*/ .dword dll_412_get_data_size
