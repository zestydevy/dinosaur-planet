.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Chuka_ctor
.dword Chuka_dtor

# export table
/*0*/ .dword Chuka_setup
/*1*/ .dword Chuka_control
/*2*/ .dword Chuka_update
/*3*/ .dword Chuka_print
/*4*/ .dword Chuka_free
/*5*/ .dword Chuka_get_model_flags
/*6*/ .dword Chuka_get_data_size
/*7*/ .dword Chuka_receive_message
