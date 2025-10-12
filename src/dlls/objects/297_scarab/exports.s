.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_297_ctor
.dword dll_297_dtor

# export table
/*0*/ .dword dll_297_setup
/*1*/ .dword dll_297_control
/*2*/ .dword dll_297_update
/*3*/ .dword dll_297_print
/*4*/ .dword dll_297_free
/*5*/ .dword dll_297_get_model_flags
/*6*/ .dword dll_297_get_data_size
