.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_609_ctor
.dword dll_609_dtor

# export table
/*0*/ .dword dll_609_setup
/*1*/ .dword dll_609_control
/*2*/ .dword dll_609_update
/*3*/ .dword dll_609_print
/*4*/ .dword dll_609_free
/*5*/ .dword dll_609_get_model_flags
/*6*/ .dword dll_609_get_data_size
/*7*/ .dword dll_609_func_ADC
