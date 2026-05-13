.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword thorntail_ctor
.dword thorntail_dtor

# export table
/*0*/ .dword thorntail_setup
/*1*/ .dword thorntail_control
/*2*/ .dword thorntail_update
/*3*/ .dword thorntail_print
/*4*/ .dword thorntail_free
/*5*/ .dword thorntail_get_model_flags
/*6*/ .dword thorntail_get_data_size
