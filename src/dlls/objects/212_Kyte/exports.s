.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_212_ctor
.dword dll_212_dtor

# export table
/*0*/ .dword dll_212_setup
/*1*/ .dword dll_212_control
/*2*/ .dword dll_212_update
/*3*/ .dword dll_212_print
/*4*/ .dword dll_212_free
/*5*/ .dword dll_212_get_model_flags
/*6*/ .dword dll_212_get_data_size
/*7*/ .dword dll_212_func_AB4
/*8*/ .dword dll_212_func_AC4
/*9*/ .dword dll_212_func_ADC
/*10*/ .dword dll_212_func_AEC
/*11*/ .dword dll_212_func_B04
/*12*/ .dword dll_212_func_B24
/*13*/ .dword dll_212_func_B34
/*14*/ .dword dll_212_func_B94
/*15*/ .dword dll_212_func_C04
/*16*/ .dword dll_212_func_C18
/*17*/ .dword dll_212_func_C2C
/*18*/ .dword dll_212_func_C48
/*19*/ .dword dll_212_func_C5C
/*20*/ .dword dll_212_func_C74
/*21*/ .dword dll_212_func_C8C
/*22*/ .dword dll_212_func_C9C
/*23*/ .dword dll_212_func_CDC
/*24*/ .dword dll_212_func_CAC
/*25*/ .dword dll_212_func_CCC
/*26*/ .dword dll_212_func_4040
