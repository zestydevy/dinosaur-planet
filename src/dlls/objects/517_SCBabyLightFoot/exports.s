.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_517_ctor
.dword dll_517_dtor

# export table
/*0*/ .dword dll_517_setup
/*1*/ .dword dll_517_control
/*2*/ .dword dll_517_update
/*3*/ .dword dll_517_print
/*4*/ .dword dll_517_free
/*5*/ .dword dll_517_get_model_flags
/*6*/ .dword dll_517_get_data_size
