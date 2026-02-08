.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword fish_ctor
.dword fish_dtor

# export table
/*0*/ .dword fish_setup
/*1*/ .dword fish_control
/*2*/ .dword fish_update
/*3*/ .dword fish_print
/*4*/ .dword fish_free
/*5*/ .dword fish_get_model_flags
/*6*/ .dword fish_get_data_size
