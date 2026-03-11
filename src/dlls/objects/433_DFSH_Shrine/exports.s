.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_433_ctor
.dword dll_433_dtor

# export table
/*0*/ .dword dll_433_setup
/*1*/ .dword dll_433_control
/*2*/ .dword dll_433_update
/*3*/ .dword dll_433_print
/*4*/ .dword dll_433_free
/*5*/ .dword dll_433_get_model_flags
/*6*/ .dword dll_433_get_data_size
