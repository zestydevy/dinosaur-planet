.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_243_ctor
.dword dll_243_dtor

# export table
/*0*/ .dword dll_243_setup
/*1*/ .dword dll_243_control
/*2*/ .dword dll_243_update
/*3*/ .dword dll_243_print
/*4*/ .dword dll_243_free
/*5*/ .dword dll_243_get_model_flags
/*6*/ .dword dll_243_get_data_size
/*7*/ .dword dll_243_func_6D8
/*8*/ .dword dll_243_func_6EC
