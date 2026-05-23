.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DR_CloudPerch_ctor
.dword DR_CloudPerch_dtor

# export table
/*0*/ .dword DR_CloudPerch_setup
/*1*/ .dword DR_CloudPerch_control
/*2*/ .dword DR_CloudPerch_update
/*3*/ .dword DR_CloudPerch_print
/*4*/ .dword DR_CloudPerch_free
/*5*/ .dword DR_CloudPerch_get_model_flags
/*6*/ .dword DR_CloudPerch_get_data_size
/*7*/ .dword DR_CloudPerch_call
/*8*/ .dword DR_CloudPerch_land
