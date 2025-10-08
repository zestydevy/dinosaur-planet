.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_283_ctor
.dword dll_283_dtor

# export table
/*0*/ .dword dll_283_create
/*1*/ .dword dll_283_update
/*2*/ .dword dll_283_func_324
/*3*/ .dword dll_283_draw
/*4*/ .dword dll_283_destroy
/*5*/ .dword dll_283_get_setup_flags
/*6*/ .dword dll_283_get_state_size
/*7*/ .dword dll_283_func_438
