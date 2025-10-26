.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_703_ctor
.dword dll_703_dtor

# export table
/*0*/ .dword dll_703_setup
/*1*/ .dword dll_703_control
/*2*/ .dword dll_703_update
/*3*/ .dword dll_703_print
/*4*/ .dword dll_703_free
/*5*/ .dword dll_703_get_model_flags
/*6*/ .dword dll_703_get_data_size
