.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFriverflow_ctor
.dword DFriverflow_dtor

# export table
/*0*/ .dword DFriverflow_setup
/*1*/ .dword DFriverflow_control
/*2*/ .dword DFriverflow_update
/*3*/ .dword DFriverflow_print
/*4*/ .dword DFriverflow_free
/*5*/ .dword DFriverflow_get_model_flags
/*6*/ .dword DFriverflow_get_data_size
