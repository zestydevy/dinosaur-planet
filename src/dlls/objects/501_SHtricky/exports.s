.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHtricky_ctor
.dword SHtricky_dtor

# export table
/*0*/ .dword SHtricky_create
/*1*/ .dword SHtricky_update
/*2*/ .dword SHtricky_func_1A8
/*3*/ .dword SHtricky_draw
/*4*/ .dword SHtricky_destroy
/*5*/ .dword SHtricky_func_1DC
/*6*/ .dword SHtricky_get_state_size
