.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_513_ctor
.dword dll_513_dtor

# export table
/*0*/ .dword dll_513_setup
/*1*/ .dword dll_513_control
/*2*/ .dword dll_513_update
/*3*/ .dword dll_513_print
/*4*/ .dword dll_513_free
/*5*/ .dword dll_513_get_model_flags
/*6*/ .dword dll_513_get_data_size
