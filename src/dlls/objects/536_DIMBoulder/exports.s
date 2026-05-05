.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMBoulder_ctor
.dword DIMBoulder_dtor

# export table
/*0*/ .dword DIMBoulder_setup
/*1*/ .dword DIMBoulder_control
/*2*/ .dword DIMBoulder_update
/*3*/ .dword DIMBoulder_print
/*4*/ .dword DIMBoulder_free
/*5*/ .dword DIMBoulder_get_model_flags
/*6*/ .dword DIMBoulder_get_data_size
