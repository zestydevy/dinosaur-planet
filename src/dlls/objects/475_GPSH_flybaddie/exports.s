.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GPSH_flybaddie_ctor
.dword GPSH_flybaddie_dtor

# export table
/*0*/ .dword GPSH_flybaddie_setup
/*1*/ .dword GPSH_flybaddie_control
/*2*/ .dword GPSH_flybaddie_update
/*3*/ .dword GPSH_flybaddie_print
/*4*/ .dword GPSH_flybaddie_free
/*5*/ .dword GPSH_flybaddie_get_model_flags
/*6*/ .dword GPSH_flybaddie_get_data_size
