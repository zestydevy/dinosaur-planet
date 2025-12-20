.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCkrazoabright_ctor
.dword CCkrazoabright_dtor

# export table
/*0*/ .dword CCkrazoabright_setup
/*1*/ .dword CCkrazoabright_control
/*2*/ .dword CCkrazoabright_update
/*3*/ .dword CCkrazoabright_print
/*4*/ .dword CCkrazoabright_free
/*5*/ .dword CCkrazoabright_get_model_flags
/*6*/ .dword CCkrazoabright_get_data_size
