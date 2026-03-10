.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_772_ctor
.dword dll_772_dtor

# export table
/*0*/ .dword dll_772_setup
/*1*/ .dword dll_772_control
/*2*/ .dword dll_772_update
/*3*/ .dword dll_772_print
/*4*/ .dword dll_772_free
/*5*/ .dword dll_772_get_model_flags
/*6*/ .dword dll_772_get_data_size
