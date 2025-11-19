.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_331_ctor
.dword dll_331_dtor

# export table
/*0*/ .dword dll_331_setup
/*1*/ .dword dll_331_control
/*2*/ .dword dll_331_update
/*3*/ .dword dll_331_print
/*4*/ .dword dll_331_free
/*5*/ .dword dll_331_get_model_flags
/*6*/ .dword dll_331_get_data_size
