.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ProjectileSwitch_ctor
.dword ProjectileSwitch_dtor

# export table
/*0*/ .dword ProjectileSwitch_setup
/*1*/ .dword ProjectileSwitch_control
/*2*/ .dword ProjectileSwitch_update
/*3*/ .dword ProjectileSwitch_print
/*4*/ .dword ProjectileSwitch_free
/*5*/ .dword ProjectileSwitch_get_model_flags
/*6*/ .dword ProjectileSwitch_get_data_size
