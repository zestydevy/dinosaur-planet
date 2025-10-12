.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHtricky_ctor
.dword SHtricky_dtor

# export table
/*0*/ .dword SHtricky_setup
/*1*/ .dword SHtricky_control
/*2*/ .dword SHtricky_update
/*3*/ .dword SHtricky_print
/*4*/ .dword SHtricky_free
/*5*/ .dword SHtricky_get_model_flags
/*6*/ .dword SHtricky_get_data_size
