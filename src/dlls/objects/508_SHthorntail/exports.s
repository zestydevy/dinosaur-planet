.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_508_ctor
.dword dll_508_dtor

# export table
/*0*/ .dword dll_508_setup
/*1*/ .dword dll_508_control
/*2*/ .dword dll_508_update
/*3*/ .dword dll_508_print
/*4*/ .dword dll_508_free
/*5*/ .dword dll_508_get_model_flags
/*6*/ .dword dll_508_get_data_size
