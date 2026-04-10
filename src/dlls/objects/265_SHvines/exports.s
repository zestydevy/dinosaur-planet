.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHvines_ctor
.dword SHvines_dtor

# export table
/*0*/ .dword SHvines_setup
/*1*/ .dword SHvines_control
/*2*/ .dword SHvines_update
/*3*/ .dword SHvines_print
/*4*/ .dword SHvines_free
/*5*/ .dword SHvines_get_model_flags
/*6*/ .dword SHvines_get_data_size
