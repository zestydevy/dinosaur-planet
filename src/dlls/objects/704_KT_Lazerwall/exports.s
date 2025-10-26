.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_704_ctor
.dword dll_704_dtor

# export table
/*0*/ .dword dll_704_setup
/*1*/ .dword dll_704_control
/*2*/ .dword dll_704_update
/*3*/ .dword dll_704_print
/*4*/ .dword dll_704_free
/*5*/ .dword dll_704_get_model_flags
/*6*/ .dword dll_704_get_data_size
