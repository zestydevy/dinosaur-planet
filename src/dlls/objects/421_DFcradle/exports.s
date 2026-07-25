.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFCradle_ctor
.dword DFCradle_dtor

# export table
/*0*/ .dword DFCradle_setup
/*1*/ .dword DFCradle_control
/*2*/ .dword DFCradle_update
/*3*/ .dword DFCradle_print
/*4*/ .dword DFCradle_free
/*5*/ .dword DFCradle_get_model_flags
/*6*/ .dword DFCradle_get_data_size
