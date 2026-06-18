.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_421_ctor
.dword dll_421_dtor

# export table
/*0*/ .dword dll_421_setup
/*1*/ .dword dll_421_control
/*2*/ .dword dll_421_update
/*3*/ .dword dll_421_print
/*4*/ .dword dll_421_free
/*5*/ .dword dll_421_get_model_flags
/*6*/ .dword dll_421_get_data_size
