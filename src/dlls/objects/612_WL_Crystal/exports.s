.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_Crystal_ctor
.dword WL_Crystal_dtor

# export table
/*0*/ .dword WL_Crystal_setup
/*1*/ .dword WL_Crystal_control
/*2*/ .dword WL_Crystal_update
/*3*/ .dword WL_Crystal_print
/*4*/ .dword WL_Crystal_free
/*5*/ .dword WL_Crystal_get_model_flags
/*6*/ .dword WL_Crystal_get_data_size
