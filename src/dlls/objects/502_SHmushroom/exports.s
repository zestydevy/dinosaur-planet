.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_502_ctor
.dword dll_502_dtor

# export table
/*0*/ .dword dll_502_setup
/*1*/ .dword dll_502_control
/*2*/ .dword dll_502_update
/*3*/ .dword dll_502_print
/*4*/ .dword dll_502_free
/*5*/ .dword dll_502_get_model_flags
/*6*/ .dword dll_502_get_data_size
