.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Transporter_ctor
.dword Transporter_dtor

# export table
/*0*/ .dword Transporter_setup
/*1*/ .dword Transporter_control
/*2*/ .dword Transporter_update
/*3*/ .dword Transporter_print
/*4*/ .dword Transporter_free
/*5*/ .dword Transporter_get_model_flags
/*6*/ .dword Transporter_get_data_size
