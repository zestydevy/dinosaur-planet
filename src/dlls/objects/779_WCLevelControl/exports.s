.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_779_ctor
.dword dll_779_dtor

# export table
/*0*/ .dword dll_779_setup
/*1*/ .dword dll_779_control
/*2*/ .dword dll_779_update
/*3*/ .dword dll_779_print
/*4*/ .dword dll_779_free
/*5*/ .dword dll_779_get_model_flags
/*6*/ .dword dll_779_get_data_size
/*7*/ .dword dll_779_func_464
/*8*/ .dword dll_779_func_534
/*9*/ .dword dll_779_func_64C
/*10*/ .dword dll_779_func_6C0
/*11*/ .dword dll_779_func_730
/*12*/ .dword dll_779_func_7FC
/*13*/ .dword dll_779_func_8C8
/*14*/ .dword dll_779_func_BA8
/*15*/ .dword dll_779_func_C78
/*16*/ .dword dll_779_func_D90
/*17*/ .dword dll_779_func_E04
/*18*/ .dword dll_779_func_E74
/*19*/ .dword dll_779_func_F40
/*20*/ .dword dll_779_func_100C
