.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBFakeHorizon_ctor
.dword DBFakeHorizon_dtor

# export table
/*0*/ .dword DBFakeHorizon_setup
/*1*/ .dword DBFakeHorizon_control
/*2*/ .dword DBFakeHorizon_update
/*3*/ .dword DBFakeHorizon_print
/*4*/ .dword DBFakeHorizon_free
/*5*/ .dword DBFakeHorizon_get_model_flags
/*6*/ .dword DBFakeHorizon_get_data_size
