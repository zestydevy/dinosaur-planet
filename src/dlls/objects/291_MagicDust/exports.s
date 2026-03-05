.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_291_ctor
.dword dll_291_dtor

# export table
/*0*/ .dword dll_291_setup
/*1*/ .dword dll_291_control
/*2*/ .dword dll_291_update
/*3*/ .dword dll_291_print
/*4*/ .dword dll_291_free
/*5*/ .dword dll_291_get_model_flags
/*6*/ .dword dll_291_get_data_size
