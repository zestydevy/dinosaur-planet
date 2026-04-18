.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCTotemBond_ctor
.dword SCTotemBond_dtor

# export table
/*0*/ .dword SCTotemBond_setup
/*1*/ .dword SCTotemBond_control
/*2*/ .dword SCTotemBond_update
/*3*/ .dword SCTotemBond_print
/*4*/ .dword SCTotemBond_free
/*5*/ .dword SCTotemBond_get_model_flags
/*6*/ .dword SCTotemBond_get_data_size
