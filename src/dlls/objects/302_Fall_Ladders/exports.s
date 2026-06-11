.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_302_ctor
.dword dll_302_dtor

# export table
/*0*/ .dword dll_302_setup
/*1*/ .dword dll_302_control
/*2*/ .dword dll_302_update
/*3*/ .dword dll_302_print
/*4*/ .dword dll_302_free
/*5*/ .dword dll_302_get_model_flags
/*6*/ .dword dll_302_get_data_size
