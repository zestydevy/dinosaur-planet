.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_713_ctor
.dword dll_713_dtor

# export table
/*0*/ .dword dll_713_setup
/*1*/ .dword dll_713_control
/*2*/ .dword dll_713_update
/*3*/ .dword dll_713_print
/*4*/ .dword dll_713_free
/*5*/ .dword dll_713_get_model_flags
/*6*/ .dword dll_713_get_data_size
/*7*/ .dword dll_713_func_197C
/*8*/ .dword dll_713_func_1990
/*9*/ .dword dll_713_func_19B8
/*10*/ .dword dll_713_func_19DC
/*11*/ .dword dll_713_func_1BB8
/*12*/ .dword dll_713_func_1BE0
/*13*/ .dword dll_713_func_1CA4
/*14*/ .dword dll_713_func_1CB4
/*15*/ .dword dll_713_func_1D5C
/*16*/ .dword dll_713_func_1D84
/*17*/ .dword dll_713_func_1DAC
/*18*/ .dword dll_713_func_1DBC
/*19*/ .dword dll_713_func_1DC8
/*20*/ .dword dll_713_func_3420
