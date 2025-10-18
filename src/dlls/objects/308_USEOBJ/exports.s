.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword UseObj_ctor
.dword UseObj_dtor

# export table
/*0*/ .dword UseObj_setup
/*1*/ .dword UseObj_control
/*2*/ .dword UseObj_update
/*3*/ .dword UseObj_print
/*4*/ .dword UseObj_free
/*5*/ .dword UseObj_get_model_flags
/*6*/ .dword UseObj_get_data_size
