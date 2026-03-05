.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_379_ctor
.dword dll_379_dtor

# export table
/*0*/ .dword dll_379_setup
/*1*/ .dword dll_379_control
/*2*/ .dword dll_379_update
/*3*/ .dword dll_379_print
/*4*/ .dword dll_379_free
/*5*/ .dword dll_379_get_model_flags
/*6*/ .dword dll_379_get_data_size
