.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_793_ctor
.dword dll_793_dtor

# export table
/*0*/ .dword dll_793_setup
/*1*/ .dword dll_793_control
/*2*/ .dword dll_793_update
/*3*/ .dword dll_793_print
/*4*/ .dword dll_793_free
/*5*/ .dword dll_793_get_model_flags
/*6*/ .dword dll_793_get_data_size
/*7*/ .dword dll_793_func_950
/*8*/ .dword dll_793_func_9E0
/*9*/ .dword dll_793_func_B28
/*10*/ .dword dll_793_func_B48
/*11*/ .dword dll_793_func_C3C
/*12*/ .dword dll_793_func_C4C
/*13*/ .dword dll_793_func_D08
/*14*/ .dword dll_793_func_D18
/*15*/ .dword dll_793_func_E2C
/*16*/ .dword dll_793_func_E48
/*17*/ .dword dll_793_func_E70
/*18*/ .dword dll_793_func_E80
/*19*/ .dword dll_793_func_E8C
/*20*/ .dword dll_793_func_E9C
