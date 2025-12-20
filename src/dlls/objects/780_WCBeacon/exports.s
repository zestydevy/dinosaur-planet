.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_780_ctor
.dword dll_780_dtor

# export table
/*0*/ .dword dll_780_setup
/*1*/ .dword dll_780_control
/*2*/ .dword dll_780_update
/*3*/ .dword dll_780_print
/*4*/ .dword dll_780_free
/*5*/ .dword dll_780_get_model_flags
/*6*/ .dword dll_780_get_data_size
