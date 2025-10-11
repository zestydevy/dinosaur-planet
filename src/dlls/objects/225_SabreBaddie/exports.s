.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_225_ctor
.dword dll_225_dtor

# export table
/*0*/ .dword dll_225_setup
/*1*/ .dword dll_225_control
/*2*/ .dword dll_225_update
/*3*/ .dword dll_225_print
/*4*/ .dword dll_225_free
/*5*/ .dword dll_225_get_model_flags
/*6*/ .dword dll_225_get_state_size
