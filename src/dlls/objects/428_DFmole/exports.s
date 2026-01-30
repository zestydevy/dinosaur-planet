.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_428_ctor
.dword dll_428_dtor

# export table
/*0*/ .dword dll_428_setup
/*1*/ .dword dll_428_control
/*2*/ .dword dll_428_update
/*3*/ .dword dll_428_print
/*4*/ .dword dll_428_free
/*5*/ .dword dll_428_get_model_flags
/*6*/ .dword dll_428_get_data_size
