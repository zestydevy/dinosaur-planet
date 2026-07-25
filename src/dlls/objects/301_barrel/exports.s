.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Barrel_ctor
.dword Barrel_dtor

# export table
/*0*/ .dword Barrel_setup
/*1*/ .dword Barrel_control
/*2*/ .dword Barrel_update
/*3*/ .dword Barrel_print
/*4*/ .dword Barrel_free
/*5*/ .dword Barrel_get_model_flags
/*6*/ .dword Barrel_get_data_size
