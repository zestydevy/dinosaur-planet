.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBBigNest_ctor
.dword DBBigNest_dtor

# export table
/*0*/ .dword DBBigNest_setup
/*1*/ .dword DBBigNest_control
/*2*/ .dword DBBigNest_update
/*3*/ .dword DBBigNest_print
/*4*/ .dword DBBigNest_free
/*5*/ .dword DBBigNest_get_model_flags
/*6*/ .dword DBBigNest_get_data_size
