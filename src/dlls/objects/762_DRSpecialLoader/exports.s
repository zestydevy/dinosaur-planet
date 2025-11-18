.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_762_ctor
.dword dll_762_dtor

# export table
/*0*/ .dword dll_762_setup
/*1*/ .dword dll_762_control
/*2*/ .dword dll_762_update
/*3*/ .dword dll_762_print
/*4*/ .dword dll_762_free
/*5*/ .dword dll_762_get_model_flags
/*6*/ .dword dll_762_get_data_size
