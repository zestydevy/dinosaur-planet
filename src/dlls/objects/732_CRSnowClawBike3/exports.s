.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_732_ctor
.dword dll_732_dtor

# export table
/*0*/ .dword dll_732_setup
/*1*/ .dword dll_732_control
/*2*/ .dword dll_732_update
/*3*/ .dword dll_732_print
/*4*/ .dword dll_732_free
/*5*/ .dword dll_732_get_model_flags
/*6*/ .dword dll_732_get_data_size
/*7*/ .dword dll_732_func_18FC
/*8*/ .dword dll_732_func_1980
/*9*/ .dword dll_732_func_1990
/*10*/ .dword dll_732_func_19B4
/*11*/ .dword dll_732_func_1A24
/*12*/ .dword dll_732_func_1A34
/*13*/ .dword dll_732_func_1A58
/*14*/ .dword dll_732_func_1A68
/*15*/ .dword dll_732_func_1B10
/*16*/ .dword dll_732_func_1B9C
/*17*/ .dword dll_732_func_1C28
/*18*/ .dword dll_732_func_1C38
/*19*/ .dword dll_732_func_1DB8
/*20*/ .dword dll_732_func_4B30
