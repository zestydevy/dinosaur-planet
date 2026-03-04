.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_220_ctor
.dword dll_220_dtor

# export table
/*0*/ .dword dll_220_setup
/*1*/ .dword dll_220_control
/*2*/ .dword dll_220_update
/*3*/ .dword dll_220_print
/*4*/ .dword dll_220_free
/*5*/ .dword dll_220_get_model_flags
/*6*/ .dword dll_220_get_data_size
/*7*/ .dword dll_220_func_704
