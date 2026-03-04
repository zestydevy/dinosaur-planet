.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_299_ctor
.dword dll_299_dtor

# export table
/*0*/ .dword dll_299_setup
/*1*/ .dword dll_299_control
/*2*/ .dword dll_299_update
/*3*/ .dword dll_299_print
/*4*/ .dword dll_299_free
/*5*/ .dword dll_299_get_model_flags
/*6*/ .dword dll_299_get_data_size
