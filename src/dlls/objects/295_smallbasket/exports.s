.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_295_ctor
.dword dll_295_dtor

# export table
/*0*/ .dword dll_295_create
/*1*/ .dword dll_295_update
/*2*/ .dword dll_295_func_C0C
/*3*/ .dword dll_295_draw
/*4*/ .dword dll_295_destroy
/*5*/ .dword dll_295_get_model_flags
/*6*/ .dword dll_295_get_state_size
