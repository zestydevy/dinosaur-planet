.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_376_ctor
.dword dll_376_dtor

# export table
/*0*/ .dword dll_376_setup
/*1*/ .dword dll_376_control
/*2*/ .dword dll_376_update
/*3*/ .dword dll_376_print
/*4*/ .dword dll_376_free
/*5*/ .dword dll_376_get_model_flags
/*6*/ .dword dll_376_get_data_size
