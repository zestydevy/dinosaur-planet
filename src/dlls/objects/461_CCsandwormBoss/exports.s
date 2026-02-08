.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_461_ctor
.dword dll_461_dtor

# export table
/*0*/ .dword dll_461_setup
/*1*/ .dword dll_461_control
/*2*/ .dword dll_461_update
/*3*/ .dword dll_461_print
/*4*/ .dword dll_461_free
/*5*/ .dword dll_461_get_model_flags
/*6*/ .dword dll_461_get_data_size
