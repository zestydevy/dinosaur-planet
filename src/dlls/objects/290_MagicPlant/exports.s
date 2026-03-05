.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_290_ctor
.dword dll_290_dtor

# export table
/*0*/ .dword dll_290_setup
/*1*/ .dword dll_290_control
/*2*/ .dword dll_290_update
/*3*/ .dword dll_290_print
/*4*/ .dword dll_290_free
/*5*/ .dword dll_290_get_model_flags
/*6*/ .dword dll_290_get_data_size
