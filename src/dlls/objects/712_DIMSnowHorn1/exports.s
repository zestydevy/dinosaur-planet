.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_712_ctor
.dword dll_712_dtor

# export table
/*0*/ .dword dll_712_setup
/*1*/ .dword dll_712_control
/*2*/ .dword dll_712_update
/*3*/ .dword dll_712_print
/*4*/ .dword dll_712_free
/*5*/ .dword dll_712_get_model_flags
/*6*/ .dword dll_712_get_data_size
/*7*/ .dword dll_712_func_1150
/*8*/ .dword dll_712_func_124C
/*9*/ .dword dll_712_func_1274
/*10*/ .dword dll_712_func_1298
/*11*/ .dword dll_712_func_1310
/*12*/ .dword dll_712_func_1338
/*13*/ .dword dll_712_func_13FC
/*14*/ .dword dll_712_func_140C
/*15*/ .dword dll_712_func_141C
/*16*/ .dword dll_712_func_1444
/*17*/ .dword dll_712_func_148C
/*18*/ .dword dll_712_func_149C
/*19*/ .dword dll_712_func_14A8
/*20*/ .dword dll_712_func_3448
