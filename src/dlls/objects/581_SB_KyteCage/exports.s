.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword kyteCage_ctor
.dword kyteCage_dtor

# export table
/*0*/ .dword kyteCage_setup
/*1*/ .dword kyteCage_control
/*2*/ .dword kyteCage_update
/*3*/ .dword kyteCage_print
/*4*/ .dword kyteCage_free
/*5*/ .dword kyteCage_get_model_flags
/*6*/ .dword kyteCage_get_data_size
