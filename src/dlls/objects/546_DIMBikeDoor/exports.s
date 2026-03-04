.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_546_ctor
.dword dll_546_dtor

# export table
/*0*/ .dword dll_546_setup
/*1*/ .dword dll_546_control
/*2*/ .dword dll_546_update
/*3*/ .dword dll_546_print
/*4*/ .dword dll_546_free
/*5*/ .dword dll_546_get_model_flags
/*6*/ .dword dll_546_get_data_size
