.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword NWsfx_ctor
.dword NWsfx_dtor

# export table
/*0*/ .dword NWsfx_setup
/*1*/ .dword NWsfx_control
/*2*/ .dword NWsfx_update
/*3*/ .dword NWsfx_print
/*4*/ .dword NWsfx_free
/*5*/ .dword NWsfx_get_model_flags
/*6*/ .dword NWsfx_get_data_size
