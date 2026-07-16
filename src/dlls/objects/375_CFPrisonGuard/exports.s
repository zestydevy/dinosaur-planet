.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_375_ctor
.dword dll_375_dtor

# export table
/*0*/ .dword dll_375_setup
/*1*/ .dword dll_375_control
/*2*/ .dword dll_375_update
/*3*/ .dword dll_375_print
/*4*/ .dword dll_375_free
/*5*/ .dword dll_375_get_model_flags
/*6*/ .dword dll_375_get_data_size
