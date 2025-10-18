.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword IMIceMountain_ctor
.dword IMIceMountain_dtor

# export table
/*0*/ .dword IMIceMountain_setup
/*1*/ .dword IMIceMountain_control
/*2*/ .dword IMIceMountain_update
/*3*/ .dword IMIceMountain_print
/*4*/ .dword IMIceMountain_free
/*5*/ .dword IMIceMountain_get_model_flags
/*6*/ .dword IMIceMountain_get_data_size
