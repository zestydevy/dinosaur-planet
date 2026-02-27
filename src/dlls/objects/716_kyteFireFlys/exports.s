.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_716_ctor
.dword dll_716_dtor

# export table
/*0*/ .dword dll_716_setup
/*1*/ .dword dll_716_control
/*2*/ .dword dll_716_update
/*3*/ .dword dll_716_print
/*4*/ .dword dll_716_free
/*5*/ .dword dll_716_get_model_flags
/*6*/ .dword dll_716_get_data_size
/*7*/ .dword dll_716_func_354
/*8*/ .dword dll_716_func_49C
/*9*/ .dword dll_716_func_4B4
/*10*/ .dword dll_716_func_4CC
