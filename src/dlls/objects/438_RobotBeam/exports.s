.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_438_ctor
.dword dll_438_dtor

# export table
/*0*/ .dword dll_438_setup
/*1*/ .dword dll_438_control
/*2*/ .dword dll_438_update
/*3*/ .dword dll_438_print
/*4*/ .dword dll_438_free
/*5*/ .dword dll_438_get_model_flags
/*6*/ .dword dll_438_get_data_size
