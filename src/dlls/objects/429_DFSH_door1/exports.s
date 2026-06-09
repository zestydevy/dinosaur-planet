.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_429_ctor
.dword dll_429_dtor

# export table
/*0*/ .dword dll_429_setup
/*1*/ .dword dll_429_control
/*2*/ .dword dll_429_update
/*3*/ .dword dll_429_print
/*4*/ .dword dll_429_free
/*5*/ .dword dll_429_get_model_flags
/*6*/ .dword dll_429_get_data_size
