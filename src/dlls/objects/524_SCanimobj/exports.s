.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCAnimObj_ctor
.dword SCAnimObj_dtor

# export table
/*0*/ .dword SCAnimObj_setup
/*1*/ .dword SCAnimObj_control
/*2*/ .dword SCAnimObj_update
/*3*/ .dword SCAnimObj_print
/*4*/ .dword SCAnimObj_free
/*5*/ .dword SCAnimObj_get_model_flags
/*6*/ .dword SCAnimObj_get_data_size
