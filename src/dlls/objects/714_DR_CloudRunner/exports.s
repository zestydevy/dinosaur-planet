.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_714_ctor
.dword dll_714_dtor

# export table
/*0*/ .dword dll_714_create
/*1*/ .dword dll_714_update
/*2*/ .dword dll_714_func_B70
/*3*/ .dword dll_714_draw
/*4*/ .dword dll_714_destroy
/*5*/ .dword dll_714_get_setup_flags
/*6*/ .dword dll_714_get_state_size
/*7*/ .dword dll_714_func_F74
/*8*/ .dword dll_714_func_10BC
/*9*/ .dword dll_714_func_10E4
/*10*/ .dword dll_714_func_1108
/*11*/ .dword dll_714_func_1318
/*12*/ .dword dll_714_func_1340
/*13*/ .dword dll_714_func_1404
/*14*/ .dword dll_714_func_1414
/*15*/ .dword dll_714_func_1488
/*16*/ .dword dll_714_func_14B0
/*17*/ .dword dll_714_func_14D8
/*18*/ .dword dll_714_func_14E8
/*19*/ .dword dll_714_func_14F4
/*20*/ .dword dll_714_func_3560
