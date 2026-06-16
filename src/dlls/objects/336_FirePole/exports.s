.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword FirePole_ctor
.dword FirePole_dtor

# export table
/*0*/ .dword FirePole_setup
/*1*/ .dword FirePole_control
/*2*/ .dword FirePole_update
/*3*/ .dword FirePole_print
/*4*/ .dword FirePole_free
/*5*/ .dword FirePole_get_model_flags
/*6*/ .dword FirePole_get_data_size
