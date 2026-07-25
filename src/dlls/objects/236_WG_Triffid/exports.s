.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WGTriffid_ctor
.dword WGTriffid_dtor

# export table
/*0*/ .dword WGTriffid_setup
/*1*/ .dword WGTriffid_control
/*2*/ .dword WGTriffid_update
/*3*/ .dword WGTriffid_print
/*4*/ .dword WGTriffid_free
/*5*/ .dword WGTriffid_get_model_flags
/*6*/ .dword WGTriffid_get_data_size
