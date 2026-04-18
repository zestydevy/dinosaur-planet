.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCTotemPole_ctor
.dword SCTotemPole_dtor

# export table
/*0*/ .dword SCTotemPole_setup
/*1*/ .dword SCTotemPole_control
/*2*/ .dword SCTotemPole_update
/*3*/ .dword SCTotemPole_print
/*4*/ .dword SCTotemPole_free
/*5*/ .dword SCTotemPole_get_model_flags
/*6*/ .dword SCTotemPole_get_data_size
