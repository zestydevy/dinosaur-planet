.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword animator_ctor
.dword animator_dtor

# export table
/*0*/ .dword animator_setup
/*1*/ .dword animator_control
/*2*/ .dword animator_update
/*3*/ .dword animator_print
/*4*/ .dword animator_free
/*5*/ .dword animator_get_model_flags
/*6*/ .dword animator_get_data_size
