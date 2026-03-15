.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Cannon_ctor
.dword Cannon_dtor

# export table
/*0*/ .dword Cannon_setup
/*1*/ .dword Cannon_control
/*2*/ .dword Cannon_update
/*3*/ .dword Cannon_print
/*4*/ .dword Cannon_free
/*5*/ .dword Cannon_get_model_flags
/*6*/ .dword Cannon_get_data_size
