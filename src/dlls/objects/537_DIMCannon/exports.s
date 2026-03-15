.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_537_ctor
.dword dll_537_dtor

# export table
/*0*/ .dword dll_537_setup
/*1*/ .dword dll_537_control
/*2*/ .dword dll_537_update
/*3*/ .dword dll_537_print
/*4*/ .dword dll_537_free
/*5*/ .dword dll_537_get_model_flags
/*6*/ .dword dll_537_get_data_size
