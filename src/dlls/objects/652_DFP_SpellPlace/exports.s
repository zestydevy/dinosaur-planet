.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_652_ctor
.dword dll_652_dtor

# export table
/*0*/ .dword dll_652_create
/*1*/ .dword dll_652_update
/*2*/ .dword dll_652_func_1C4
/*3*/ .dword dll_652_draw
/*4*/ .dword dll_652_destroy
/*5*/ .dword dll_652_get_model_flags
/*6*/ .dword dll_652_get_state_size
