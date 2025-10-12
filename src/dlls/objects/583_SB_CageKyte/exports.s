.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SBCageKyte_ctor
.dword SBCageKyte_dtor

# export table
/*0*/ .dword SBCageKyte_setup
/*1*/ .dword SBCageKyte_control
/*2*/ .dword SBCageKyte_update
/*3*/ .dword SBCageKyte_print
/*4*/ .dword SBCageKyte_free
/*5*/ .dword SBCageKyte_get_model_flags
/*6*/ .dword SBCageKyte_get_data_size
