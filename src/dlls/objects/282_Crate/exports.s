.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword crate_ctor
.dword crate_dtor

# export table
/*0*/ .dword crate_setup
/*1*/ .dword crate_control
/*2*/ .dword crate_update
/*3*/ .dword crate_print
/*4*/ .dword crate_free
/*5*/ .dword crate_get_model_flags
/*6*/ .dword crate_get_data_size
