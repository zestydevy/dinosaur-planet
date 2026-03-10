.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_658_ctor
.dword dll_658_dtor

# export table
/*0*/ .dword dll_658_setup
/*1*/ .dword dll_658_control
/*2*/ .dword dll_658_update
/*3*/ .dword dll_658_print
/*4*/ .dword dll_658_free
/*5*/ .dword dll_658_get_model_flags
/*6*/ .dword dll_658_get_data_size
