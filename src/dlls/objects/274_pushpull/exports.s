.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_274_ctor
.dword dll_274_dtor

# export table
/*0*/ .dword dll_274_setup
/*1*/ .dword dll_274_control
/*2*/ .dword dll_274_update
/*3*/ .dword dll_274_print
/*4*/ .dword dll_274_free
/*5*/ .dword dll_274_get_model_flags
/*6*/ .dword dll_274_get_state_size
/*7*/ .dword dll_274_func_1190
/*8*/ .dword dll_274_func_176C
/*9*/ .dword dll_274_func_1814
/*10*/ .dword dll_274_func_1834
