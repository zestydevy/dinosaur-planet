.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_457_ctor
.dword dll_457_dtor

# export table
/*0*/ .dword dll_457_setup
/*1*/ .dword dll_457_control
/*2*/ .dword dll_457_update
/*3*/ .dword dll_457_print
/*4*/ .dword dll_457_free
/*5*/ .dword dll_457_get_model_flags
/*6*/ .dword dll_457_get_data_size
