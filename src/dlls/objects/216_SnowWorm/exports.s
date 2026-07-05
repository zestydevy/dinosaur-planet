.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_216_ctor
.dword dll_216_dtor

# export table
/*0*/ .dword dll_216_setup
/*1*/ .dword dll_216_control
/*2*/ .dword dll_216_update
/*3*/ .dword dll_216_print
/*4*/ .dword dll_216_free
/*5*/ .dword dll_216_get_model_flags
/*6*/ .dword dll_216_get_data_size
/*7*/ .dword dll_216_func_87C
/*8*/ .dword dll_216_func_88C
