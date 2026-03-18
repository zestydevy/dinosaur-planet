.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword smallbasket_ctor
.dword smallbasket_dtor

# export table
/*0*/ .dword smallbasket_setup
/*1*/ .dword smallbasket_control
/*2*/ .dword smallbasket_update
/*3*/ .dword smallbasket_print
/*4*/ .dword smallbasket_free
/*5*/ .dword smallbasket_get_model_flags
/*6*/ .dword smallbasket_get_data_size
