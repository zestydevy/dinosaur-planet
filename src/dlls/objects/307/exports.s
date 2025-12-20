.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_307_ctor
.dword dll_307_dtor

# export table
/*0*/ .dword dll_307_setup
/*1*/ .dword dll_307_control
/*2*/ .dword dll_307_update
/*3*/ .dword dll_307_print
/*4*/ .dword dll_307_free
/*5*/ .dword dll_307_get_model_flags
/*6*/ .dword dll_307_get_data_size
