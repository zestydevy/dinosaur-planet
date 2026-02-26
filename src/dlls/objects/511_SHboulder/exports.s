.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHboulder_ctor
.dword SHboulder_dtor

# export table
/*0*/ .dword SHboulder_setup
/*1*/ .dword SHboulder_control
/*2*/ .dword SHboulder_update
/*3*/ .dword SHboulder_print
/*4*/ .dword SHboulder_free
/*5*/ .dword SHboulder_get_model_flags
/*6*/ .dword SHboulder_get_data_size
