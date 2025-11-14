.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_787_ctor
.dword dll_787_dtor

# export table
/*0*/ .dword dll_787_setup
/*1*/ .dword dll_787_control
/*2*/ .dword dll_787_update
/*3*/ .dword dll_787_print
/*4*/ .dword dll_787_free
/*5*/ .dword dll_787_get_model_flags
/*6*/ .dword dll_787_get_data_size
