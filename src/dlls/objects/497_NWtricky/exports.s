.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword NWtricky_ctor
.dword NWtricky_dtor

# export table
/*0*/ .dword NWtricky_setup
/*1*/ .dword NWtricky_control
/*2*/ .dword NWtricky_update
/*3*/ .dword NWtricky_print
/*4*/ .dword NWtricky_free
/*5*/ .dword NWtricky_get_model_flags
/*6*/ .dword NWtricky_get_data_size
