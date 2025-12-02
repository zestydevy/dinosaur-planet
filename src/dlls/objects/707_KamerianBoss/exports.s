.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_707_ctor
.dword dll_707_dtor

# export table
/*0*/ .dword dll_707_setup
/*1*/ .dword dll_707_control
/*2*/ .dword dll_707_update
/*3*/ .dword dll_707_print
/*4*/ .dword dll_707_free
/*5*/ .dword dll_707_get_model_flags
/*6*/ .dword dll_707_get_data_size
