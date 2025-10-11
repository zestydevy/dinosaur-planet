.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_549_ctor
.dword dll_549_dtor

# export table
/*0*/ .dword dll_549_create
/*1*/ .dword dll_549_update
/*2*/ .dword dll_549_func_1D0
/*3*/ .dword dll_549_draw
/*4*/ .dword dll_549_destroy
/*5*/ .dword dll_549_get_model_flags
/*6*/ .dword dll_549_get_state_size
