.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_535_ctor
.dword dll_535_dtor

# export table
/*0*/ .dword dll_535_setup
/*1*/ .dword dll_535_control
/*2*/ .dword dll_535_update
/*3*/ .dword dll_535_print
/*4*/ .dword dll_535_free
/*5*/ .dword dll_535_get_model_flags
/*6*/ .dword dll_535_get_data_size
/*7*/ .dword dll_535_func_278
