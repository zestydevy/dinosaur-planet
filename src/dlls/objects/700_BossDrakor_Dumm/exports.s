.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_700_ctor
.dword dll_700_dtor

# export table
/*0*/ .dword dll_700_setup
/*1*/ .dword dll_700_control
/*2*/ .dword dll_700_update
/*3*/ .dword dll_700_print
/*4*/ .dword dll_700_free
/*5*/ .dword dll_700_get_model_flags
/*6*/ .dword dll_700_get_data_size
