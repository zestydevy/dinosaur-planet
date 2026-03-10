.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFwhirlpool_ctor
.dword DFwhirlpool_dtor

# export table
/*0*/ .dword DFwhirlpool_setup
/*1*/ .dword DFwhirlpool_control
/*2*/ .dword DFwhirlpool_update
/*3*/ .dword DFwhirlpool_print
/*4*/ .dword DFwhirlpool_free
/*5*/ .dword DFwhirlpool_get_model_flags
/*6*/ .dword DFwhirlpool_get_data_size
