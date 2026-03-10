.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SidekickToy_ctor
.dword SidekickToy_dtor

# export table
/*0*/ .dword SidekickToy_setup
/*1*/ .dword SidekickToy_control
/*2*/ .dword SidekickToy_update
/*3*/ .dword SidekickToy_print
/*4*/ .dword SidekickToy_free
/*5*/ .dword SidekickToy_get_model_flags
/*6*/ .dword SidekickToy_get_data_size
/*7*/ .dword SidekickToy_throw
/*8*/ .dword SidekickToy_carry
/*9*/ .dword SidekickToy_is_stopped
