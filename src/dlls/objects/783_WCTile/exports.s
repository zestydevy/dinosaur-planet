.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_783_ctor
.dword dll_783_dtor

# export table
/*0*/ .dword dll_783_setup
/*1*/ .dword dll_783_control
/*2*/ .dword dll_783_update
/*3*/ .dword dll_783_print
/*4*/ .dword dll_783_free
/*5*/ .dword dll_783_get_model_flags
/*6*/ .dword dll_783_get_data_size
