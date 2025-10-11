.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_652_ctor
.dword dll_652_dtor

# export table
/*0*/ .dword dll_652_setup
/*1*/ .dword dll_652_control
/*2*/ .dword dll_652_update
/*3*/ .dword dll_652_print
/*4*/ .dword dll_652_free
/*5*/ .dword dll_652_get_model_flags
/*6*/ .dword dll_652_get_state_size
