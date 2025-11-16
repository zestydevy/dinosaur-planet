.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCSabreChase_ctor
.dword WCSabreChase_dtor

# export table
/*0*/ .dword WCSabreChase_setup
/*1*/ .dword WCSabreChase_control
/*2*/ .dword WCSabreChase_update
/*3*/ .dword WCSabreChase_print
/*4*/ .dword WCSabreChase_free
/*5*/ .dword WCSabreChase_get_model_flags
/*6*/ .dword WCSabreChase_get_data_size
