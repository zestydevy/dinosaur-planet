.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_786_ctor
.dword dll_786_dtor

# export table
/*0*/ .dword dll_786_setup
/*1*/ .dword dll_786_control
/*2*/ .dword dll_786_update
/*3*/ .dword dll_786_print
/*4*/ .dword dll_786_free
/*5*/ .dword dll_786_get_model_flags
/*6*/ .dword dll_786_get_data_size
