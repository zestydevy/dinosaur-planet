.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBSpike_ctor
.dword DBSpike_dtor

# export table
/*0*/ .dword DBSpike_setup
/*1*/ .dword DBSpike_control
/*2*/ .dword DBSpike_update
/*3*/ .dword DBSpike_print
/*4*/ .dword DBSpike_free
/*5*/ .dword DBSpike_get_model_flags
/*6*/ .dword DBSpike_get_data_size
