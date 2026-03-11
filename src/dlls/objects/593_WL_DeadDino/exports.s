.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_593_ctor
.dword dll_593_dtor

# export table
/*0*/ .dword dll_593_setup
/*1*/ .dword dll_593_control
/*2*/ .dword dll_593_update
/*3*/ .dword dll_593_print
/*4*/ .dword dll_593_free
/*5*/ .dword dll_593_get_model_flags
/*6*/ .dword dll_593_get_data_size
