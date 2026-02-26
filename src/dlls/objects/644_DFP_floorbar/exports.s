.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFPfloorbar_ctor
.dword DFPfloorbar_dtor

# export table
/*0*/ .dword DFPfloorbar_setup
/*1*/ .dword DFPfloorbar_control
/*2*/ .dword DFPfloorbar_update
/*3*/ .dword DFPfloorbar_print
/*4*/ .dword DFPfloorbar_free
/*5*/ .dword DFPfloorbar_get_model_flags
/*6*/ .dword DFPfloorbar_get_data_size
