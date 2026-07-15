.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_Column_ctor
.dword WL_Column_dtor

# export table
/*0*/ .dword WL_Column_setup
/*1*/ .dword WL_Column_control
/*2*/ .dword WL_Column_update
/*3*/ .dword WL_Column_print
/*4*/ .dword WL_Column_free
/*5*/ .dword WL_Column_get_model_flags
/*6*/ .dword WL_Column_get_data_size
