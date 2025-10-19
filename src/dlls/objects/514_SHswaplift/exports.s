.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHswaplift_ctor
.dword SHswaplift_dtor

# export table
/*0*/ .dword SHswaplift_setup
/*1*/ .dword SHswaplift_control
/*2*/ .dword SHswaplift_update
/*3*/ .dword SHswaplift_print
/*4*/ .dword SHswaplift_free
/*5*/ .dword SHswaplift_get_model_flags
/*6*/ .dword SHswaplift_get_data_size
