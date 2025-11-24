.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Spirit_ctor
.dword Spirit_dtor

# export table
/*0*/ .dword Spirit_setup
/*1*/ .dword Spirit_control
/*2*/ .dword Spirit_update
/*3*/ .dword Spirit_print
/*4*/ .dword Spirit_free
/*5*/ .dword Spirit_get_model_flags
/*6*/ .dword Spirit_get_data_size
