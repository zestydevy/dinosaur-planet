.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMGate_ctor
.dword DIMGate_dtor

# export table
/*0*/ .dword DIMGate_setup
/*1*/ .dword DIMGate_control
/*2*/ .dword DIMGate_update
/*3*/ .dword DIMGate_print
/*4*/ .dword DIMGate_free
/*5*/ .dword DIMGate_get_model_flags
/*6*/ .dword DIMGate_get_data_size
