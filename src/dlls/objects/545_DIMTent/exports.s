.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMTent_ctor
.dword DIMTent_dtor

# export table
/*0*/ .dword DIMTent_setup
/*1*/ .dword DIMTent_control
/*2*/ .dword DIMTent_update
/*3*/ .dword DIMTent_print
/*4*/ .dword DIMTent_free
/*5*/ .dword DIMTent_get_model_flags
/*6*/ .dword DIMTent_get_data_size
