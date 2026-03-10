.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_382_ctor
.dword dll_382_dtor

# export table
/*0*/ .dword dll_382_setup
/*1*/ .dword dll_382_control
/*2*/ .dword dll_382_update
/*3*/ .dword dll_382_print
/*4*/ .dword dll_382_free
/*5*/ .dword dll_382_get_model_flags
/*6*/ .dword dll_382_get_data_size
