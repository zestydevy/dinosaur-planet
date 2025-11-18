.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFlevelcontrol_ctor
.dword DFlevelcontrol_dtor

# export table
/*0*/ .dword DFlevelcontrol_setup
/*1*/ .dword DFlevelcontrol_control
/*2*/ .dword DFlevelcontrol_update
/*3*/ .dword DFlevelcontrol_print
/*4*/ .dword DFlevelcontrol_free
/*5*/ .dword DFlevelcontrol_get_model_flags
/*6*/ .dword DFlevelcontrol_get_data_size
