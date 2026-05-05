.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_536_ctor
.dword dll_536_dtor

# export table
/*0*/ .dword dll_536_setup
/*1*/ .dword dll_536_control
/*2*/ .dword dll_536_update
/*3*/ .dword dll_536_print
/*4*/ .dword dll_536_free
/*5*/ .dword dll_536_get_model_flags
/*6*/ .dword dll_536_get_data_size
