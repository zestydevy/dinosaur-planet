.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword FireFlyLantern_ctor
.dword FireFlyLantern_dtor

# export table
/*0*/ .dword FireFlyLantern_setup
/*1*/ .dword FireFlyLantern_control
/*2*/ .dword FireFlyLantern_update
/*3*/ .dword FireFlyLantern_print
/*4*/ .dword FireFlyLantern_free
/*5*/ .dword FireFlyLantern_get_model_flags
/*6*/ .dword FireFlyLantern_get_data_size
