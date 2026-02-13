.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_294_ctor
.dword dll_294_dtor

# export table
/*0*/ .dword dll_294_setup
/*1*/ .dword dll_294_control
/*2*/ .dword dll_294_update
/*3*/ .dword dll_294_print
/*4*/ .dword dll_294_free
/*5*/ .dword dll_294_get_model_flags
/*6*/ .dword dll_294_get_data_size
