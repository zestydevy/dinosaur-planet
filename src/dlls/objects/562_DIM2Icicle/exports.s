.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_562_ctor
.dword dll_562_dtor

# export table
/*0*/ .dword dll_562_setup
/*1*/ .dword dll_562_control
/*2*/ .dword dll_562_update
/*3*/ .dword dll_562_print
/*4*/ .dword dll_562_free
/*5*/ .dword dll_562_get_model_flags
/*6*/ .dword dll_562_get_data_size
