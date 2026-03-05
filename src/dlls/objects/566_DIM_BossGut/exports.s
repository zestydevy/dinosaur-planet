.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_566_ctor
.dword dll_566_dtor

# export table
/*0*/ .dword dll_566_setup
/*1*/ .dword dll_566_control
/*2*/ .dword dll_566_update
/*3*/ .dword dll_566_print
/*4*/ .dword dll_566_free
/*5*/ .dword dll_566_get_model_flags
/*6*/ .dword dll_566_get_data_size
