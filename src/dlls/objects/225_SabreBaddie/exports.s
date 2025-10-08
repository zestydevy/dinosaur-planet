.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_225_ctor
.dword dll_225_dtor

# export table
/*0*/ .dword dll_225_create
/*1*/ .dword dll_225_update
/*2*/ .dword dll_225_func_450
/*3*/ .dword dll_225_draw
/*4*/ .dword dll_225_destroy
/*5*/ .dword dll_225_get_setup_flags
/*6*/ .dword dll_225_get_state_size
