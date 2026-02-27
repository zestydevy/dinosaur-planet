.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_260_ctor
.dword dll_260_dtor

# export table
/*0*/ .dword dll_260_setup
/*1*/ .dword dll_260_control
/*2*/ .dword dll_260_update
/*3*/ .dword dll_260_print
/*4*/ .dword dll_260_free
/*5*/ .dword dll_260_get_model_flags
/*6*/ .dword dll_260_get_data_size
