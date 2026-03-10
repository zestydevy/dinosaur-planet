.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_510_ctor
.dword dll_510_dtor

# export table
/*0*/ .dword dll_510_setup
/*1*/ .dword dll_510_control
/*2*/ .dword dll_510_update
/*3*/ .dword dll_510_print
/*4*/ .dword dll_510_free
/*5*/ .dword dll_510_get_model_flags
/*6*/ .dword dll_510_get_data_size
