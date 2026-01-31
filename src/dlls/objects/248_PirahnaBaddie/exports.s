.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_248_ctor
.dword dll_248_dtor

# export table
/*0*/ .dword dll_248_setup
/*1*/ .dword dll_248_control
/*2*/ .dword dll_248_update
/*3*/ .dword dll_248_print
/*4*/ .dword dll_248_free
/*5*/ .dword dll_248_get_model_flags
/*6*/ .dword dll_248_get_data_size
