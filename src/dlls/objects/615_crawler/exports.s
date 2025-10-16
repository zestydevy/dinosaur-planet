.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_615_ctor
.dword dll_615_dtor

# export table
/*0*/ .dword dll_615_setup
/*1*/ .dword dll_615_control
/*2*/ .dword dll_615_update
/*3*/ .dword dll_615_print
/*4*/ .dword dll_615_free
/*5*/ .dword dll_615_get_model_flags
/*6*/ .dword dll_615_get_data_size
