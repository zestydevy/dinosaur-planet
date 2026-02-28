.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_279_ctor
.dword dll_279_dtor

# export table
/*0*/ .dword dll_279_setup
/*1*/ .dword dll_279_control
/*2*/ .dword dll_279_update
/*3*/ .dword dll_279_print
/*4*/ .dword dll_279_free
/*5*/ .dword dll_279_get_model_flags
/*6*/ .dword dll_279_get_data_size
