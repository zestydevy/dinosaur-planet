.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_388_ctor
.dword dll_388_dtor

# export table
/*0*/ .dword dll_388_setup
/*1*/ .dword dll_388_control
/*2*/ .dword dll_388_update
/*3*/ .dword dll_388_print
/*4*/ .dword dll_388_free
/*5*/ .dword dll_388_get_model_flags
/*6*/ .dword dll_388_get_data_size
/*7*/ .dword dll_388_func_17F4
