.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WGSH_flybaddie_ctor
.dword WGSH_flybaddie_dtor

# export table
/*0*/ .dword WGSH_flybaddie_setup
/*1*/ .dword WGSH_flybaddie_control
/*2*/ .dword WGSH_flybaddie_update
/*3*/ .dword WGSH_flybaddie_print
/*4*/ .dword WGSH_flybaddie_free
/*5*/ .dword WGSH_flybaddie_get_model_flags
/*6*/ .dword WGSH_flybaddie_get_data_size
