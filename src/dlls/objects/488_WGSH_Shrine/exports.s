.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_488_ctor
.dword dll_488_dtor

# export table
/*0*/ .dword dll_488_setup
/*1*/ .dword dll_488_control
/*2*/ .dword dll_488_update
/*3*/ .dword dll_488_print
/*4*/ .dword dll_488_free
/*5*/ .dword dll_488_get_model_flags
/*6*/ .dword dll_488_get_data_size
