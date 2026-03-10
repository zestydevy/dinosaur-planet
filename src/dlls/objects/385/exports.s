.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_385_ctor
.dword dll_385_dtor

# export table
/*0*/ .dword dll_385_setup
/*1*/ .dword dll_385_control
/*2*/ .dword dll_385_update
/*3*/ .dword dll_385_print
/*4*/ .dword dll_385_free
/*5*/ .dword dll_385_get_model_flags
/*6*/ .dword dll_385_get_data_size
