.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_300_ctor
.dword dll_300_dtor

# export table
/*0*/ .dword dll_300_setup
/*1*/ .dword dll_300_control
/*2*/ .dword dll_300_update
/*3*/ .dword dll_300_print
/*4*/ .dword dll_300_free
/*5*/ .dword dll_300_get_model_flags
/*6*/ .dword dll_300_get_data_size
/*7*/ .dword dll_300_func_884
