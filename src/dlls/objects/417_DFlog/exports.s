.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_417_ctor
.dword dll_417_dtor

# export table
/*0*/ .dword dll_417_setup
/*1*/ .dword dll_417_control
/*2*/ .dword dll_417_update
/*3*/ .dword dll_417_print
/*4*/ .dword dll_417_free
/*5*/ .dword dll_417_get_model_flags
/*6*/ .dword dll_417_get_data_size
/*7*/ .dword dll_417_func_400
/*8*/ .dword dll_417_func_490
/*9*/ .dword dll_417_func_5D8
/*10*/ .dword dll_417_func_5F8
/*11*/ .dword dll_417_func_75C
/*12*/ .dword dll_417_func_76C
/*13*/ .dword dll_417_func_85C
/*14*/ .dword dll_417_func_86C
/*15*/ .dword dll_417_func_944
/*16*/ .dword dll_417_func_9D8
/*17*/ .dword dll_417_func_A64
/*18*/ .dword dll_417_func_A74
/*19*/ .dword dll_417_func_A80
/*20*/ .dword dll_417_func_21E4
