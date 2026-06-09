.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_355_ctor
.dword dll_355_dtor

# export table
/*0*/ .dword dll_355_setup
/*1*/ .dword dll_355_control
/*2*/ .dword dll_355_update
/*3*/ .dword dll_355_print
/*4*/ .dword dll_355_free
/*5*/ .dword dll_355_get_model_flags
/*6*/ .dword dll_355_get_data_size
/*7*/ .dword dll_355_func_B54
/*8*/ .dword dll_355_func_C20
/*9*/ .dword dll_355_func_C3C
