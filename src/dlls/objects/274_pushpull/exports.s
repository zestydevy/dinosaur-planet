.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_274_ctor
.dword dll_274_dtor

# export table
/*0*/ .dword dll_274_create
/*1*/ .dword dll_274_update
/*2*/ .dword dll_274_func_87C
/*3*/ .dword dll_274_draw
/*4*/ .dword dll_274_destroy
/*5*/ .dword dll_274_get_setup_flags
/*6*/ .dword dll_274_get_state_size
/*7*/ .dword dll_274_func_1190
/*8*/ .dword dll_274_func_176C
/*9*/ .dword dll_274_func_1814
/*10*/ .dword dll_274_func_1834
