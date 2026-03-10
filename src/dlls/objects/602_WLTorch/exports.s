.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_602_ctor
.dword dll_602_dtor

# export table
/*0*/ .dword dll_602_setup
/*1*/ .dword dll_602_control
/*2*/ .dword dll_602_update
/*3*/ .dword dll_602_print
/*4*/ .dword dll_602_free
/*5*/ .dword dll_602_get_model_flags
/*6*/ .dword dll_602_get_data_size
