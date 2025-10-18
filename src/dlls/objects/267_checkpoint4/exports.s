.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword checkpoint4_ctor
.dword checkpoint4_dtor

# export table
/*0*/ .dword checkpoint4_setup
/*1*/ .dword checkpoint4_control
/*2*/ .dword checkpoint4_update
/*3*/ .dword checkpoint4_print
/*4*/ .dword checkpoint4_free
/*5*/ .dword checkpoint4_get_model_flags
/*6*/ .dword checkpoint4_get_data_size
/*7*/ .dword checkpoint4_func_278
