.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_688_ctor
.dword dll_688_dtor

# export table
/*0*/ .dword dll_688_setup
/*1*/ .dword dll_688_control
/*2*/ .dword dll_688_update
/*3*/ .dword dll_688_print
/*4*/ .dword dll_688_free
/*5*/ .dword dll_688_get_model_flags
/*6*/ .dword dll_688_get_data_size
/*7*/ .dword dll_688_func_6DC
/*8*/ .dword dll_688_func_6F0
