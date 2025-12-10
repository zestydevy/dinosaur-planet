.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_698_ctor
.dword dll_698_dtor

# export table
/*0*/ .dword dll_698_setup
/*1*/ .dword dll_698_control
/*2*/ .dword dll_698_update
/*3*/ .dword dll_698_print
/*4*/ .dword dll_698_free
/*5*/ .dword dll_698_get_model_flags
/*6*/ .dword dll_698_get_data_size
/*7*/ .dword dll_698_func_564
