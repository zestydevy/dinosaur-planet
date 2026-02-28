.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_796_ctor
.dword dll_796_dtor

# export table
/*0*/ .dword dll_796_setup
/*1*/ .dword dll_796_control
/*2*/ .dword dll_796_update
/*3*/ .dword dll_796_print
/*4*/ .dword dll_796_free
/*5*/ .dword dll_796_get_model_flags
/*6*/ .dword dll_796_get_data_size
