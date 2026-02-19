.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_776_ctor
.dword dll_776_dtor

# export table
/*0*/ .dword dll_776_setup
/*1*/ .dword dll_776_control
/*2*/ .dword dll_776_update
/*3*/ .dword dll_776_print
/*4*/ .dword dll_776_free
/*5*/ .dword dll_776_get_model_flags
/*6*/ .dword dll_776_get_data_size
