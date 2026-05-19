.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DR_EarthCallPad_ctor
.dword DR_EarthCallPad_dtor

# export table
/*0*/ .dword DR_EarthCallPad_setup
/*1*/ .dword DR_EarthCallPad_control
/*2*/ .dword DR_EarthCallPad_update
/*3*/ .dword DR_EarthCallPad_print
/*4*/ .dword DR_EarthCallPad_free
/*5*/ .dword DR_EarthCallPad_get_model_flags
/*6*/ .dword DR_EarthCallPad_get_data_size
/*7*/ .dword DR_EarthCallPad_call
