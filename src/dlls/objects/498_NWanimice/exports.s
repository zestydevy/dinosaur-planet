.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_498_ctor
.dword dll_498_dtor

# export table
/*0*/ .dword dll_498_setup
/*1*/ .dword dll_498_control
/*2*/ .dword dll_498_update
/*3*/ .dword dll_498_print
/*4*/ .dword dll_498_free
/*5*/ .dword dll_498_get_model_flags
/*6*/ .dword dll_498_get_data_size
