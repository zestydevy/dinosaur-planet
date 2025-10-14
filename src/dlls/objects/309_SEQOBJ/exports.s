.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_309_ctor
.dword dll_309_dtor

# export table
/*0*/ .dword dll_309_setup
/*1*/ .dword dll_309_control
/*2*/ .dword dll_309_update
/*3*/ .dword dll_309_print
/*4*/ .dword dll_309_free
/*5*/ .dword dll_309_get_model_flags
/*6*/ .dword dll_309_get_data_size
