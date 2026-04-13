.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_327_ctor
.dword dll_327_dtor

# export table
/*0*/ .dword dll_327_setup
/*1*/ .dword dll_327_control
/*2*/ .dword dll_327_update
/*3*/ .dword dll_327_print
/*4*/ .dword dll_327_free
/*5*/ .dword dll_327_get_model_flags
/*6*/ .dword dll_327_get_data_size
