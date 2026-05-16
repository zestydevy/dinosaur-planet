.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFbarrel_ctor
.dword DFbarrel_dtor

# export table
/*0*/ .dword DFbarrel_setup
/*1*/ .dword DFbarrel_control
/*2*/ .dword DFbarrel_update
/*3*/ .dword DFbarrel_print
/*4*/ .dword DFbarrel_free
/*5*/ .dword DFbarrel_get_model_flags
/*6*/ .dword DFbarrel_get_data_size
