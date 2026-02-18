.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword capy_ctor
.dword capy_dtor

# export table
/*0*/ .dword capy_setup
/*1*/ .dword capy_control
/*2*/ .dword capy_update
/*3*/ .dword capy_print
/*4*/ .dword capy_free
/*5*/ .dword capy_get_model_flags
/*6*/ .dword capy_get_data_size
