.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCTotemStrength_ctor
.dword SCTotemStrength_dtor

# export table
/*0*/ .dword SCTotemStrength_setup
/*1*/ .dword SCTotemStrength_control
/*2*/ .dword SCTotemStrength_update
/*3*/ .dword SCTotemStrength_print
/*4*/ .dword SCTotemStrength_free
/*5*/ .dword SCTotemStrength_get_model_flags
/*6*/ .dword SCTotemStrength_get_data_size
