.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFShrine_ctor
.dword DFShrine_dtor

# export table
/*0*/ .dword DFShrine_setup
/*1*/ .dword DFShrine_control
/*2*/ .dword DFShrine_update
/*3*/ .dword DFShrine_print
/*4*/ .dword DFShrine_free
/*5*/ .dword DFShrine_get_model_flags
/*6*/ .dword DFShrine_get_data_size
