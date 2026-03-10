.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_320_ctor
.dword dll_320_dtor

# export table
/*0*/ .dword dll_320_setup
/*1*/ .dword dll_320_control
/*2*/ .dword dll_320_update
/*3*/ .dword dll_320_print
/*4*/ .dword dll_320_free
/*5*/ .dword dll_320_get_model_flags
/*6*/ .dword dll_320_get_data_size
