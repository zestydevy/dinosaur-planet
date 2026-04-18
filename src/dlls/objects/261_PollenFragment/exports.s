.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword PollenFragment_ctor
.dword PollenFragment_dtor

# export table
/*0*/ .dword PollenFragment_setup
/*1*/ .dword PollenFragment_control
/*2*/ .dword PollenFragment_update
/*3*/ .dword PollenFragment_print
/*4*/ .dword PollenFragment_free
/*5*/ .dword PollenFragment_get_model_flags
/*6*/ .dword PollenFragment_get_data_size
