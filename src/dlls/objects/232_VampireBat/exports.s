.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_232_ctor
.dword dll_232_dtor

# export table
/*0*/ .dword dll_232_setup
/*1*/ .dword dll_232_control
/*2*/ .dword dll_232_update
/*3*/ .dword dll_232_print
/*4*/ .dword dll_232_free
/*5*/ .dword dll_232_get_model_flags
/*6*/ .dword dll_232_get_data_size
