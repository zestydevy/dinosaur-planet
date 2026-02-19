.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_730_ctor
.dword dll_730_dtor

# export table
/*0*/ .dword dll_730_setup
/*1*/ .dword dll_730_control
/*2*/ .dword dll_730_update
/*3*/ .dword dll_730_print
/*4*/ .dword dll_730_free
/*5*/ .dword dll_730_get_model_flags
/*6*/ .dword dll_730_get_data_size
/*7*/ .dword dll_730_func_2EC
/*8*/ .dword dll_730_func_334
