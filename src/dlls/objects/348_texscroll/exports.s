.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TexScroll_ctor
.dword TexScroll_dtor

# export table
/*0*/ .dword TexScroll_setup
/*1*/ .dword TexScroll_control
/*2*/ .dword TexScroll_update
/*3*/ .dword TexScroll_print
/*4*/ .dword TexScroll_free
/*5*/ .dword TexScroll_get_model_flags
/*6*/ .dword TexScroll_get_data_size
