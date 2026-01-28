.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_318_ctor
.dword dll_318_dtor

# export table
/*0*/ .dword dll_318_setup
/*1*/ .dword dll_318_control
/*2*/ .dword dll_318_update
/*3*/ .dword dll_318_print
/*4*/ .dword dll_318_free
/*5*/ .dword dll_318_get_model_flags
/*6*/ .dword dll_318_get_data_size
