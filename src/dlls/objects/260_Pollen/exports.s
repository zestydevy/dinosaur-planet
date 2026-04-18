.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Pollen_ctor
.dword Pollen_dtor

# export table
/*0*/ .dword Pollen_setup
/*1*/ .dword Pollen_control
/*2*/ .dword Pollen_update
/*3*/ .dword Pollen_print
/*4*/ .dword Pollen_free
/*5*/ .dword Pollen_get_model_flags
/*6*/ .dword Pollen_get_data_size
