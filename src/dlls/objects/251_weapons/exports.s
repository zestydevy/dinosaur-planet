.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_251_ctor
.dword dll_251_dtor

# export table
/*0*/ .dword dll_251_setup
/*1*/ .dword dll_251_control
/*2*/ .dword dll_251_update
/*3*/ .dword dll_251_print
/*4*/ .dword dll_251_free
/*5*/ .dword dll_251_get_model_flags
/*6*/ .dword dll_251_get_data_size
/*7*/ .dword dll_251_func_B08
/*8*/ .dword dll_251_func_B30
/*9*/ .dword dll_251_func_B64
/*10*/ .dword dll_251_func_BC0
/*11*/ .dword dll_251_func_D98
/*12*/ .dword dll_251_func_DB0
/*13*/ .dword dll_251_func_DE0
/*14*/ .dword dll_251_func_DF0
/*15*/ .dword dll_251_func_E2C
/*16*/ .dword dll_251_func_E64
/*17*/ .dword dll_251_func_E84
/*18*/ .dword dll_251_func_E0C
/*19*/ .dword dll_251_func_E74
