.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFPulley_ctor
.dword DFPulley_dtor

# export table
/*0*/ .dword DFPulley_setup
/*1*/ .dword DFPulley_control
/*2*/ .dword DFPulley_update
/*3*/ .dword DFPulley_print
/*4*/ .dword DFPulley_free
/*5*/ .dword DFPulley_get_model_flags
/*6*/ .dword DFPulley_get_data_size
