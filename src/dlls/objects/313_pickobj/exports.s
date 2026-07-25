.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword PickObj_ctor
.dword PickObj_dtor

# export table
/*0*/ .dword PickObj_setup
/*1*/ .dword PickObj_control
/*2*/ .dword PickObj_update
/*3*/ .dword PickObj_print
/*4*/ .dword PickObj_free
/*5*/ .dword PickObj_get_model_flags
/*6*/ .dword PickObj_get_data_size
