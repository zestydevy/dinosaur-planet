.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCMeterBlock_ctor
.dword SCMeterBlock_dtor

# export table
/*0*/ .dword SCMeterBlock_setup
/*1*/ .dword SCMeterBlock_control
/*2*/ .dword SCMeterBlock_update
/*3*/ .dword SCMeterBlock_print
/*4*/ .dword SCMeterBlock_free
/*5*/ .dword SCMeterBlock_get_model_flags
/*6*/ .dword SCMeterBlock_get_data_size
