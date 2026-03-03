.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword curve_ctor
.dword curve_dtor

# export table
/*0*/ .dword curve_setup
/*1*/ .dword curve_control
/*2*/ .dword curve_update
/*3*/ .dword curve_print
/*4*/ .dword curve_free
/*5*/ .dword curve_get_model_flags
/*6*/ .dword curve_get_data_size
/*7*/ .dword curve_func_164
/*8*/ .dword curve_func_170
