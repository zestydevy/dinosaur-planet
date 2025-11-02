.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_365_ctor
.dword dll_365_dtor

# export table
/*0*/ .dword dll_365_setup
/*1*/ .dword dll_365_control
/*2*/ .dword dll_365_update
/*3*/ .dword dll_365_print
/*4*/ .dword dll_365_free
/*5*/ .dword dll_365_get_model_flags
/*6*/ .dword dll_365_get_data_size
