.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword LightPole_ctor
.dword LightPole_dtor

# export table
/*0*/ .dword LightPole_setup
/*1*/ .dword LightPole_control
/*2*/ .dword LightPole_update
/*3*/ .dword LightPole_print
/*4*/ .dword LightPole_free
/*5*/ .dword LightPole_get_model_flags
/*6*/ .dword LightPole_get_data_size
