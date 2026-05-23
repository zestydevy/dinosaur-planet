.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword sideload_ctor
.dword sideload_dtor

# export table
/*0*/ .dword sideload_setup
/*1*/ .dword sideload_control
/*2*/ .dword sideload_update
/*3*/ .dword sideload_print
/*4*/ .dword sideload_free
/*5*/ .dword sideload_get_model_flags
/*6*/ .dword sideload_get_data_size
