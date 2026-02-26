.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword scarab_ctor
.dword scarab_dtor

# export table
/*0*/ .dword scarab_setup
/*1*/ .dword scarab_control
/*2*/ .dword scarab_update
/*3*/ .dword scarab_print
/*4*/ .dword scarab_free
/*5*/ .dword scarab_get_model_flags
/*6*/ .dword scarab_get_data_size
