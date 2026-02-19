.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_455_ctor
.dword dll_455_dtor

# export table
/*0*/ .dword dll_455_setup
/*1*/ .dword dll_455_control
/*2*/ .dword dll_455_update
/*3*/ .dword dll_455_print
/*4*/ .dword dll_455_free
/*5*/ .dword dll_455_get_model_flags
/*6*/ .dword dll_455_get_data_size
