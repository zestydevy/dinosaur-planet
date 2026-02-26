.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_525_ctor
.dword dll_525_dtor

# export table
/*0*/ .dword dll_525_setup
/*1*/ .dword dll_525_control
/*2*/ .dword dll_525_update
/*3*/ .dword dll_525_print
/*4*/ .dword dll_525_free
/*5*/ .dword dll_525_get_model_flags
/*6*/ .dword dll_525_get_data_size
