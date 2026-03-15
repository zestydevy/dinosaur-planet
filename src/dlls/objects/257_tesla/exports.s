.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_257_ctor
.dword dll_257_dtor

# export table
/*0*/ .dword dll_257_setup
/*1*/ .dword dll_257_control
/*2*/ .dword dll_257_update
/*3*/ .dword dll_257_print
/*4*/ .dword dll_257_free
/*5*/ .dword dll_257_get_model_flags
/*6*/ .dword dll_257_get_data_size
/*7*/ .dword dll_257_func_11C8
