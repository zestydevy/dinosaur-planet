.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_711_ctor
.dword dll_711_dtor

# export table
/*0*/ .dword dll_711_setup
/*1*/ .dword dll_711_control
/*2*/ .dword dll_711_update
/*3*/ .dword dll_711_print
/*4*/ .dword dll_711_free
/*5*/ .dword dll_711_get_model_flags
/*6*/ .dword dll_711_get_data_size
/*7*/ .dword dll_711_func_14D4
/*8*/ .dword dll_711_func_1500
/*9*/ .dword dll_711_func_1510
/*10*/ .dword dll_711_func_1534
/*11*/ .dword dll_711_func_15A4
/*12*/ .dword dll_711_func_15B4
/*13*/ .dword dll_711_func_15D8
/*14*/ .dword dll_711_func_15E8
/*15*/ .dword dll_711_func_1638
/*16*/ .dword dll_711_func_16C4
/*17*/ .dword dll_711_func_1750
/*18*/ .dword dll_711_func_1760
/*19*/ .dword dll_711_func_1860
/*20*/ .dword dll_711_func_4780
