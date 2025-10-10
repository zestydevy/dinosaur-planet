.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_297_ctor
.dword dll_297_dtor

# export table
/*0*/ .dword dll_297_create
/*1*/ .dword dll_297_update
/*2*/ .dword dll_297_func_1164
/*3*/ .dword dll_297_draw
/*4*/ .dword dll_297_destroy
/*5*/ .dword dll_297_get_model_flags
/*6*/ .dword dll_297_get_state_size
