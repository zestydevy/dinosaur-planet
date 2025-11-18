.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_339_ctor
.dword dll_339_dtor

# export table
/*0*/ .dword dll_339_setup
/*1*/ .dword dll_339_control
/*2*/ .dword dll_339_update
/*3*/ .dword dll_339_print
/*4*/ .dword dll_339_free
/*5*/ .dword dll_339_get_model_flags
/*6*/ .dword dll_339_get_data_size
