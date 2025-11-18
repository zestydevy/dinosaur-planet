.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_612_ctor
.dword dll_612_dtor

# export table
/*0*/ .dword dll_612_setup
/*1*/ .dword dll_612_control
/*2*/ .dword dll_612_update
/*3*/ .dword dll_612_print
/*4*/ .dword dll_612_free
/*5*/ .dword dll_612_get_model_flags
/*6*/ .dword dll_612_get_data_size
