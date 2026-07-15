.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_587_ctor
.dword dll_587_dtor

# export table
/*0*/ .dword dll_587_setup
/*1*/ .dword dll_587_control
/*2*/ .dword dll_587_update
/*3*/ .dword dll_587_print
/*4*/ .dword dll_587_free
/*5*/ .dword dll_587_get_model_flags
/*6*/ .dword dll_587_get_data_size
