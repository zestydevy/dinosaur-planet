.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_477_ctor
.dword dll_477_dtor

# export table
/*0*/ .dword dll_477_setup
/*1*/ .dword dll_477_control
/*2*/ .dword dll_477_update
/*3*/ .dword dll_477_print
/*4*/ .dword dll_477_free
/*5*/ .dword dll_477_get_model_flags
/*6*/ .dword dll_477_get_data_size
