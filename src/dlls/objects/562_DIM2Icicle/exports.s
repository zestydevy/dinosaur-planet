.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIM2Icicle_ctor
.dword DIM2Icicle_dtor

# export table
/*0*/ .dword DIM2Icicle_setup
/*1*/ .dword DIM2Icicle_control
/*2*/ .dword DIM2Icicle_update
/*3*/ .dword DIM2Icicle_print
/*4*/ .dword DIM2Icicle_free
/*5*/ .dword DIM2Icicle_get_model_flags
/*6*/ .dword DIM2Icicle_get_data_size
