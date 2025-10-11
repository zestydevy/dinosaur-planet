.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_295_ctor
.dword dll_295_dtor

# export table
/*0*/ .dword dll_295_setup
/*1*/ .dword dll_295_control
/*2*/ .dword dll_295_update
/*3*/ .dword dll_295_print
/*4*/ .dword dll_295_free
/*5*/ .dword dll_295_get_model_flags
/*6*/ .dword dll_295_get_state_size
